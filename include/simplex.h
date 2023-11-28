#pragma once
#include <eigen-3.4.0/Eigen/Dense>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "reader.h"

using Eigen::ArrayXf;
using Eigen::ArrayXXf;
using Eigen::Dynamic;
using Eigen::Matrix;
using Eigen::MatrixXf; // integer matrix with dynamic dimensions

class Simplex
{
  private:
    MatrixXf A;   // restrictions
    MatrixXf B;   // base
    ArrayXf b;   // restriction costs
    ArrayXf c;   // objective function
    ArrayXf X;   // variables in the base (basic variables)
    ArrayXf c_r; // reduced costs

    int restrictions;
    int variables;

    ArrayXf SolveTableau(MatrixXf& A, ArrayXf& b, ArrayXf& c, ArrayXf& X);

  public:
    Simplex();
    Simplex solve();
    ArrayXXf SolveFirstPhase();
    ArrayXXf SolveSecondPhase();
    class Reader
    {
      private:
        std::fstream file_d;

      public:
        Simplex ReadMatrix(const std::string path);
        void ReadLineFiletoVec(std::vector<float> &vec);
    };
};
