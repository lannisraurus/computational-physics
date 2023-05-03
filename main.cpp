#include <iostream>
#include "matrix.h"

int main(){

    matrix<float> square({{1,2,3},{4,5,6},{7,8,9}});
    square.representation();
    matrix<float> cofactor = square.cofactorMatrix();
    cofactor.representation();

}