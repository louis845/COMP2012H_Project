#include "LinearOperations.h"

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

RF& LinearOperations::A(int row, int col) const{
    return *(matrix[row][col]);
}