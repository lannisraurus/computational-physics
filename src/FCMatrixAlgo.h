#ifndef __FCMATRIXALGO__
#define __FCMATRIXALGO__

#include <Eigen/Dense>
#include <iostream>
#include <limits>

using namespace Eigen;
using namespace std;

/*
This class implements different algorithms for matrices - gauss elimination and LU decomposition
*/
class FCMatrixAlgo{
public:
    // Constructors
    FCMatrixAlgo() = default;
    ~FCMatrixAlgo() = default;

    // Mx=b <- solves this system!
    static void GaussElimination(Matrix<double,Dynamic,Dynamic> &matrix, Matrix<double,Dynamic,1> &vector);
    // Another method of doing Mx=b
    static void GaussEliminationPivot(Matrix<double,Dynamic,Dynamic> &matrix, Matrix<double,Dynamic,1> &vector, Matrix<int,Dynamic,1> &order);
    // decomposes matrix into two matrices (lower and upper matrices). matrix will store all info about the lower and upper matrices
    static void LUdecomposition(Matrix<double,Dynamic,Dynamic> &matrix, Matrix<int,Dynamic,1> &order, bool bpivot=false);

};

#endif