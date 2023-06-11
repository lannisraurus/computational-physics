#ifndef __EQSOLVER__
#define __EQSOLVER__

#include <Eigen/Dense>
#include "FCMatrixAlgo.h"
#include <iostream>

using namespace Eigen;
using namespace std;

/*
This class uses FCMatrixAlgo to solve linear systems of equations
*/
class EqSolver{
private:
    // Gauss elimintation and LU decomposition.
    FCMatrixAlgo algo;
    // Your matrix. [M]x=b
    Matrix<double,Dynamic,Dynamic> matrix;
    // Your vector. Mx=[b]
    Matrix<double,Dynamic,1> vector;
    // Your solution
    Matrix<double,Dynamic,1> solution;

public:

    // Constructors
    ~EqSolver() = default;
    EqSolver() = default;
    EqSolver(const Matrix<double,Dynamic,Dynamic>& M, const Matrix<double,Dynamic,1>& b);

    // output
    friend ostream& operator<<(ostream&, const EqSolver&);

    // deterministic solvers
    const Matrix<double,Dynamic,1>& GaussSolver(bool pivot=false);
    const Matrix<double,Dynamic,1>& LUSolver(bool pivot=false);

    // Iterative solvers
    // NOTE - In wikipedia the names are reversed in respect to the professor's notes. We went by the professor's indications.
    void IterativeJacobiSolver(Matrix<double,Dynamic,1>& solution, int& maxIter, double tolerance=1.E-3);
    void IterativeGaussSeidelSolver(Matrix<double,Dynamic,1>& solution, int& maxIter, double tolerance=1.E-3);

};

// friend functions for output
ostream& operator<<(ostream&, const EqSolver&);


#endif