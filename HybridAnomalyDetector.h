

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    void makeNewCFCircle(const TimeSeries &series, int i, int j, Point **pPoint, float corl);

    float devCal(Point *point, correlatedFeatures feature) override ;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
