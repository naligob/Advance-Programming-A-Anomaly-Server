

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string>
#include <sstream>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes
struct commandsDataCollector{
    TimeSeries *train= nullptr;
    TimeSeries *test= nullptr;
    float th = 0.9;
    HybridAnomalyDetector had;
    vector<AnomalyReport> ar;
};
struct unifiedDateReport{
    string description;
    int timeStart;
    int timeEnd;
};

// you may edit this class
class Command{
    string description;
	DefaultIO* dio;
protected:
    void setDescription(string desc){
        this->description = desc;
    }
    DefaultIO* getIO() {
        return this->dio;
    }
public:
	Command(DefaultIO* dio):dio(dio){
    }
    string printDescription(){
        return this->description;
    }
	virtual void execute(commandsDataCollector &cdc)=0;
	virtual ~Command(){}
};

// implement here your command classes
class UploadCommand: public Command {

    void fillCSV(ofstream &in, commandsDataCollector &cdc){
        string input = Command::getIO()->read();
        while (input.compare("done") != 0) {
            in << input+"\n";
            input = Command::getIO()->read();
        }
        in.close();
    }

public:
    UploadCommand(DefaultIO *dio) : Command(dio) {
        Command::setDescription("1.upload a time series csv file");
    }

    void makeTrainCSV(commandsDataCollector &cdc) {
        ofstream in;
        in.open("train.csv");
        fillCSV(in,cdc);
        cdc.train = new TimeSeries("train.csv");
    }

    void makeTestCSV(commandsDataCollector &cdc) {
        ofstream in;
        in.open("test.csv");
        fillCSV(in,cdc);
        cdc.test = new TimeSeries("test.csv");
    }

    void execute(commandsDataCollector &cdc) override {
        Command::getIO()->write("Please upload your local train CSV file.\n");
        makeTrainCSV(cdc);
        Command::getIO()->write("Upload complete.\n");
        Command::getIO()->write("Please upload your local test CSV file.\n");
        makeTestCSV(cdc);
        Command::getIO()->write("Upload complete.\n");
    }
};

class SetAlgorithmCommand: public Command {

public:
    SetAlgorithmCommand(DefaultIO *dio) : Command(dio) {
        Command::setDescription("2.algorithm settings");
    }
    void execute(commandsDataCollector &cdc) override {
        Command::getIO()->write("The current correlation threshold is ");
        Command::getIO()->write(cdc.th);
        Command::getIO()->write("\nType a new threshold\n");

        float input;
        do {
            Command::getIO()->read(&input);
            if (input > 1 || input < 0) {
                std::cout << "please choose a value between 0 and 1.\n" << std::endl;
            } else {
                cdc.th = input;
                break;
            }
        } while (true);
    }
};

class AnomalyDetectionCommand: public Command {

public:
    AnomalyDetectionCommand(DefaultIO *dio) : Command(dio) {
        Command::setDescription("3.detect anomalies");
    }
    void execute(commandsDataCollector &cdc) override {
        cdc.had.setTH(cdc.th);
        cdc.had.learnNormal(*cdc.train);
        Command::getIO()->write("anomaly detection complete.\n");
    }
};

class ResultCommand: public Command {

public:
    ResultCommand(DefaultIO *dio) : Command(dio) {
        Command::setDescription("4.display results");
    }
    void execute(commandsDataCollector &cdc) override {
        cdc.ar = cdc.had.detect(*cdc.test);
        for(AnomalyReport ar : cdc.ar){
            Command::getIO()->write(ar.timeStep);
            Command::getIO()->write(" \t"+ar.description+"\n");
        }
        Command::getIO()->write("Done.\n");
    }
};

class UploadAnoAnalyzeCommand: public Command {

public:
    UploadAnoAnalyzeCommand(DefaultIO *dio) : Command(dio) {
        Command::setDescription("5.upload anomalies and analyze results");
    }

    void makeN_P_FileExeptionData(int &p, int &n, vector<unifiedDateReport> &report) {
        unifiedDateReport udf;
        string input = Command::getIO()->read();
        string number;
        while (input.compare("done") != 0){
            p++;
            stringstream line(input);
            getline(line,number,',');
            udf.description = "";
            udf.timeStart = stof(number);
            getline(line,number,',');
            udf.timeEnd = stof(number);
            input = Command::getIO()->read();
            report.push_back(udf);
            n -= - udf.timeStart + udf.timeEnd + 1;
        }
        Command::getIO()->write("Upload complete.\n");
    }

    void makeNewCF(vector<unifiedDateReport> &udr , vector<AnomalyReport> &arAnalyze ) {
        unifiedDateReport udf;
        int size = arAnalyze.size();
        bool same = true;
        int i = 0;
        while(i<size) {
            udf.description = arAnalyze[i].description;
            udf.timeStart = arAnalyze[i].timeStep;
            for (;i<size;i++) {
                if(arAnalyze[i].description.compare(udf.description) == 0) {
                } else {
                    udf.timeEnd = arAnalyze[i - 1].timeStep;
                    break;
                }
            }
            if (i == size) {
                udf.timeEnd = arAnalyze[i - 1].timeStep;
            }
            udr.push_back(udf);
        }
    }

    void updateTP_FP(float &tp, float &fp, vector<unifiedDateReport> &trainReport,
                     vector<unifiedDateReport> &dataReport) {
        int start , end;
        bool status;
        for (unifiedDateReport train : trainReport) {
            status = false;
            start = train.timeStart;
            end = train.timeEnd;
            for (unifiedDateReport data : dataReport) {
                if ((start <= data.timeStart && end >= data.timeStart)
                || (start <= data.timeEnd && end >= data.timeEnd)
                || (start >= data.timeStart && end <= data.timeEnd)) { status = true; }
            }
            if (status) { tp++; }
            else { fp++; }
        }
    }

    void execute(commandsDataCollector &cdc) override {
        // make TP
        Command::getIO()->write("Please upload your local anomalies file.\n");
        vector<unifiedDateReport> dataTrainReport;
        makeNewCF(dataTrainReport , cdc.ar);
        int p = 0;
        int N = cdc.train->getNumberOfMembers();
        vector<unifiedDateReport> dataFileReport;
        makeN_P_FileExeptionData(p, N, dataFileReport);
        // make fp
        float tp=0 , fp=0;
        updateTP_FP(tp,fp,dataTrainReport,dataFileReport);
        // make N
        Command::getIO()->write("True Positive Rate: ");
//        int temp = 1000 * fp / p;
//        float result = temp / 1000;
        Command::getIO()->write(floorf((float)tp/p*1000)/1000);
        Command::getIO()->write("\nFalse Positive Rate: ");
        Command::getIO()->write(fp/N/*floorf((float)fp/N*1000)/1000*/);
        Command::getIO()->write("\n");
    }
};

class ExitCommand: public Command {

public:
    ExitCommand(DefaultIO *dio) : Command(dio) {
        Command::setDescription("6.exit");
    }
    void execute(commandsDataCollector &cdc) override {
        delete(cdc.test);
        delete(cdc.train);
    }
};

#endif /* COMMANDS_H_ */
