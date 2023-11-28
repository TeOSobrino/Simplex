#include <eigen-3.4.0/Eigen/Core>
#include <eigen-3.4.0/Eigen/Dense>
#include <iostream>

#include "simplex.h"

// returns the first negative index or -1 if all are positive
inline int find_var_add_base(ArrayXf a)
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
inline int find_var_remove_base(MatrixXf T, int in)
{
    float min = INT32_MAX;
    int idx = -1;
    for (int i = 1; i < T.col(0).size(); i++) {
        if (T.coeff(i, in) <= 0)
            continue;
        float d = T.coeff(i, 0) / T.coeff(i, in);
        if (d < min) {
            min = d;
            idx = i;
        }
    }

    return idx;
}

// calculates the reduced cost for the tableau
ArrayXf ReducedCost(MatrixXf ABi, ArrayXf c, ArrayXf X)
{
    ArrayXf cr(c.size()); // reduced cost array
    Eigen::Vector<float, Dynamic> c_b(X.size());

    for (int i = 0; i < X.size(); i++) {
        c_b[i] = c[X[i]]; // take each basic variable cost
    }

    for (int i = 1; i < cr.size(); i++) {

        Eigen::VectorXf p_col = ABi.block(
            1, i, c_b.size(), 1); // get variables value in tableau column
        cr[i] = c[i] -
                p_col.dot(c_b); // dot product between variables and basic cost
    }

    cr[0] = 0;
    for (int i = 0; i < X.size(); i++) {
        cr[0] -= c[X[i]] * ABi.coeff(i + 1, 0); // calculate objective fnt cost
    }

    return cr;
}

// inserts a variable in the base (zeros in entire column except in inserted
// row)
inline void insert_in_base(MatrixXf &T, ArrayXf &X, int idx_enter_base,
                           int idx_remove_base)
{
    if (idx_enter_base == -1 || idx_remove_base == -1)
        return;
    X[idx_remove_base - 1] = idx_enter_base;

    float pivot = T.coeff(idx_remove_base, idx_enter_base);

    for (int i = 0; i < T.col(0).size(); i++) {
        if (i == idx_remove_base)
            continue;
        float k = T.coeff(i, idx_enter_base);
        float coef = -(float)k / (float)pivot;
        T.row(i) += coef * T.row(idx_remove_base);
    }

    T.row(idx_remove_base) *= 1.0f / (float)pivot;
}

ArrayXf remove_item(ArrayXf &a, int posToRemove)
{
    float v[a.size()];

    int j = 0;
    for (int i = 0; i < a.size(); i++) {
        if (i == posToRemove)
            continue;
        v[j] = a[i];
        j++;
    }

    Eigen::Map<ArrayXf> b(v, a.size() - 1);
    a = b;

    return b;
}

void remove_row(Eigen::MatrixXf &matrix, unsigned int rowToRemove)
{
    unsigned int numRows = matrix.rows() - 1;
    unsigned int numCols = matrix.cols();

    if (rowToRemove < numRows)
        matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) =
            matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);

    matrix.conservativeResize(numRows, numCols);
}

Simplex::Simplex():
    iter_num{0}
{
};

Simplex Simplex::solve()
{

    Reader r = Reader();
    std::string path;
    std::cout << "Insert path to file" << std::endl;
    std:: cin >> path;
    Simplex s = r.ReadMatrix(path);

    s.SolveFirstPhase();
    s.SolveSecondPhase();

    // print solution
    std::cout << "X = (";
    for (int i = 0; i < s.variables; i++) {
        auto pos = std::find(s.X.begin(), s.X.end(), i + 1);
        if (pos != s.X.end()) {
            int p = pos - s.X.begin();
            std::cout << s.T.coeff(p + 1, 0);
        } else {
            std::cout << "0";
        }
        if (i + 1 < s.variables)
            std::cout << ", ";
    }
    std::cout << ")\n";
    std::cout << "f(X) = " << -s.T.coeff(0, 0) << "\n";
    std::cout << "Total number of iterations: " << s.iter_num << "\n";


    return s;
}

ArrayXf Simplex::SolveTableau(MatrixXf &T, ArrayXf &b, ArrayXf &c, ArrayXf &X)
{
    // calculates reduced cost based on cost array
    ArrayXXf cr = ReducedCost(T, c, X);

    // update reduced cost in the tableau
    T.topRows(1) = cr.transpose();

    int idx_enter_base = 0;
    int idx_remove_base = 0;
    while (idx_enter_base != -1) {
        iter_num++;
        // apply bland's rule
        idx_enter_base = find_var_add_base(T.row(0));
        idx_remove_base = find_var_remove_base(T, idx_enter_base);

        // if there is a negative reduced cost and no available variable to exit
        // the base, the problem is unbounded
        if (idx_remove_base == -1 && idx_enter_base != -1) {
            throw std::domain_error(
                "Invalid restrictions (unbounded problem)\n");
            exit(1);
        }
        insert_in_base(T, X, idx_enter_base, idx_remove_base);
    }

    return X;
}

ArrayXXf Simplex::SolveFirstPhase()
{

    // adapt the problem to the first phase

    // change cost function
    ArrayXf cst(variables + restrictions + 1);

    // cost function is 1 in artificial variables and 0 elsewhere
    cst = ArrayXf::Zero(cst.size());
    cst.tail(restrictions) = 1;

    // build the tableau
    T = BuildTableau(cst);
    
    // get the initial base (artifical variables for the first phase)
    float data[restrictions];
    for (int i = variables; i < variables + restrictions; i++) {
        data[i - variables] = i + 1;
    }
    Eigen::Map<ArrayXf> base(data, restrictions);
    ArrayXf bb = base;

    X = SolveTableau(T, b, cst, bb);
    UpdateTableau();

    return ArrayXXf();
}

ArrayXXf Simplex::SolveSecondPhase()
{
    // update cost function to recalculate reduced costs
    ArrayXf cst = ArrayXf::Zero(variables + 1);
    cst.tail(variables) = c;
    X = SolveTableau(T, b, cst, X);

    return X;
}

MatrixXf Simplex::BuildTableau(ArrayXf cst)
{
    // create a matrix to fit B^⁻1A and the cost
    MatrixXf m(restrictions, variables + restrictions + 1);
    m.topLeftCorner(restrictions, 1) = b; // add costs at first column
    m.middleCols(1, variables) = A;       // add the B^-1A matrix (B = Identity)
    m.bottomRightCorner(restrictions, restrictions) = MatrixXf::Identity(
        restrictions, restrictions); // add artificial variables to last cols

    // create a matrix to fit the tableau
    MatrixXf M(restrictions + 1, variables + restrictions + 1);
    M.topRows(1) = cst.transpose(); // add reduced cost row
    M.block(1, 0, restrictions, restrictions + variables + 1) = m; // add m

    return M;
}

void Simplex::UpdateTableau() 
{
    MatrixXf updated_tableau(restrictions + 1, variables + 1);
    updated_tableau = T.block(0, 0, restrictions + 1, variables + 1);

    // remove artifical variables from last columns and from base (if any)
    for (int i = 1; i <= X.size(); i++) {
        if (X[i - 1] > variables) {
            remove_row(updated_tableau, i);
            X = remove_item(X, i - 1);
        }
    }

    // checks if the solution is feasible
    if (updated_tableau.col(0).size() <= restrictions) {
        throw std::domain_error("Invalid restrictions (infeasible solution)\n");
        exit(1);
    }
    
    T = updated_tableau;
}
