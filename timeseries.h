#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <sstream>

using namespace std;


class TimeSeries{

	vector<vector<string>> m_table;
	vector<vector<float>> m_tableFloat;
	vector<vector<float>> m_tableTranspose;
	void insertColums(ifstream& data);
	vector<string> getColumsInfo(int index=0) const;
	void tableTranspose();

public:

	TimeSeries(const char* CSVfileName);
	void insertLine(string newLine);
	string valueFIAtTime(string fi,string time) const;
	string valueFIByIndex(int index) const;

	ifstream openFile(const char* file);
	vector<vector<string>> getTable() const {
		return m_table;
	}
	vector<string> getColumsName(int index=0) const {
		return getColumsInfo();
	}	
	string valueFIAtTime(string fi,float time) const;
	vector<float> getColomnData(string fiName) const;
	vector<float> getColomnData(int index) const;	
	int getNumberOfFi() const;
	void getArrOfCol(int index,float arr[]) const;
	int getNumberOfMembers() const;
	int findIndexColoms(string nameFi) const;
	
};


#endif /* TIMESERIES_H_ */