#include <eigen-3.4.0/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "reader.h"

MatrixXi reader::readMatrix(const std::string path)
{
    file_d.open(path, std::ios_base::in);
    int rows, cols;
    if (file_d.is_open()) {
        file_d >> rows >> cols;
    } else {
        std::cout << "Invalid path to file" << std::endl;
        return MatrixXi();
    }

    std::cout << rows << " " << cols << std::endl;

    std::vector<int> matrix_entries; // holds the integers to become matrix
    std::string matrix_row; // current line from file
    std::string matrix_entry; // parsed integer from matrix_row

    for (int i = 0; i <= rows; i++) {
        getline(file_d, matrix_row);
        std::stringstream matrixRowStringStream(matrix_row);

        while (getline(matrixRowStringStream, matrix_entry, ' ')) 
        // parses int separated by space
        {
            matrix_entries.push_back(stoi(matrix_entry)); // adds int to vector
        }
    }

    return Eigen::Map<Matrix<int, Dynamic, Dynamic, Eigen::RowMajor>>(
        matrix_entries.data(), rows, cols); // converts array to row matrix
}