#include <vector>
#include <iostream>
#include <math.h>

template<class type> class matrix{



private:



    //////////////////////////////////////////////////// MATRIX DATA ////////////////////////////////////////////////////

    std::vector<std::vector<type>> data;
    int numRows, numCollumns;



    //////////////////////////////////////////////////// EXCEPTIONS ////////////////////////////////////////////////////

    //check if object is a matrix //update size function
    void checkAndSetSize(){
        //Intializes number of collumns and rows
        numCollumns=data[0].size();
        numRows=data.size();

        //Checks if all are equal
        for(int i = 0; i < numRows; i++){
            if (data[i].size()!=numCollumns) buildError();
            numCollumns = data[i].size();
        }
    }
    //throw exception when building the matrix
    void buildError(){
        throw std::invalid_argument("Object is not a matrix - uneven number of collumns per row");
    }
    //throw exception when building the matrix
    void operationError(){
        throw std::invalid_argument("Matrix Operation is forbidden");
    }
    //throw exception if matrix isnt square - if statement inside
    void squareError(){
        if(numCollumns!=numRows) throw std::invalid_argument("Matrix is not square");
    }



public:



    //////////////////////////////////////////////////// BASIC OPERATIONS ////////////////////////////////////////////////////

    //constructor - generates matrix object
    matrix(std::vector<std::vector<type>> newMatrix){
        data = newMatrix;
        checkAndSetSize();
    }
    //representation
    void representation(){
        std::cout << '\n';
        for(int i = 0; i < numRows; i++){
            std::cout << "{ ";
            for(int j = 0; j < numCollumns; j++){
                std::cout << data[i][j] << " ";
            } std::cout << "}\n";
        }
        std::cout << '\n';
    }




    //////////////////////////////////////////////////// OPERATIONS BETWEEN MATRICES ////////////////////////////////////////////////////

    //Addition operator - adds matrices (entry by entry)
    matrix operator+(const matrix add){
        if (numRows != add.numRows || numCollumns != add.numCollumns) operationError();
        std::vector<std::vector<type>> result;
        for(int i = 0; i < numRows; i++){
            result.push_back({});
            for(int j = 0; j < numCollumns; j++){
                result[i].push_back(data[i][j]+add.data[i][j]);
            }
        }
        return matrix(result);
    }
    //Scalar multiplication operator
    matrix operator*(const type scalar){
        std::vector<std::vector<type>> result;
        for(int i = 0; i < numRows; i++){
            result.push_back({});
            for(int j = 0; j < numCollumns; j++){
                result[i].push_back(data[i][j]*scalar);
            }
        }
        return matrix(result);
    }
    //Multiplication operator - does matrix multiplication (throws exception if impossible)
    matrix operator*(const matrix multiply){
        if(numCollumns!=multiply.numRows) operationError();
        //default matrix
        std::vector<std::vector<type>> result;
        std::vector<type> defaultRow;
        for (int i = 0; i < multiply.numCollumns; i++) defaultRow.push_back((type)0);
        for (int i = 0; i < numRows; i++ ){ result.push_back(defaultRow); }
        //fill matrix with values
        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < multiply.numCollumns; j++){
                for(int k = 0; k < numCollumns; k++){
                    result[i][j] += data[i][k]*multiply.data[k][j];
                }
            }
        }
        return matrix(result);
    }
    



    //////////////////////////////////////////////////// VECTOR OPERATIONS ////////////////////////////////////////////////////

    //Cross product - calculates the cross product for 2 3x1 matrices - exception if they aren't 3x1
    matrix operator^(const matrix m){
        if (numRows != 3 || m.numRows != 3 || numCollumns != 1 || m.numCollumns != 1) operationError();
        std::vector<std::vector<type>> result;
        for (int i = 0; i < 3; i++){ result.push_back({}); }
        result[0].push_back(data[1][0]*m.data[2][0]-data[2][0]*m.data[1][0]);
        result[1].push_back(data[2][0]*m.data[0][0]-data[0][0]*m.data[2][0]);
        result[2].push_back(data[0][0]*m.data[1][0]-data[1][0]*m.data[0][0]);
        return matrix(result);
    }
    //Dot product - calculates the dot product for 2 nx1 matrices - exception if they aren't nx1
    type operator%(const matrix dotProduct){
        if (numCollumns != 1 || dotProduct.numCollumns != 1) operationError();
        type result = (type)0;
        for(int i = 0; i < numRows; i++){
            result += data[i][0]*dotProduct.data[i][0];
        }
        return result;
    }
    //Get size of vector
    type size(){
        return sqrt((*this)%(*this));
    }
    //Normalizes Collum Matrices - error if not collumn matrix
    void normalizeVector(){
        type scalar = 1/(*this).size();
        (*this) = (*this)*scalar;
    }



    //////////////////////////////////////////////////// MATRIX FUNCTIONS ////////////////////////////////////////////////////

    //Matrix trace
    type trace(){
        squareError();
        type result;
        for(int i = 0; i < numCollumns; i++){
            result+=data[i][i];
        }
        return result;
    }
    //Determinant
    type determinant(){
        //exceptions
        squareError();
        //minimal size
        if(numRows==1) return data[0][0];
        //intermidiate variables
        type result;
        int incrementX;
        int multiplier;
        std::vector<std::vector<type>> subMatrix;
        std::vector<type> defaultVector;
        //set default row
        for (int i=0; i<numRows-1; i++) defaultVector.push_back((type)0);
        //clean sub-matrix
        for (int j=0; j<numRows-1; j++) subMatrix.push_back(defaultVector);

        for(int i = 0; i < numRows; i++){
            //fill sub-matrix
            incrementX=0;
            for(int x=0;x<numRows-1;x++){
                for(int y=0;y<numRows-1;y++){
                    if (x==i) incrementX=1;
                    subMatrix[y][x] = data[y+1][x+incrementX];
                }
            }
            //recursion step - add to result
            matrix<type> temp(subMatrix);
            if (i%2==0){multiplier=1;}else{multiplier=-1;}
            result += data[0][i]*temp.determinant()*multiplier;
        }
        return result;

    };
    //get entry
    type getEntry();
    //Adjoint Matrix
    matrix adjoint();
    //Transpose operator
    matrix transpose();
    //Inverse matrix
    matrix inverse();





    //equation solvers
    std::vector<type> solve();
    std::vector<type> solve(std::vector<type> solveFor); // throw error if size(solveFor)<k

    //Characteristic check
    int characteristic();




};