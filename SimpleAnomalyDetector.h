

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
    float xP;
    float yP;
};

class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
protected:
	vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector(float th=0.9);
	virtual ~SimpleAnomalyDetector();

    void makeArrOfPoints(Point** const &dest,float* src1,float* src2, int arrSize);
    float getThreshold(Point** const &points,Line lin_reg ,int arrSize);
    void makeNewCF(const TimeSeries &ts, int i,int j,Point** points, float cor);
    void setTH(float th){ this->threshold = th; }
    virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

    virtual float devCal(Point *point, correlatedFeatures feature);
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
