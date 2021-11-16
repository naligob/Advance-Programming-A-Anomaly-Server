#include <string>
#include "AnomalyAlg.h"


std::vector<AnomalyReport> detectForCircle( std::string data, std::string test ) {

    const char* pathTS = data.c_str();

    TimeSeries ts(pathTS);
    HybridAnomalyDetector had;
    had.learnNormal(ts);

    const char* pathTest = test.c_str();
    TimeSeries testTS(pathTest);

    std::vector<AnomalyReport> ar = had.detect(testTS);

    return ar;
}

std::vector<AnomalyReport> detectForLinearReg( std::string data, std::string test ) {
    const char* pathTS = data.c_str();

    TimeSeries ts(pathTS);
    SimpleAnomalyDetector sad;
    sad.learnNormal(ts);

    const char* pathTest = test.c_str();
    TimeSeries testTS(pathTest);

    std::vector<AnomalyReport> ar = sad.detect(testTS);

    return ar;
}