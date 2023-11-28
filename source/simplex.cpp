#include <eigen-3.4.0/Eigen/Core>
#include <eigen-3.4.0/Eigen/Dense>
#include <iostream>

#include "simplex.h"

// returns the first negative index or -1 if all are positive
int find_var_add_base(ArrayXf a)
{
    int j = INT32_MAX;
    int k = 0;
    for (int i = 1; i < a.size(); i++) {
        if (a[i] < 0)
            return i;
    }

    return -1;
}

// removes the smallest quocient between cost[i]/added_var[i]
int find_var_remove_base(MatrixXf A, int in)
{   
    float min = INT32_MAX;
    int idx = -1;
    for(int i = 1; i < A.col(0).size(); i++){
        if(A.coeff(i, in) <= 0) continue;
        float d = A.coeff(i, 0)/A.coeff(i, in); 
        if(d < min){
            min = d;
            idx = i;
        }
    }

    return idx;
}

ArrayXf ReducedCost(MatrixXf ABi, ArrayXf c)
{
    ArrayXf cr(c.size());
    for (int i = 0; i < c.size(); i++) {
        cr[i] = 2 * ABi.coeff(0, i) - ABi.col(i).sum(); // computing x
    }

    // just for display
    Eigen::Array<float, 1, Dynamic, Eigen::RowMajor> ct = cr;

    return cr;
}

void insert_in_base(MatrixXf& A, ArrayXf& X, int idx_enter_base,
                    int idx_remove_base)
{
    if(idx_enter_base == -1 || idx_remove_base == -1) return;
    X[idx_remove_base-1] = idx_enter_base;

    float pivot = A.coeff(idx_remove_base, idx_enter_base);

    std::cout << "pivot = " << pivot << "\n";

    for(int i = 0; i < A.col(0).size(); i++){
        if(i == idx_remove_base) continue;
        float k = A.coeff(i, idx_enter_base);
        float coef = -(float)k/(float)pivot;
        A.row(i) += coef*A.row(idx_remove_base);
    }

    A.row(idx_remove_base) *= 1.0f/(float)pivot;
}

ArrayXf remove_item(ArrayXf& a, int posToRemove)
{
    float v[a.size()];

    int j = 0;
    for(int i = 0; i < a.size(); i++){
        if(i == posToRemove) continue;
        v[j] = a[i];
        j++;
    }

    Eigen::Map<ArrayXf> b(v, a.size()-1);
    a = b;

    return b;
}


void remove_row(Eigen::MatrixXf& matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows()-1;
    unsigned int numCols = matrix.cols();

    if( rowToRemove < numRows )
        matrix.block(rowToRemove,0,numRows-rowToRemove,numCols) = matrix.block(rowToRemove+1,0,numRows-rowToRemove,numCols);

    matrix.conservativeResize(numRows,numCols);
}

Simplex::Simplex(){

};

Simplex Simplex::solve()
{

    Reader r = Reader();

    Simplex s = r.ReadMatrix("/home/teo/Documents/4s/trablin/orig/t.txt");
    std::cout << "restrictions:\n"
              << s.A << "\ncosts:\n"
              << s.c << "\nb:\n"
              << s.b << "\n\n";

    s.SolveFirstPhase();
    s.SolveSecondPhase();
    std::cout<<"X =" << s.X.transpose() << "\n";
    std::cout<<"Last tableau:\n" << s.A <<"\n\n";

    std::cout<<"X = (";
    for(int i = 0; i < s.variables; i++){
        auto pos = std::find(s.X.begin(), s.X.end(), i+1);
        if(pos != s.X.end()){
            int p = pos - s.X.begin();
            std::cout << s.A.coeff(p+1, 0);
        } else {
            std::cout << "0"; 
        }
        if(i+1 < s.variables) std::cout<<", ";
    }
    std::cout<<")\n";

    std::cout<<"f(X) = " << -s.A.coeff(0, 0) << "\n";

    return s;
}

ArrayXf Simplex::SolveTableau(MatrixXf &T, ArrayXf &b, ArrayXf &c, ArrayXf &X)
{
    //calculates reduced cost based on cost array
    ArrayXXf cr = ReducedCost(T, c);
    
    // update reduced cost in the tableau
    T.topRows(1) = cr.transpose();

    int idx_enter_base = 0;

    while (idx_enter_base != -1){

        // apply bland's rule
        idx_enter_base = find_var_add_base(T.row(0));
    
        std::cout << "Current Tableau:\n"
                  << T << "\nCurrent Base: " << X.transpose() << "\n";
        int idx_remove_base = find_var_remove_base(T, idx_enter_base);

        std::cout << "insert: " << idx_enter_base
                  << "\nremove: " << idx_remove_base << "\n";

        insert_in_base(T, X, idx_enter_base, idx_remove_base);
    }

    A = T;
    return X;
}

ArrayXXf Simplex::SolveFirstPhase()
{

    // adapt the problem to the first phase

    // change cost function
    Eigen::Array<float, Dynamic, Eigen::RowMajor> cst(variables + restrictions +
                                                    1);
    cst = ArrayXf::Zero(cst.size());
    cst.tail(restrictions) = 1;

    // add the artificail variables
    MatrixXf m(restrictions, variables + restrictions + 1);
    m.topLeftCorner(restrictions, 1) = b; // add costs at first column
    m.middleCols(1, variables) = A;       // add the AB^-1 matrix (B = Identity)
    m.bottomRightCorner(restrictions, restrictions) = MatrixXf::Identity(
        restrictions, restrictions); // artificial variables added

    MatrixXf M(restrictions + 1, variables + restrictions + 1);
    M.topRows(1) = cst.transpose();
    M.block(1, 0, restrictions, restrictions + variables + 1) = m;

    std::cout << "initial tableau:\n" << M << "\n";

    // get the initial base (artifical variables for the first phase)
    float data[restrictions];
    for (int i = variables; i < variables + restrictions; i++) {
        data[i - variables] = i + 1;
    }
    Eigen::Map<ArrayXf> base(data, restrictions);
    ArrayXf bb = base;

    X = SolveTableau(M, b, cst, bb);

    MatrixXf updated_tableau(restrictions+1, variables+1);
    updated_tableau = M.block(0, 0, variables+1, restrictions+1);

    // remove_non_basics(updated_tableau, X);
    for(int i = 1; i <= X.size(); i++){
        if(X[i-1] > variables){
            remove_row(updated_tableau, i);
            X = remove_item(X, i-1);
        }
    }
    A = updated_tableau;

    return ArrayXXf();
}

ArrayXXf Simplex::SolveSecondPhase() 
{

    ArrayXf cst = ArrayXf::Zero(variables+1);
    cst.tail(variables) = c;

    A.topRows(1) = cst.transpose();
    X = SolveTableau(A, b, cst, X);

    return X;  
}