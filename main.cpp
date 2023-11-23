#include <eigen-3.4.0/Eigen/Dense>
#include <iostream>

#include "reader.h"

int main (void)
{   
    std::string path;
    std::cout << "Insert path to file" << std::endl;


    reader r = reader();
    MatrixXi m = r.readMatrix("/home/teo/Documents/4s/trablin/orig/t.txt");

    std::cout << m;

    return 0;
}