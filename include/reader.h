#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <eigen-3.4.0/Eigen/Dense>

using Eigen::MatrixXi; //integer matrix with dynamic dimensions
using Eigen::Matrix;
using Eigen::Dynamic; 


class reader{
private:
    std::fstream file_d;

public:
    MatrixXi readMatrix(const std::string path);
};