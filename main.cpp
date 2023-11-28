#include <eigen-3.4.0/Eigen/Dense>
#include <iostream>

#include "reader.h"
#include "simplex.h"

int main (void)
{   
    std::string path;
    std::cout << "Insert path to file" << std::endl;

    Simplex s = Simplex();
    s = s.solve();


    return 0;
}