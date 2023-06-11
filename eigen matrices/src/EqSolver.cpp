#include "EqSolver.h"

// OUTPUT
ostream& operator<<(ostream& o, const EqSolver& e){
    o << "--------\nSOLUTION\n--------\n" << e.solution.transpose() << "\n--------\n";
    return o;
}


// CLASS FUNCTIONS

EqSolver::EqSolver(const Matrix<double,Dynamic,Dynamic>& M, const Matrix<double,Dynamic,1>& b)
: matrix(M),
vector(b),
solution(Matrix<double,Dynamic,1>(M.rows(),1))
{}

const Matrix<double,Dynamic,1>& EqSolver::GaussSolver(bool pivot){

    // needed variables (preserves original matrices and vectors)
    Matrix<double,Dynamic,Dynamic> m(matrix);
    Matrix<double,Dynamic,1> v(vector);

    // use different gauss elimination algo.
    if(pivot) {
        Matrix<int,Dynamic,1> order(m.rows());
        for(int i = 0; i < order.rows(); i++) order(i,0) = i;
        algo.GaussEliminationPivot(m,v,order);
    }else{
        algo.GaussElimination(m,v);
    }
    
    // solve :)
    for(int i = m.rows()-1; i >= 0; i--){
        double sum = 0;
        for(int j = m.rows()-1; j > i; j--){
            sum += m(i,j)*solution(j,0);
        }
        solution(i,0) = (v(i,0) - sum)/m(i,i);
    }

    // return
    return solution;
}


const Matrix<double,Dynamic,1>& EqSolver::LUSolver(bool pivot){

    // needed variables (preserves original matrices and vectors)
    Matrix<double,Dynamic,Dynamic> m(matrix); 
    Matrix<double,Dynamic,Dynamic> v(vector);
    Matrix<int,Dynamic,1> order(m.rows());
    Matrix<double,Dynamic,1> Ux(vector.rows()); // intermidiary for solving the system

    // Do decomposition
    algo.LUdecomposition(m,order,pivot);

    // Create lower matrix
    Matrix<double,Dynamic,Dynamic> L(matrix.rows(),matrix.cols());
    L.setIdentity();
    for (int i = 1; i < L.rows(); i++) {
        for (int j = 0; j < i; j++) {
            L(i,j) = m(i,j);
        }
    }

    // Create upper matrix
    Matrix<double,Dynamic,Dynamic> U(matrix.rows(),matrix.cols());
    U.setZero();
    for (int j = 0; j < L.cols(); j++) {
        for (int i = 0; i <= j; i++) {
            U(i,j) = m(i,j);
        }
    }

    // solve step 1
    for(int i = 0; i < m.rows(); i++){
        double sum = 0;
        for(int j = 0; j < i; j++){
            sum += L(i,j)*Ux(j,0);
        }
        Ux(i,0) = v(i,0) - sum;
    }    
    // solve step 2
    for(int i = U.rows()-1; i >= 0; i--){
        double sum = 0;
        for(int j = U.rows()-1; j > i; j--){
            sum += U(i,j)*solution(j,0);
        }
        solution(i,0) = (Ux(i,0) - sum)/U(i,i);
    }

    //return
    return solution;
}



void EqSolver::IterativeJacobiSolver(Matrix<double,Dynamic,1>& solution, int& maxIter, double tolerance){

    // needed variables
    bool running = true;
    int iter = 0;
    Matrix<double,Dynamic,1> next(vector.rows(),1);
    Matrix<double,Dynamic,1> prev(vector.rows(),1);

    // initialize the aux vector - we choose a vector that is all ones.
    for (int i = 0; i < next.rows(); i++) next(i,0) = 1;
    
    // iterations - exit if below tolerance OR max iterations reached
    while(running && iter<maxIter){
        
        // save previous 
        prev = next;

        // calculate next iteration
        for (int i=0; i<matrix.rows(); i++) {
            double sum = 0. ;
            for (int j=0; j<matrix.cols(); j++) if (i != j) sum += matrix(i,j)*prev(j,0);
            next(i,0) = (vector(i,0)-sum)/matrix(i,i);
        }

        
        // check if the answer is below the tolerance
        if ( (next-prev).cwiseAbs().maxCoeff() < tolerance ) running = false;
        

        // Increase iterations
        iter++;
    }
    solution = next;
}


void EqSolver::IterativeGaussSeidelSolver(Matrix<double,Dynamic,1>& solution, int& maxIter, double tolerance){

    // needed variables
    bool running = true;
    int iter = 0;
    Matrix<double,Dynamic,1> next(vector.rows(),1);
    Matrix<double,Dynamic,1> prev(vector.rows(),1);

    // initialize the aux vector - we choose a vector that is all ones.
    for (int i = 0; i < next.rows(); i++) next(i,0) = 1;
    
    // iterations - exit if below tolerance OR max iterations reached
    while(running && iter<maxIter){
        
        // save previous 
        prev = next;

        // calculate next iteration
        for (int i=0; i<matrix.rows(); i++) {
            double sum = 0. ;
            for (int j=0; j<matrix.cols(); j++) if (i != j) sum += matrix(i,j)*next(j,0);
            next(i,0) = (vector(i,0)-sum)/matrix(i,i);
        }

        
        // check if the answer is below the tolerance
        if ( (next-prev).cwiseAbs().maxCoeff() < tolerance ) running = false;
        

        // Increase iterations
        iter++;
    }
    solution = next;
}