#include "FCMatrixAlgo.h"

void FCMatrixAlgo::GaussElimination(Matrix<double,Dynamic,Dynamic> &matrix, Matrix<double,Dynamic,1> &vector){

	// needed variables
	int iter = 0;
	int lastRowIndex = matrix.rows()-1;
	
	// iterate over all rows (last one doesn't need to be iterated)
	while(iter <= lastRowIndex-1){

		// check for weird collumns (all zeros)
		if ( matrix(seq(iter,lastRowIndex),iter).isZero(0) ) {
			iter++;
			continue;
		}

		// find a fitting row - swaps row with non zero row (in the iter position)
		int j = 0;
		while(matrix(iter,iter) == 0){
			if(matrix(lastRowIndex-j,iter) != 0){
				matrix.row(iter).swap(matrix.row(lastRowIndex-j));
				vector.row(iter).swap(vector.row(lastRowIndex-j));
			}else j++;
		}

		// eliminatation step number *iter* out of rows-2
		for(int i = iter+1; i <= lastRowIndex; i++){
			double coeff = matrix(i,iter)/matrix(iter,iter);
			matrix.row(i) = matrix.row(i) - coeff*matrix.row(iter);
			vector.row(i) = vector.row(i) - coeff*vector.row(iter); 
		}

		// go to next row
		iter++;

	}
}


void FCMatrixAlgo::GaussEliminationPivot(Matrix<double,Dynamic,Dynamic> &matrix, Matrix<double,Dynamic,1> &vector, Matrix<int,Dynamic,1> &order){

	// needed variables and initialization
	int iter = 0;
	int lastRowIndex = matrix.rows()-1;
	Matrix<double,Dynamic,1> maxValues;

	// Fill the order vector with the max coefficient per row.
	maxValues = matrix.cwiseAbs().rowwise().maxCoeff();
	
	// iterate over all rows (last one doesn't need to be iterated)
	while(iter <= lastRowIndex-1){

		// check for weird collumns (all zeros)
		if ( matrix(seq(iter,last),iter).isZero(0) ) {
			iter++;
			continue;
		}

		// pivoting
		Array<double,Dynamic,1> weights = matrix(seq(iter,lastRowIndex),iter).cwiseAbs().array()/maxValues(seq(iter,lastRowIndex),0).array();
		double max = weights[0];
		int entry = iter;
		for(int i = 1; i < weights.size(); i++){
			if (weights[i]>max){
				max = weights[i];
				entry = iter+i;
			}
		}
		matrix.row(iter).swap(matrix.row(entry));
		vector.row(iter).swap(vector.row(entry));
		maxValues.row(iter).swap(maxValues.row(entry));
		order.row(iter).swap(order.row(entry));

		
		//eliminatation step number *iter* out of rows-2
		for(int i = iter+1; i <= lastRowIndex; i++){
			double coeff = matrix(i,iter)/matrix(iter,iter);
			matrix.row(i) = matrix.row(i) - coeff*matrix.row(iter);
			vector.row(i) = vector.row(i) - coeff*vector.row(iter);
		}
		

		// go to next row
		iter++;

	}
}


void FCMatrixAlgo::LUdecomposition(Matrix<double,Dynamic,Dynamic> &matrix, Matrix<int,Dynamic,1> &order, bool bpivot){

	// needed variables and initialization
	int iter = 0;
	int lastRowIndex = matrix.rows()-1;
	Matrix<double,Dynamic,Dynamic> result(matrix.rows(),matrix.cols());
	result.setZero();
	Matrix<double,Dynamic,1> maxValues;

	// Fill the order vector with the max coefficient per row.
	maxValues = matrix.cwiseAbs().rowwise().maxCoeff();
	
	

	// iterate over all rows (last one doesn't need to be iterated)
	while(iter <= lastRowIndex-1){
		
		

		// check for weird collumns (all zeros)
		if ( matrix(seq(iter,last),iter).isZero(0) ) {
			iter++;
			continue;
		}

		// pivoting / row swapping (depends on the algorithm)
		if (bpivot){ // Pivoting enabled
			Array<double,Dynamic,1> weights = matrix(seq(iter,lastRowIndex),iter).cwiseAbs().array()/maxValues(seq(iter,lastRowIndex),0).array();
			double max = weights[0];
			int entry = iter;
			for(int i = 1; i < weights.size(); i++){
				if (weights[i]>max){
					max = weights[i];
					entry = iter+i;
				}
			}
		matrix.row(iter).swap(matrix.row(entry));
		maxValues.row(iter).swap(maxValues.row(entry));
		order.row(iter).swap(order.row(entry));
		} else { // Pivoting disabled
			int j = 0;
			while(matrix(iter,iter) == 0){
				if(matrix(lastRowIndex-j,iter) != 0){
					matrix.row(iter).swap(matrix.row(lastRowIndex-j));
					order.row(iter).swap(order.row(lastRowIndex-j));
				}else j++;
			}
		}
		
		//eliminatation step number *iter* out of rows-2
		for(int i = iter+1; i <= lastRowIndex; i++){
			double coeff = matrix(i,iter)/matrix(iter,iter);
			matrix.row(i) = matrix.row(i) - coeff*matrix.row(iter);
			result(i,iter) = coeff;
		}
		
		

		// go to next row
		iter++;

		

	}
	
	
	for (int i = 1; i <= lastRowIndex; i++){
		for (int j = 0; j < i; j++){
			matrix(i,j) = result(i,j);
		}
	}
	
	
}