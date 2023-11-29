#pragma once
#include <eigen3/Eigen/Dense>
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
    MatrixXf T;   // tableau
    MatrixXf A;   // restrictions
    MatrixXf B;   // base
    ArrayXf b;   // restriction costs
    ArrayXf c;   // objective function
    ArrayXf X;   // variables in the base (basic variables)
    ArrayXf c_r; // reduced costs
    int iter_num;
    int f_iter_n;
    int s_iter_n;

    int restrictions;
    int variables;

    ArrayXf SolveTableau(MatrixXf& A, ArrayXf& c, ArrayXf& X);
    MatrixXf BuildTableau(ArrayXf cst);
    void UpdateTableau();


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
