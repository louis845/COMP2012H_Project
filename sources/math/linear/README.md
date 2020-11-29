# Linear operations class
* Include "math/linear/LinearOperations.h
* The relevant functions are in namespace LinearOperationsFunc
## Implemented functions:
* void row_reduce(R** mat, int rows, int cols, StepsHistory* &steps);
* void col_reduce(R** mat, int rows, int cols, StepsHistory* &steps);
* void solve(R** mat, int rows, int cols, StepsHistory* &steps);
* void invert(R** mat, int rows, int cols, StepsHistory* &steps);
*
* Give matrix as input, with the rows and cols (of the matrix). Allocates a new StepsHistory and puts it into the pointer.
## Try using GUI - just GIT pull then run the gui QT program
