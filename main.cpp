#include <iostream>
#include "matrix.h"

int main(){

    matrix<float> square({{1,2,3},{0,1,2},{2,3,0}});
    std::cout << "the determinant is " << square.determinant() << '\n';

}