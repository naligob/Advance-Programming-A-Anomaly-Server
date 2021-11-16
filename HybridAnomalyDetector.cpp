
#include "HybridAnomalyDetector.h"
#include <iostream>

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
    int limit = ts.getNumberOfFi();
    int arrSize = ts.getNumberOfMembers();
    Point **points = new Point * [arrSize];
	//Point *points[ts.getNumberOfMembers()];
	float *arr1 = new float[arrSize];
	float *arr2 = new float[arrSize];
	// float arr1[2175] , arr2[2175];
    float currentCorl, corelation;
    int saveI,saveJ;
    bool flag;
    for (size_t i = 0; i < limit - 1; i++)
    {
        flag = true;
        currentCorl = 0;
        ts.getArrOfCol(i,arr1);
        for (size_t j = i + 1; j < limit; j++)
        {
            ts.getArrOfCol(j,arr2);
            corelation = pearson(arr1,arr2,arrSize);
            if (corelation >= this->threshold && corelation > currentCorl) {
                flag = false;
                currentCorl = corelation;
                saveI = i;
                saveJ = j;
                makeArrOfPoints(points,arr1,arr2,arrSize);
            } else if (flag && corelation >= 0.5 && corelation > currentCorl) {
                currentCorl = corelation;
                saveI = i;
                saveJ = j;
                makeArrOfPoints(points,arr1,arr2,arrSize);
            }
        }
        if (currentCorl != 0){
            if (!flag) {
                makeNewCF(ts, saveI, saveJ, points, currentCorl);
            } else {
                makeNewCFCircle(ts, saveI, saveJ, points, currentCorl);
            }
            // for (Point* point : points) {
            //     delete(point);
            // }
        }
    }
}
vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
    return SimpleAnomalyDetector::detect(ts);
}
void HybridAnomalyDetector::makeNewCFCircle(const TimeSeries &ts, int i
                                            , int j, Point **pPoint, float corl) {
    int arrSize = ts.getNumberOfMembers();
    Circle minCircle = findMinCircle(pPoint, arrSize);
    struct correlatedFeatures newCF;
    newCF.corrlation = corl;
    newCF.feature1 = ts.valueFIByIndex(i);
    newCF.feature2 = ts.valueFIByIndex(j);
    newCF.threshold = 1.1 * minCircle.radius;
    newCF.xP = minCircle.center.x;
    newCF.yP = minCircle.center.y;
    this->cf.push_back(newCF);
}
float HybridAnomalyDetector::devCal(Point *point, correlatedFeatures feature){
    if (feature.corrlation >= this->threshold) {
        return dev(*point,feature.lin_reg);
    }
    else {
        return dist(*point,Point(feature.xP,feature.yP)); // from circle
    }
}
