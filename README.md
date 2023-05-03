# Computational Physics
Here are some tools for my computational physics class.
# Matrix.h
Features to fix - Cofactor matrix isn't working well; generalize sub matrix operation and empty matrix operation; allow for automatic matrix generation (completly full and identity matrix).</br></br>
Create matrix:
```
matrix<datatype> name(std::vector<std::vector<datatype>>);
```

Representation function - prints the matrix into the console:

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
B.representation();
```

```
OUTPUT:
[ 2 4 6 ]
[ 6 4 2 ]
```

Scalar Multiplication:
```
matrix<int> A({{1,2,6}});
A=A*2;
A.representation();
```

```
OUTPUT:
[ 2 4 12 ]
```
