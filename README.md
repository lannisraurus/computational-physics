# Computational Physics
Here are some tools for my computational physics class.
# Matrix.h
Features to fix - Cofactor matrix isn't working well; generalize sub matrix operation and empty matrix operation; allow for automatic matrix generation (completly full and identity matrix).</br></br>
Create matrix:
```
matrix<datatype> name(std::vector<std::vector<datatype>>);
```
Example:
```
matrix<int> test({
{1,2,3},
{4,5,6},
{7,8,9}
});
```
Representation function - cout's the matrix into the console:
```
matrix<int> A({{1,2,3},{3,2,1}});
A.representation();
```
```
OUTPUT:
[ 1 2 3 ]
[ 3 2 1 ]
```
Addition operation - adds matrices of the same size, entry by entry:
```
matrix<int> A({{1,2,3},{3,2,1}});
matrix<int> B = A+A;
```
