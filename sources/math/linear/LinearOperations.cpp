#include "math/linear/LinearOperations.h"
#include "math/linear/CoupledOperations.h"
#include "math/fraction/Fraction.h"
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
        string* latex=new string[1]{row_col+"_{"+to_string(to+1)+"}"+mult.to_latex_coeff()+row_col+"_{"+to_string(from+1)+"}"};
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{row_col+to_string(to+1)+mult.to_coeff()+row_col+to_string(from+1)};
        recorded_latex_ops_in_pause[record_index]=string{row_col+"_{"+to_string(to+1)+"}"+mult.to_latex_coeff()+row_col+"_{"+to_string(from+1)+"}"};
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
        string* latex=new string[1]{row_col+"_{"+to_string(i+1)+"} \\Leftrightarrow "+row_col+"_{"+to_string(j+1)+"}"};
        recorder->capture_instance(console,latex,1);
    }else{
        recorded_ops_in_pause[record_index]=string{row_col+to_string(i+1)+"<->"+row_col+to_string(j+1)};
        recorded_latex_ops_in_pause[record_index]=string{row_col+"_{"+to_string(i+1)+"} \\Leftrightarrow "+row_col+"_{"+to_string(j+1)+"}"};
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

const int& LinearOperations::get_rows() const{
    return rows;
}

const int& LinearOperations::get_cols() const{
    return cols;
}

void LinearOperations::toRREF(){
    ref_reduce(0,0);
    complete_reduce(0,0);
}

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

namespace OperationsWithSteps{
    void rowReduce(R **mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF **mat=RF::matrix_copy_into_equal_types(mat_slow,rows,cols);
        if(mat==nullptr){
            return;
        }
        RF::promote_all_elements_into_field(mat,rows,cols);

        steps=new StepsHistory{};

        LinOpsRecorder rc{steps, mat, rows, cols};
        rc.capture_initial();
    
        LinearOperations o{mat, rows, cols, false, &rc};

        o.toRREF();

        RF::dealloc_matrix(mat,rows);
    }

    void colReduce(R **mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF **mat=RF::matrix_copy_into_equal_types(mat_slow,rows,cols);
        if(mat==nullptr){
            return;
        }
        RF::promote_all_elements_into_field(mat,rows,cols);

        steps=new StepsHistory{};

        LinOpsRecorder rc{steps, mat, rows, cols};
        rc.capture_initial();
    
        LinearOperations o{mat, rows, cols, true, &rc};

        o.toRREF();

        RF::dealloc_matrix(mat,rows);
    }

    void solve(R **mat_slow, int rows, int cols, StepsHistory* &steps){
        steps=nullptr;
        RF **mat=RF::matrix_copy_into_equal_types(mat_slow,rows,cols);
        if(mat==nullptr){
            return;
        }
        RF::promote_all_elements_into_field(mat, rows, cols);
        RF** last_col=new RF*[rows];

        for(int i=0;i<rows;i++){
            last_col[i]=(mat[i]+cols-1); //The submatrix with column starting from col (the last column). No need to do value copy.
        }

        steps=new StepsHistory{};

        LinOpsRecorder rc{steps, mat, rows, cols};
        rc.capture_initial();

        CoupledOperations o{mat, rows, cols-1, false, &rc, last_col, 1};

        o.toRREF();

        delete[]last_col; //No need to delete last_col since last_col is only stores the pointers that point to the same values as mat.
        RF::dealloc_matrix(mat,rows);
    }

    void invert(R **mat_slow, int rows, int cols, StepsHistory* &steps){
        RF** mat;
        RF** cpl; //Coupled matrix, submatrix of mat. Saves a pointer to the pointers of mat.
        int mrows;
        int mcols;

        /**
         * Append an identity matrix with the original matrix.
        */
        if(rows>=cols){
            mrows=rows;
            mcols=rows+cols;

            mat=new RF*[rows];
            cpl=new RF*[rows];
            for(int i=0;i<rows;i++){
                mat[i]=new RF[mcols];
                for(int j=0;j<cols;j++){
                    if(!mat_slow[i][j].is_field()){
                        R one=mat_slow[i][j].promote_one();
                        mat[i][j]=RF{new Fraction{mat_slow[i][j],one}};
                    }else{
                        mat[i][j]=mat_slow[i][j];
                    }
                }
                for(int j=cols;j<mcols;j++){
                    if(i==j-cols){
                        //j-cols is the columns in the right submatrix. We want it to be diagonally one.
                        mat[i][j]=mat[0][0].promote_one(); //mat[0][0] is already initialized.
                    }else{
                        mat[i][j]=mat[0][0].promote(R::ZERO);
                    }
                }

                cpl[i]=(mat[i]+cols); //copy the pointer position, starting from cols.
            }
        }else{
            mrows=rows+cols;
            mcols=cols;

            mat=new RF*[mrows];
            cpl=new RF*[cols];
            for(int i=0;i<mrows;i++){
                mat[i]=new RF[cols];
                for(int j=0;j<cols;j++){
                    if(i<rows){
                        if(!mat_slow[i][j].is_field()){
                            R one=mat_slow[i][j].promote_one();
                            mat[i][j]=RF{new Fraction{mat_slow[i][j],one}};
                        }else{
                            mat[i][j]=mat_slow[i][j];
                        }
                    }else{
                        if(i-rows==j){
                            //i-rows is the row in the bottom submatrix.
                            mat[i][j]=mat[0][0].promote_one();
                        }else{
                            mat[i][j]=mat[0][0].promote(R::ZERO);
                        }
                    }
                }
            }

            for(int i=0;i<cols;i++){
                cpl[i]=mat[rows+i]; //copy the pointer position, starting from rows.
            }
        }

        steps=new StepsHistory{};
        LinOpsRecorder rc{steps, mat, mrows, mcols}; //we record the whole matrix
        rc.capture_initial();
        
        if(rows>=cols){
            CoupledOperations o{mat, rows, cols, false, &rc, cpl, rows};

            o.toRREF();
            if(o.is_diagonally_one()){
                if(rows==cols){
                    steps->add_step(new MatrixSpaceStep{cpl, rows, rows, -1, true, "The inverse of the matrix is:"});
                }else{
                    cout<<"bf_add_step\n";
                    string text="General left inverse is A+XB, for arbitrary matrix X (with size "+to_string(cols)+"x"+to_string(rows-cols)+" ).\n This is such that (A+XB)*input=I";
                    steps->add_step(new MatrixSpaceStep{cpl, rows, rows, cols, true, text});
                    cout<<"af_add_step\n";
                }
            }else{
                steps->add_step(new StepText{"The row echelon form is not diagonal, so the inverse does not exist!"});
            }
        }else{
            CoupledOperations o{mat, rows, cols, true, &rc, cpl, cols};

            o.toRREF();
            if(o.is_diagonally_one()){
                string text="General right inverse is A+BX, for arbitrary matrix X (with size "+to_string(cols-rows)+"x"+to_string(rows)+" ).\n This is such that input*(A+BX)=I";
                steps->add_step(new MatrixSpaceStep{cpl, cols, cols, rows, false, text});
            }else{
                steps->add_step(new StepText{"The row echelon form is not diagonal, so the inverse does not exist!"});
            }
        }

        for(int i=0;i<mrows;i++){
            delete[]mat[i];
        }
        delete[]mat;
        delete[]cpl; //no need to delete the subarrays, it is deleted in mat since cpl is a pointer copy.
    }
}