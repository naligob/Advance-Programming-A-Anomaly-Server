
#include "SimpleAnomalyDetector.h"


SimpleAnomalyDetector::SimpleAnomalyDetector(float th) {
	// TODO Auto-generated constructor stub
    this->threshold = th;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	int limit = ts.getNumberOfFi();
	int arrSize = ts.getNumberOfMembers();
	Point **points = new Point * [arrSize];
	//Point *points[ts.getNumberOfMembers()];
	float *arr1 = new float[arrSize];
	float *arr2 = new float[arrSize];
	// float arr1[2175] , arr2[2175];
	float currentCorl, corelation;
    int saveI,saveJ;
	for (size_t i = 0; i < limit - 1; i++)
	{
		currentCorl = 0;
		ts.getArrOfCol(i,arr1);
		for (size_t j = i + 1; j < limit; j++)
		{
			ts.getArrOfCol(j,arr2);
			corelation = pearson(arr1,arr2,arrSize);
			if (corelation >= this->threshold && corelation > currentCorl) {
			    currentCorl = corelation;
		        saveI = i;
		        saveJ = j;
                makeArrOfPoints(points,arr1,arr2,arrSize);
			}					
		}	
		if (currentCorl != 0){
		    makeNewCF(ts,saveI,saveJ,points,currentCorl);
			// for (size_t i = 0; i < arrSize; i++)
			// {
			// 	delete[] points[i];
			// }
			// delete
            // for (Point* point : points) {
            //     delete(point);
            // }
		}		
	}
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
	vector<AnomalyReport> ar;
	int arrSize = ts.getNumberOfMembers();
	Point **points = new Point * [arrSize];
	//Point *points[ts.getNumberOfMembers()];
	float *arr1 = new float[arrSize];
	float *arr2 = new float[arrSize];
	// float arr1[2175] , arr2[2175];
	int timeStemp = 1;
	for(correlatedFeatures feature : cf) {
		string feature1 = feature.feature1;
		string feature2 = feature.feature2;
		ts.getArrOfCol(ts.findIndexColoms(feature1),arr1);
		ts.getArrOfCol(ts.findIndexColoms(feature2),arr2);		
		makeArrOfPoints(points,arr1,arr2,arrSize);
		// for(Point *point : points) 
		for(int i=0; i< arrSize; i++){
			float devNumber = devCal(points[i], feature); /*dev(*point,feature.lin_reg);*/
			if(feature.threshold < devNumber){
				AnomalyReport report(feature1+" <---> "+feature2,timeStemp);
				ar.push_back(report);				
			}			
			timeStemp++;
		}
		timeStemp = 1;		
	}
	return ar;
}
void SimpleAnomalyDetector::makeNewCF(const TimeSeries &ts, int i,int j,Point** points, float cor) {
    struct correlatedFeatures newCF;
    int arrSize = ts.getNumberOfMembers();
    newCF.corrlation = cor;
    newCF.feature1 = ts.valueFIByIndex(i);
    newCF.feature2 = ts.valueFIByIndex(j);
    newCF.lin_reg = linear_reg(points,arrSize);
    newCF.threshold = 1.1 * getThreshold(points,newCF.lin_reg,arrSize);
    this->cf.push_back(newCF);
}
void SimpleAnomalyDetector::makeArrOfPoints(Point** const &points,float* arr1,float* arr2 , int arrSize) {
	for (size_t i = 0; i < arrSize; i++)
	{
		Point *point= new Point(arr1[i],arr2[i]);
		points[i]=point;
	}
}
float SimpleAnomalyDetector::getThreshold(Point** const &points,Line lin_reg ,int arrSize) {
    float maxDev = 0, currentDev;
    for (size_t i = 0; i < arrSize; i++) {
        Point point(points[i]->x, points[i]->y);
        currentDev = dev(point, lin_reg);
        if (currentDev > maxDev) maxDev = currentDev;
    }
    return maxDev;
}
float SimpleAnomalyDetector::devCal(Point *point, correlatedFeatures feature){
    return dev(*point,feature.lin_reg);
}