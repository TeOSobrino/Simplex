#include <eigen3/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "simplex.h"

Simplex Simplex::Reader::ReadMatrix(const std::string path)
{   
    Simplex s = Simplex();
    file_d.open(path, std::ios_base::in);
    int rows, cols; // restrictions, variables
    
    if (file_d.is_open()) {
        file_d >> rows >> cols;
    } else {
        std::cout << "Invalid path to file" << std::endl;
        exit(2);
    }

    std::vector<float> matrix_entries; // holds the integers to become matrix
    std::vector<float> cost_fnt; // holds the integers to become matrix

    ReadLineFiletoVec(cost_fnt);
    ReadLineFiletoVec(cost_fnt);
    for (int i = 0; i <= rows+1; i++) { //reads the cost and then n rows
        ReadLineFiletoVec(matrix_entries);
    }

    MatrixXf T = Eigen::Map<Matrix<float, Dynamic, Dynamic, Eigen::RowMajor>>(
        matrix_entries.data(), rows, cols+1); // converts array to row matrix
    
    MatrixXf cst = Eigen::Map<Matrix<float, Dynamic, Dynamic, Eigen::RowMajor>>(
        cost_fnt.data(), 1, cols); // converts array to row matrix
    s.c = cst.row(0);
    s.b = T.col(cols);
    s.A = T.block(0,0, rows, cols);

    s.restrictions = rows;
    s.variables = cols;
    
    return s;
}

void Simplex::Reader::ReadLineFiletoVec(std::vector<float> &vec) 
{
    std::string matrix_row; // current line from file
    std::string matrix_entry; // parsed integer from matrix_row
    getline(file_d, matrix_row);

    std::stringstream matrixRowStringStream(matrix_row);
    while (getline(matrixRowStringStream, matrix_entry, ' ')) 
    // parses int separated by space
    {
        vec.push_back(stoi(matrix_entry)); // adds int to vector
    }
}