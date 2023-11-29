#include <eigen3/Eigen/Dense>
#include <iostream>

#include "reader.h"
#include "simplex.h"

int main (void)
{   

    Simplex s = Simplex();
    s = s.solve();

    return 0;
}