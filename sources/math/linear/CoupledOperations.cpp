#include "math/linear/CoupledOperations.h"

using namespace std;

CoupledOperations::CoupledOperations(RF** mat, const int& rows, const int& cols, bool transpose, LinOpsRecorder* recorder, RF** cpl, int couple_dim) : LinearOperations(mat, rows, cols, transpose, recorder){
    if(transpose){
        couple=new RF**[cols];
        couple_rows=cols;
        couple_cols=couple_dim;
        for(int i=0;i<cols;i++){
            couple[i]=new RF*[couple_dim];
            for(int j=0;j<couple_dim;j++){
                couple[i][j]=&cpl[j][i];
            }
        }
    }else{
        couple=new RF**[rows];
        couple_rows=rows;
        couple_cols=couple_dim;
        for(int i=0;i<rows;i++){
            couple[i]=new RF*[couple_dim];
            for(int j=0;j<couple_dim;j++){
                couple[i][j]=&cpl[i][j];
            }
        }
    }

#ifdef DEBUG_MODE
    for(int i=0;i<couple_rows;i++){
        for(int j=0;j<couple_cols;j++){
            couple[i][j]->get_type();
        }
    }
#endif
}

CoupledOperations::~CoupledOperations(){
    for(int i=0;i<couple_rows;i++){
        delete[] couple[i];
    }
    delete[]couple;
}

void CoupledOperations::row_add(int from, int to, const RF& mult) {
    for(int col=0;col<couple_cols;++col){
        A(to,col) = A(from,col)*mult+A(to,col);
    }
    LinearOperations::row_add(from, to, mult); //since super row_add records the matrix, we need to do operations on here first.
}

void CoupledOperations::row_swap(int i, int j) {
    for(int col=0;col<couple_cols;++col){
        RF temp=A(i,col);
        A(i,col)=A(j,col);
        A(j,col)=temp;
    }
    LinearOperations::row_swap(i, j); //same reason
}

void CoupledOperations::row_multiply(int row, const RF& mult) {
    for(int col=0;col<couple_cols;++col){
        A(row,col) = A(row,col)*mult;
    }
    LinearOperations::row_multiply(row, mult); //same reason
}

void CoupledOperations::col_add(int from, int to, const RF& mult){
    throw "Does not support col_add on coupled operations!";
}

void CoupledOperations::col_swap(int i,int j){
    throw "Does not support col_swap on coupled operations!";
}

void CoupledOperations::col_multiply(int row, const RF& mult){
    throw "Does not support col_multiply on coupled operations!";
}

void CoupledOperations::diagonalize_no_mult(){
    throw "Does not support diagonalize_no_mult on coupled operations!";
}

RF& CoupledOperations::A(const int& row, const int& col) const{
    return *(couple[row][col]);
}