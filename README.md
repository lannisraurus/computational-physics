
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

Matrix Multiplication:
```
matrix<int> A({{1,2,6}});
matrix<int> B({{5},{3},{1}});
C=A*B;
C.representation();
```

```
OUTPUT:
[ 5 10 30 ]
[ 3 6 18 ]
[ 1 2 6 ]
```
Determinant:
```
matrix<int> A({{1,2},{1,3}});
std::cout << A.determinant();
```

```
OUTPUT:
1
```
Dot product (nx1):
```
matrix<int> A({{1},{1},{2}});
matrix<int> B({{0},{3},{1}});
std::cout << A%B;
```

```
OUTPUT:
5
```
Cross Product (3x1):
```
matrix<int> A({{1},{0},{0}});
matrix<int> B({{0},{1},{0}});
matrix<int> C = A^B;
C.representation();
```

```
OUTPUT:
[ 0 ]
[ 0 ]
[ 1 ]

```
Size:
```
float num = 1/sqrt(3);
matrix<float> A({{num},{num},{num}});
std::cout << A.size();
```

```
OUTPUT:
1
```
Normalize Vector:
```
matrix<float> A({{1},{1},{1}});
A.normalizeVector();
A.representation();
```

```
OUTPUT:
[ 0,577.. ]
[ 0,577.. ]
[ 0,577.. ]
```
