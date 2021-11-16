#ifndef ANOMALYALG_H
#define ANOMALYALG_H

#include <iostream>
#include <string>
#include <vector>
#include "HybridAnomalyDetector.h"

std::vector<AnomalyReport> detectForCircle( std::string data, std::string test );
std::vector<AnomalyReport> detectForLinearReg( std::string data, std::string test );

#endif