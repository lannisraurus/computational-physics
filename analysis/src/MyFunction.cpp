#include "MyFunction.h"

MyFunction::MyFunction(string name_): Functor(name_) {}

double MyFunction::operator() (double& x){
    return pow(x,4)*log(x+sqrt(pow(x,2)+1));
}
