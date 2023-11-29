
# Simplex

Group Members: Felipe Destaole,  João Pedro Farjoun,
  Luis Roberto Piva,  Téo Sobrino

## Overview

Simplex method implementation using two phases and Bland's rule in C++ for
simple problems, as final assignment of my Linear Optimizitaion class.

This code uses [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)
Library, an open source light-weight library for Linear Algebra.

## Prerequisites

To install eigen library in Ubuntu based distros:

```bash
sudo apt update
sudo apt install libeigen3-dev
```

Or download the
[zip](https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip)
file.

## Running the Program

Running this program should be easy enough with

```bash
make all
make run
```

When the program starts it will prompt the user for a filepath, that should
contain a linear programming problem descrpited in a file with a specific format

## Input File

The input file should be in the following format:

\#restrictions &nbsp; &nbsp; &nbsp; #variables

c<sub>1</sub>  c<sub>2</sub>  ... c<sub>n</sub>  

A<sub>11</sub>  A<sub>12</sub>  ... A<sub>1n</sub>  b<sub>1</sub>  
A<sub>21</sub>  A<sub>22</sub>  ... A<sub>2n</sub>  b<sub>2</sub>

.
.
.

A<sub>m1</sub>  A<sub>m2</sub>  ... A<sub>mn</sub>  b<sub>m</sub>

Note that m = \#restrictions and n = \#variables.

c<sub>i</sub> denotes the i<sup>th</sup> coefficient in the cost array.

Each line denotes the coefficients of a restriction (A), and the value
associated (b).
