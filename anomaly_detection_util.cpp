#include <iostream>
#include <string>
#include <cmath>
#include "anomaly_detection_util.h"

// returns the expectation of value 
float valOfExpectation(float* x,int size);

float avg(float* x, int size){
    return 0;
}

float valOfExpectation(float* x,int size){
    float sum = 0;   // need to check if the defult is ok
    for (int i = 0; i < size; i++){
        sum += x[i];
    }
  
    return sum / size; 
}

float var(float* x, int size){
    float sum = 0;
    for (int i = 0; i < size; i++){
        sum += std::pow(x[i] - valOfExpectation(x , size),2);

    }
    
    return sum / size;
}

float cov(float* x, float* y, int size){
    float multy = 0;
    float xMid = valOfExpectation(x ,size);
    float yMid = valOfExpectation(y , size);
    for (int i = 0; i < size; i++){
        multy += (x[i] - xMid) * (y[i] - yMid);
    }

    return multy / size;
}

float pearson(float* x, float* y, int size){
    
    return cov(x,y,size) / std::sqrt(var(x,size)*var(y,size));   
}

Line linear_reg(Point** points,int size){
    float arrX[3000] , arrY[3000];
    for(int i =0;i < size; i++){
        arrX[i] = points[i]->x;
        arrY[i] = points[i]->y;
    }

    float a = cov(arrX,arrY,size) / var(arrX,size);

    float xMid = valOfExpectation(arrX , size);
    float yMid = valOfExpectation(arrY , size);
    
    float b = yMid - a * xMid;

    return Line(a , b);
}

float dev(Point p,Point** points,int size){
    Line line = linear_reg(points,size);
    
    return dev(p , line);
}

float dev(Point p, Line l){

    return std::abs(l.f(p.x)-p.y);
}


