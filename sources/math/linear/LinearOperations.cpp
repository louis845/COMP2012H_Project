#include "math/linear/LinearOperations.h"
#include "math/linear/CoupledOperations.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"
#include <string>
using namespace std;

LinearOperations::LinearOperations(RF** mat, const int& rows, const int& cols, bool transpose, LinOpsRecorder* recorder){
    this->recorder=recorder;
    recording=true;
    record_index=0;
    recorded_latex_ops_in_pause=recorded_ops_in_pause=nullptr;

    if(transpose){
        matrix=new RF**[cols];
        for(int i=0;i<cols;i++){
            matrix[i]=new RF*[rows];
            for(int j=0;j<rows;j++){
                matrix[i][j]=&mat[j][i];
            }
        }
        this->rows=cols;
        this->cols=rows;

        row_col='C'; //transposed, we are doing column operations.
        row_col_T='R'; //transpose of transpose of rows is still rows.
    }else{
        matrix=new RF**[rows];
        for(int i=0;i<rows;i++){
            matrix[i]=new RF*[cols];
            for(int j=0;j<cols;j++){
                matrix[i][j]=&mat[i][j];
            }
        }
        this->rows=rows;
        this->cols=cols;

        row_col='R';
        row_col_T='C';
    }
}

LinearOperations::~LinearOperations(){
    for(int i=0;i<rows;i++){
        delete[] matrix[i];
    }
    delete[] matrix;
}

void LinearOperations::row_add(int from, int to, const RF& mult) {
    for(int col=0;col<cols;++col){
        A(to,col) = A(from,col)*mult+A(to,col);
    }
    if(recording){
        string* console=new string[1]{row_col+to_string(to+1)+mult.to_coeff()+row_col+to_string(from+1)};
        string* latex=new string[1]{row_col+string{"_{"}+to_string(to+1)+"}"+mult.to_latex_coeff()+row_col+string{"_{"}+to_string(from+1)+"}"};
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{row_col+to_string(to+1)+mult.to_coeff()+row_col+to_string(from+1)};
        recorded_latex_ops_in_pause[record_index]=string{row_col+string{"_{"}+to_string(to+1)+"}"+mult.to_latex_coeff()+row_col+string{"_{"}+to_string(from+1)+"}"};
        ++record_index;
    }
}

void LinearOperations::row_swap(int i, int j) {
    for(int col=0;col<cols;++col){
        RF temp=A(i,col);
        A(i,col)=A(j,col);
        A(j,col)=temp;
    }
    if(recording){
        string* console=new string[1]{row_col+to_string(i+1)+"<->"+row_col+to_string(j+1)};
        string* latex=new string[1]{row_col+string{"_{"}+to_string(i+1)+"} \\Leftrightarrow "+string{row_col}+"_{"+to_string(j+1)+"}"};;
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{row_col+to_string(i+1)+"<->"+row_col+to_string(j+1)};
        recorded_latex_ops_in_pause[record_index]=string{row_col+string{"_{"}+to_string(i+1)+"} \\Leftrightarrow "+string{row_col}+"_{"+to_string(j+1)+"}"};
        ++record_index;
    }
}

void LinearOperations::row_multiply(int row, const RF& mult) {
    for(int col=0;col<cols;++col){
        A(row,col) = A(row,col)*mult;
    }
    if(recording){
        string* console=new string[1]{mult.to_coeff()+row_col+to_string(row+1)};
        string* latex=new string[1]{mult.to_latex_coeff()+row_col+"_{"+to_string(row+1)+"}"};
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{mult.to_coeff()+row_col+to_string(row+1)};
        recorded_latex_ops_in_pause[record_index]=string{mult.to_latex_coeff()+row_col+"_{"+to_string(row+1)+"}"};
        ++record_index;
    }
}

void LinearOperations::col_add(int from, int to, const RF& mult) {
    for(int row=0;row<rows;++row){
        A(row,to) = A(row,from)*mult+A(row,to);
    }
    if(recording){
        string* console=new string[1]{row_col_T+to_string(to+1)+mult.to_coeff()+row_col_T+to_string(from+1)};
        string* latex=new string[1]{row_col_T+string{"_{"}+to_string(to+1)+"}"+mult.to_latex_coeff()+row_col_T+string{"_{"}+to_string(from+1)+"}"};
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{row_col_T+to_string(to+1)+mult.to_coeff()+row_col_T+to_string(from+1)};
        recorded_latex_ops_in_pause[record_index]=string{row_col_T+string{"_{"}+to_string(to+1)+"}"+mult.to_latex_coeff()+row_col_T+string{"_{"}+to_string(from+1)+"}"};
        ++record_index;
    }
}

void LinearOperations::col_swap(int i, int j) {
    for(int row=0;row<rows;++row){
        RF temp=A(row,i);
        A(row,i)=A(row,j);
        A(row,j)=temp;
    }
    if(recording){
        string* console=new string[1]{row_col_T+to_string(i+1)+"<->"+row_col_T+to_string(j+1)};
        string* latex=new string[1]{row_col_T+string{"_{"}+to_string(i+1)+"} \\Leftrightarrow "+string{row_col_T}+"_{"+to_string(j+1)+"}"};;
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{row_col_T+to_string(i+1)+"<->"+row_col_T+to_string(j+1)};
        recorded_latex_ops_in_pause[record_index]=string{row_col_T+string{"_{"}+to_string(i+1)+"} \\Leftrightarrow "+string{row_col_T}+"_{"+to_string(j+1)+"}"};
        ++record_index;
    }
}

void LinearOperations::col_multiply(int col, const RF& mult) {
    for(int row=0;row<rows;++row){
        A(row,col) = A(row,col)*mult;
    }
    if(recording){
        string* console=new string[1]{mult.to_coeff()+row_col_T+to_string(col+1)};
        string* latex=new string[1]{mult.to_latex_coeff()+row_col_T+"_{"+to_string(col+1)+"}"};
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{mult.to_coeff()+row_col_T+to_string(col+1)};
        recorded_latex_ops_in_pause[record_index]=string{mult.to_latex_coeff()+row_col_T+"_{"+to_string(col+1)+"}"};
        ++record_index;
    }
}

const int& LinearOperations::get_rows() const{
    return rows;
}

const int& LinearOperations::get_cols() const{
    return cols;
}

/**
 * Row echleon form / column echelon form, depending on whether the matrix is transposed.
*/
void LinearOperations::toRREF(){
    ref_reduce(0,0); //Row echelon form
    complete_reduce(0,0); //Complete reduced row echelon form
}

/**
 * Reduces to row echelon form
*/
void LinearOperations::ref_reduce(int cur_row, int cur_col){
    if(cur_col==cols){
        return;
    }
    if(cur_row==rows){
        return;
    } //return if at the end
    int non_zero_rows=0; //Number of non zero elements in the current column, excluding A(cur_row, cur_col)

    //If current column is zero, go next. If not, swap and normalize
    if(A(cur_row,cur_col).is_zero()){
        int nRows=0;
        int to_swap=-1;
        for(int row=cur_row+1; row<rows; row++){
            if(!A(row,cur_col).is_zero()) {
                nRows++;
                to_swap=row;
            }
        }
        if (nRows==0) {
            //Go next here, zero column
            ref_reduce(cur_row,cur_col+1);
            return;
        }

        row_swap(cur_row,to_swap); //A non-zero row is found

        non_zero_rows=nRows-1;
    }else{
        for(int row=cur_row+1; row<rows; ++row){
            if(!A(row,cur_col).is_zero()){
                non_zero_rows++;
            }
        }
    }
    //The A(cur_row,cur_col) is non-zero now
    if(!A(cur_row,cur_col).is_one()){
        RF multiply=A(cur_row,cur_col).invert();
        row_multiply(cur_row, multiply);
    }

    if(non_zero_rows>0){
        pause_recording(non_zero_rows);
        int idx=0;

        for(int row=cur_row+1; row<rows; row++){
            if(!A(row,cur_col).is_zero()){
                RF co= -A(row,cur_col);
                row_add(cur_row, row, co);
                
            }
        }
        commit_recording_and_continue();
    }
    ref_reduce(cur_row+1, cur_col+1);
}

/**
 * To reduced row echelon form here
*/
void LinearOperations::complete_reduce(int cur_row, int cur_col){
    if(cur_col==cols){
        return;
    }
    if(cur_row==rows){
        return;
    }
    int lastIdx=-1; //index of the last element that is one in the column
    for(int row=cur_row; row<rows; row++){
        if(A(row,cur_col).is_one()){
            lastIdx=row;
        }
    }
    
    if(lastIdx!=-1){
        //found element value equal to one in column
        int count = 0;
        for(int row = 0; row < lastIdx; row++){
            if(!A(row,cur_col).is_zero()){
                count++;
            }
        }
        if(count>0){
            pause_recording(count);
            int idx=0;
            for(int row=0; row<lastIdx; row++){
                if(!A(row,cur_col).is_zero()){
                    RF co = -A(row,cur_col);
                    row_add(lastIdx,row,co);
                }
            }
            commit_recording_and_continue();
        }
        complete_reduce(cur_row+1, cur_col+1);
    }else{
        // not found, go next
        complete_reduce(cur_row, cur_col+1);
    }
}

void LinearOperations::diagonalize_no_mult(){
    diagonalize_submat(0,0);
}

void LinearOperations::diagonalize_submat(int cur_row,int cur_col){
    if(cur_row==cols || cur_row==rows){
        return;
    }
    bool is_all_zero=true; //Is the row cur_row, and the col cur_col all zero?
    int non_zero_row=-1;
    int non_zero_col=-1; //The location (row,col), of an instance of an non-zero element.

    for(int row=cur_row+1;row<rows;++row){
        if(!A(row,cur_col).is_zero()){
            is_all_zero=false;
            non_zero_row=row;
            non_zero_col=cur_col;
            break;
        }
    }
    if(is_all_zero){
        for(int col=cur_col+1;col<cols;++col){
            if(!A(cur_row,col).is_zero()){
                is_all_zero=false;
                non_zero_row=cur_row;
                non_zero_col=col;
                break;
            }
        }
    }

    //If not all zero, we need to do operations.
    if(!is_all_zero){
        if(A(cur_row,cur_col).is_zero()){
            //need to swap.
            if(cur_row==non_zero_row){
                //col swap
                col_swap(cur_col,non_zero_col);
            }else{
                //row swap (here cur_col==non_zero_col)
                row_swap(cur_row,non_zero_row);
            }
        }

        int num_non_zero=0; //Number of non_zero elements.
        //Now zero the whole column.
        for(int row=cur_row+1;row<rows;++row){
            if(!A(row,cur_col).is_zero()){
                ++num_non_zero;
            }
        }

        if(num_non_zero>0){
            pause_recording(num_non_zero);
            for(int row=cur_row+1;row<rows;++row){
                if(!A(row,cur_col).is_zero()){
                    RF mult=-A(row,cur_col)/A(cur_row,cur_col);
                    row_add(cur_row,row,mult);
                }
            }
            commit_recording_and_continue();
        }

        num_non_zero=0;
        //Now zero the whole row.
        for(int col=cur_col+1;col<cols;++col){
            if(!A(cur_row,col).is_zero()){
                ++num_non_zero;
            }
        }

        if(num_non_zero>0){
            pause_recording(num_non_zero);
            for(int col=cur_col+1;col<cols;++col){
                if(!A(cur_row,col).is_zero()){
                    RF mult=-A(cur_row,col)/A(cur_row,cur_col);
                    col_add(cur_col,col,mult);
                }
            }
            commit_recording_and_continue();
        }
    }
    diagonalize_submat(cur_row+1,cur_col+1);
}

void LinearOperations::diagonalize_no_mult_no_div(){
    diagonalize_nmd_submat(0,0);
}

void LinearOperations::findSmallestEuclideanFunc(int cur_row,int cur_col,int& non_zero_row, int& non_zero_col){
    non_zero_row=non_zero_col=-1;
    RF smallest_euc;

    for(int row=cur_row+1;row<rows;++row){
        if(!A(row,cur_col).is_zero()){
            if(non_zero_row==-1){
                non_zero_row=row;
                non_zero_col=cur_col;
                smallest_euc=A(row,cur_col);
            }else{
                if(A(row,cur_col) < smallest_euc){
                    non_zero_row=row;
                    non_zero_col=cur_col;
                    smallest_euc=A(row,cur_col);
                }
            }
            break;
        }
    }
    for(int col=cur_col+1;col<cols;++col){
        if(!A(cur_row,col).is_zero()){
            if(non_zero_row==-1){
                non_zero_row=cur_row;
                non_zero_col=col;
                smallest_euc=A(cur_row,col);
            }else{
                if(A(cur_row,col) < smallest_euc){
                    non_zero_row=cur_row;
                    non_zero_col=col;
                    smallest_euc=A(cur_row,col);
                }
            }
            break;
        }
    }
}

void LinearOperations::diagonalize_nmd_submat(const int cur_row,const int cur_col){
    if(cur_row==cols || cur_row==rows){
        return;
    }
    bool not_all_zeroed=true;
    while(not_all_zeroed){
        int non_zero_row=-1;
        int non_zero_col=-1; //The location (row,col), of a non-zero element with the smallest euclidean func.

        findSmallestEuclideanFunc(cur_row,cur_col,non_zero_row,non_zero_col);

        //If not all zero, we need to do operations.
        if(non_zero_row!=-1){
            if(A(cur_row,cur_col).is_zero() || (A(non_zero_row,non_zero_col)<A(cur_row,cur_col))){
                //now we swap
                if(cur_row==non_zero_row){
                    //col swap
                    col_swap(cur_col,non_zero_col);
                }else{
                    //row swap (here cur_col==non_zero_col)
                    row_swap(cur_row,non_zero_row);
                }
            }

            int num_non_zero=0; //Number of non_zero elements.
            //Now zero the whole column.
            for(int row=cur_row+1;row<rows;++row){
                if(!A(row,cur_col).is_zero()){
                    ++num_non_zero;
                }
            }

            if(num_non_zero>0){
                pause_recording(num_non_zero);
                for(int row=cur_row+1;row<rows;++row){
                    if(!A(row,cur_col).is_zero()){
                        RF mult=-A(row,cur_col)/A(cur_row,cur_col);
                        row_add(cur_row,row,mult);
                    }
                }
                commit_recording_and_continue();
            }

            num_non_zero=0;
            //Now zero the whole row.
            for(int col=cur_col+1;col<cols;++col){
                if(!A(cur_row,col).is_zero()){
                    ++num_non_zero;
                }
            }

            if(num_non_zero>0){
                pause_recording(num_non_zero);
                for(int col=cur_col+1;col<cols;++col){
                    if(!A(cur_row,col).is_zero()){
                        RF mult=-A(cur_row,col)/A(cur_row,cur_col);
                        col_add(cur_col,col,mult);
                    }
                }
                commit_recording_and_continue();
            }
        }else{
            not_all_zeroed=false;
        }
    }
    diagonalize_nmd_submat(cur_row+1,cur_col+1);
}

void LinearOperations::pause_recording(int length){
    recorded_ops_in_pause=new std::string[length];
    recorded_latex_ops_in_pause=new std::string[length];
    record_index=0;
    record_length=length;
    recording=false;
}

void LinearOperations::commit_recording_and_continue(){
    recording=true;
    recorder->capture_instance(recorded_ops_in_pause,recorded_latex_ops_in_pause,record_length);
    recorded_ops_in_pause=nullptr;
    recorded_latex_ops_in_pause=nullptr; //no need to delete, the strings gets passed onto recorder.
}

RF& LinearOperations::A(const int& row, const int& col) const{
    return *(matrix[row][col]);
}

bool LinearOperations::is_diagonally_one() const{
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(i==j){
                if(!A(i,j).is_one()){
                    return false;
                }
            }else{
                if(!A(i,j).is_zero()){
                    return false;
                }
            }
        }
    }
    return true;
}

namespace LinearOperationsFunc{
    void row_reduce(R** mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            RF::promote_to_field(mat,rows,cols);
            steps=new StepsHistory;
            LinOpsRecorder rc{steps, mat, rows, cols};
            rc.capture_initial();

            LinearOperations o{mat, rows, cols, false, &rc};

            o.toRREF();

            steps->setAnswer(mat,rows,cols);
        }else{
            steps=nullptr;
        }
    }

    void col_reduce(R** mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            RF::promote_to_field(mat,rows,cols);
            steps=new StepsHistory;
            LinOpsRecorder rc{steps, mat, rows, cols};
            rc.capture_initial();

            LinearOperations o{mat, rows, cols, true, &rc};

            o.toRREF();

            steps->setAnswer(mat,rows,cols);
        }else{
            steps=nullptr;
        }
    }

    void solve(R** mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            RF::promote_to_field(mat,rows,cols);

            RF** last_col=new RF*[rows];
            for(int i=0;i<rows;i++){
                last_col[i]=(mat[i]+cols-1); //The submatrix with column starting from col (the last column). No need to do value copy.
            }

            steps=new StepsHistory;
            LinOpsRecorder rc{steps, mat, rows, cols};
            rc.capture_initial();

            CoupledOperations o{mat, rows, cols-1, false, &rc, last_col, 1};

            o.toRREF();

            //Now last col really represents a column matrix by itself, not referencing the addresses of mat.
            for(int i=0;i<rows;i++){
                last_col[i]=new RF[1]{last_col[i][0]};
                //last_col[i][0] originally refers to the value of the mat in the lasts column (last_col[i] points to it)
                //now do a value copy and last_col will be decoupled with mat.
            }

            RF::deallocate_matrix(mat,rows);

            steps->setAnswer(last_col,rows,1);
        }else{
            steps=nullptr;
        }
    }

    void invert(R** mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            RF** combined_matrix;
            RF** cpl; //Coupled matrix, submatrix of combined_matrix. Saves a pointer to the pointers of combined_matrix.
            int mrows;
            int mcols;

            //we create a combined matrix here by combining with the identity matrix. Throughout this process, mat will be deallocated.
            if(rows>=cols){
                mrows=rows;
                mcols=rows+cols;

                combined_matrix=new RF*[rows];
                cpl=new RF*[rows];
                for(int i=0;i<rows;i++){
                    combined_matrix[i]=new RF[mcols];
                    for(int j=0;j<cols;j++){
                        if(!mat[i][j].is_field()){
                            R one=mat[i][j].promote_one();
                            combined_matrix[i][j]=RF{new Fraction{mat[i][j],one}};
                        }else{
                            combined_matrix[i][j]=mat[i][j];
                        }
                    }
                    for(int j=cols;j<mcols;j++){
                        if(i==j-cols){
                            //j-cols is the columns in the right submatrix. We want it to be diagonally one.
                            combined_matrix[i][j]=combined_matrix[0][0].promote_one(); //combined_matrix[0][0] is already initialized.
                        }else{
                            combined_matrix[i][j]=combined_matrix[0][0].promote(R::ZERO);
                        }
                    }
                    delete[] mat[i];

                    cpl[i]=(combined_matrix[i]+cols); //copy the pointer position, starting from cols.
                }
                delete[]mat;
            }else{
                mrows=rows+cols;
                mcols=cols;

                combined_matrix=new RF*[mrows];
                cpl=new RF*[cols];
                for(int i=0;i<mrows;i++){
                    combined_matrix[i]=new RF[cols];
                    for(int j=0;j<cols;j++){
                        if(i<rows){
                            if(!mat[i][j].is_field()){
                                R one=mat[i][j].promote_one();
                                combined_matrix[i][j]=RF{new Fraction{mat[i][j],one}};
                            }else{
                                combined_matrix[i][j]=mat[i][j];
                            }
                        }else{
                            if(i-rows==j){
                                //i-rows is the row in the bottom submatrix.
                                combined_matrix[i][j]=combined_matrix[0][0].promote_one();
                            }else{
                                combined_matrix[i][j]=combined_matrix[0][0].promote(R::ZERO);
                            }
                        }
                    }
                    if(i<rows)
                        delete[] mat[i];
                }
                delete[]mat;

                for(int i=0;i<cols;i++){
                    cpl[i]=combined_matrix[rows+i]; //copy the pointer position, starting from rows.
                }
            }

            //Now combined_matrix are the values of mat adjoined with identity matrix, and mat is deallocated here
            //Cpl is the submatrix pointing to the same addresses as combined_matrix

            steps=new StepsHistory;
            LinOpsRecorder rc{steps, combined_matrix, mrows, mcols}; //we record the whole matrix
            rc.capture_initial();
            
            if(rows>=cols){
                CoupledOperations o{combined_matrix, rows, cols, false, &rc, cpl, rows};

                o.toRREF();
                if(o.is_diagonally_one()){
                    if(rows==cols){
                        MatrixSpaceStep *m=new MatrixSpaceStep{cpl, rows, rows, -1, true, "The inverse of the matrix is:"};
                        steps->add_step(m);
                        steps->setAnswer(m->get_matrix_copy(),m->get_matrix_rows(),m->get_matrix_cols());
                    }else{
                        string text="General left inverse is A+XB, for arbitrary matrix X (with size "+to_string(cols)+"x"+to_string(rows-cols)+" ).\n This is such that (A+XB)*input=I";
                        MatrixSpaceStep *m=new MatrixSpaceStep{cpl, rows, rows, cols, true, text};
                        steps->add_step(m);
                        steps->setAnswer(m->get_matrix_copy(),m->get_matrix_rows(),m->get_matrix_cols());
                    }
                }else{
                    steps->add_step(new StepText{"The row echelon form is not diagonal, so the inverse does not exist!"});
                }
            }else{
                CoupledOperations o{combined_matrix, rows, cols, true, &rc, cpl, cols};

                o.toRREF();
                if(o.is_diagonally_one()){
                    string text="General right inverse is A+BX, for arbitrary matrix X (with size "+to_string(cols-rows)+"x"+to_string(rows)+" ).\n This is such that input*(A+BX)=I";
                    MatrixSpaceStep *m=new MatrixSpaceStep{cpl, cols, cols, rows, false, text};
                    steps->add_step(m);
                    steps->setAnswer(m->get_matrix_copy(),m->get_matrix_rows(),m->get_matrix_cols());
                }else{
                    steps->add_step(new StepText{"The row echelon form is not diagonal, so the inverse does not exist!"});
                }
            }

            for(int i=0;i<mrows;i++){
                delete[]combined_matrix[i];
            }
            delete[]combined_matrix;
            delete[]cpl; //no need to delete the subarrays, it is deleted in combined_matrix since cpl is a pointer copy.
        }else{
            steps=nullptr;
        }
    }

    void determinant(R** mat_slow,int rows,int cols,StepsHistory*& steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            RF::promote_to_field(mat, rows, cols);
            steps=new StepsHistory;
            LinOpsRecorder rc{steps, mat, rows, cols};
            rc.capture_initial();

            LinearOperations o{mat, rows, cols, false, &rc};

            o.diagonalize_no_mult();

            RF val=mat[0][0].promote_one();
            for(int i=0;i<std::min(rows,cols);i++){
                val=val*mat[i][i];
            }
            steps->add_step(new StepText{val.to_string()});

            RF::deallocate_matrix(mat,rows);

            RF** answer=new RF*[1];
            answer[0]=new RF[1]{val};
            steps->setAnswer(answer,1,1);
        }else{
            steps=nullptr;
        }
    }

    void char_poly(R** mat_slow,int rows,int cols,StepsHistory*& steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            NestedRingType *type=new NestedRingType{RingType::FRACTION};
            NestedRingType *inside=new NestedRingType{RingType::COMPLEXIFY};
            type->set_sub_type_no_copy(inside);
            inside->set_sub_type_no_copy(new NestedRingType{RingType::LONG});

            //Only FRACTION COMPLEXIFY LONG, which means polynomials and rational polynomials are excluded.

            if(!Ring::is_type_subset(*type,mat[0][0].get_type())){
                //Not subset of FRACTION COMPLEXIFY LONG, dealloc and remove
                delete type; //NestedRingType deletes it's children nodes, so no need to del inside
                RF::deallocate_matrix(mat,rows);
            }else{
                delete type;

                //Form Char Poly tI-A
                RF::promote_to_field(mat,rows,cols);
                for(int row=0;row<rows;row++){
                    for(int col=0;col<cols;col++){
                        R *coeff_arr;
                        int coeff_len;
                        if(row==col){
                            coeff_arr=new R[2];
                            coeff_arr[0] = -mat[row][col];
                            coeff_arr[1] = mat[row][col].promote_one();
                            coeff_len=2;
                        }else{
                            coeff_arr=new R[1];
                            coeff_len=1;
                            coeff_arr[0] = -mat[row][col];
                        }
                        mat[row][col]=RF{new Polynomial{coeff_arr,coeff_len}};
                        delete[] coeff_arr;
                    }
                }

                steps=new StepsHistory;
                LinOpsRecorder rc{steps, mat, rows, cols};
                rc.capture_initial();

                LinearOperations o{mat, rows, cols, false, &rc};

                o.diagonalize_no_mult_no_div();

                RF val=mat[0][0].promote_one();
                for(int i=0;i<std::min(rows,cols);i++){
                    val=val*mat[i][i];
                }
                steps->add_step(new StepText{val.to_string()});

                RF::deallocate_matrix(mat,rows);

                RF** answer=new RF*[1];
                answer[0]=new RF[1]{val};
                steps->setAnswer(answer,1,1);
            }
        }
    }
}