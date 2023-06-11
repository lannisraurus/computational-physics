#include <iostream>
#include "FCMatrixAlgo.h"
#include "EqSolver.h"
#include <Eigen/Dense>

using namespace Eigen;

int main(){

	// battery (V)
	double V0 = 10;

	// resistances (Ohm)
	double r1 = 30;
	double r2 = 50;
	double r3 = 5;
	double r4 = 10;
	double r5 = 15;
	double r6 = 20;
	double r7 = 25;
	double r8 = 55;

	// linear system
	Matrix<double,Dynamic,Dynamic> system(4,4);
	Matrix<double,Dynamic,Dynamic> vec(4,1);
	system <<
	r1+r2,          -r1,           -r2,        0,
	  -r1,  r1+r3+r6+r4,           -r4,      -r6,
	  -r2,          -r4,   r5+r2+r4+r7,      -r7,
	    0,          -r6,           -r7, r7+r6+r8;
	vec << V0, 0, 0, 0;

    // Solve
    cout << "I1, I2, I3, I4\n";

	// Solve - deterministic methods
	Matrix<double,Dynamic,1> sol(4,1);
	EqSolver solver(system,vec);
	cout << "Gauss solver with no pivot:\n" << solver.GaussSolver(false).transpose() << "\n";	
	cout << "Gauss solver with pivot:\n" << solver.GaussSolver(true).transpose() << "\n";
	cout << "LU Decomp solver with no pivot:\n" << solver.LUSolver(false).transpose() << "\n";
	cout << "LU Decomp solver with pivot:\n" << solver.LUSolver(true).transpose() << "\n";

	// Solve - iterative methods
	int maxIters = 1000;
	solver.IterativeJacobiSolver(sol,maxIters);
	cout << "Jacobi method:\n" << sol.transpose() << "\n";
	solver.IterativeGaussSeidelSolver(sol,maxIters);
	cout << "Gauss Sneidel method:\n" << sol.transpose() << "\n";

}
