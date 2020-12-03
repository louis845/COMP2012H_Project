#include "math/linear/LinearOperations.h"
#include "math/linear/CoupledOperations.h"
#include "math/fraction/Fraction.h"
#include "math/polynomial/Polynomial.h"
#include "math/R.h"
#include <string>
#include <algorithm>
#include <vector>
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

    num_swaps=0;
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
    ++num_swaps;
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

    ++num_swaps;
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

const int& LinearOperations::get_num_swaps() const{
    return num_swaps;
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

void LinearOperations::diagonalize_no_mult(bool fully){
    diagonalize_submat(0,0,fully);
}

void LinearOperations::diagonalize_submat(int cur_row,int cur_col,bool fully){
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
    if(is_all_zero && fully){ //We need to check the row is zero only if fully is enabled.
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

        //Now zero the whole row, only if required to reduce fully to diagonal
        if(fully){
            num_non_zero=0;
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
    }
    diagonalize_submat(cur_row+1,cur_col+1,fully);
}

void LinearOperations::diagonalize_no_mult_no_div(bool fully){
    diagonalize_nmd_submat(0,0,fully);
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

void LinearOperations::diagonalize_nmd_submat(const int cur_row,const int cur_col,bool fully){
    if(cur_row==cols || cur_row==rows){
        return;
    }
    bool not_all_zeroed=true;
    while(not_all_zeroed){
        int non_zero_row=-1;
        int non_zero_col=-1; //The location (row,col), of a non-zero element with the smallest euclidean func.

        bool required_zero=false; //Whether or not the requirement of zeroing the elements are satisfied.

        findSmallestEuclideanFunc(cur_row,cur_col,non_zero_row,non_zero_col);

        if(fully){
            required_zero = non_zero_row==-1; //For full reduction, we need to zero all the elements in the row and col.
        }else{
            //We only need to zero the column.
            required_zero=true;
            for(int row=cur_row+1;row<rows;++row){
                if(!A(row,cur_col).is_zero()){
                    required_zero=false;
                    break;
                }
            }
        }

        //If not all zero, we need to do operations.
        if(!required_zero){
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
            if(fully){
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
        }else{
            not_all_zeroed=false;
        }
    }
    diagonalize_nmd_submat(cur_row+1,cur_col+1,fully);
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

RF projection_coeff(RF** from,RF** project_onto, int length){
    RF onto_norm=project_onto[0][0].promote(R::ZERO);
    RF inner_prod=project_onto[0][0].promote(R::ZERO);
    for(int i=0;i<length;i++){
        const RF &onto_val=*project_onto[i];
        const RF &from_val=*from[i];

        cout<<onto_val<<" "<<onto_val.conjugate()<<"\n";
        cout<<from_val<<" "<<from_val.conjugate()<<"\n";

        onto_norm = onto_norm + onto_val * onto_val.conjugate();
        inner_prod = inner_prod + from_val * onto_val.conjugate();
    }
    std::cout<<inner_prod<<"\n";
    std::cout<<onto_norm<<"\n";
    return -inner_prod/onto_norm;
}

void LinearOperations::orthogonalize_rows(){
    for(int row=1;row<rows;++row){
        //Minus the projection of the previous orthogonalized rows.
        pause_recording(row);
        RF* projs=new RF[row];
        for(int ortho_row=0;ortho_row<row;++ortho_row){
            projs[ortho_row]=projection_coeff(matrix[row],matrix[ortho_row],cols);
        }
        for(int ortho_row=0;ortho_row<row;++ortho_row){
            row_add(ortho_row, row, projs[ortho_row]);
        }
        delete[] projs;
        commit_recording_and_continue();
    }
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
            steps->add_step(new StepText{"The system of linear equations is solved by reducing the augmented matrix.", //Console
            "The system of linear equations is solved by reducing the augmented matrix." //Latex
            });
            LinOpsRecorder rc{steps, mat, rows, cols};
            rc.capture_initial();

            CoupledOperations o{mat, rows, cols-1, false, &rc, last_col, 1};

            o.toRREF();

            bool solvable=true;
            std::vector<int> fixed_variables_cols; //The columns representing the fixed variables.

            for(int row=0;row<rows;++row){
                bool has_var=false;
                for(int col=0;col<cols-1;++col){
                    if(mat[row][col].is_one()){
                        has_var=true;
                        fixed_variables_cols.push_back(col);
                        break;
                    }
                }
                if((!has_var) && !last_col[row][0].is_zero()){
                    solvable=false;
                }
            }

            if(solvable){
                RF** particular_solution=new RF*[cols-1];
                RF** sol_space;

                int num_free_vars = cols-1-fixed_variables_cols.size();

                if(num_free_vars==0){ //All variables are fixed, no solution space
                    sol_space=nullptr;
                }else{
                    sol_space=new RF*[cols-1];
                }

                for(int ncol=0;ncol<cols-1;++ncol){
                    particular_solution[ncol]=new RF[1];
                    particular_solution[ncol][0]=mat[0][0].promote(R::ZERO);
                    if(num_free_vars>0){
                        sol_space[ncol]=new RF[num_free_vars];
                        for(int var=0;var<num_free_vars;++var){
                            sol_space[ncol][var]=mat[0][0].promote(R::ZERO);
                        }
                    }
                }

                if(num_free_vars>0){
                    int free_var_idx=0;
                    for(int col=0;col<cols-1;++col){
                        if(std::find(fixed_variables_cols.begin(),fixed_variables_cols.end(), col) == fixed_variables_cols.end()){

                            //Whether the column represents a free variable.

                            //Free variable contributes 1 to itself.
                            sol_space[col][free_var_idx] = mat[0][0].promote_one();

                            for(int row=0;row<rows;++row){
                                //In the RREF, the last columns which are zero can be omitted.
                                if(row < fixed_variables_cols.size()){
                                    sol_space[fixed_variables_cols.at(row)][free_var_idx] = -mat[row][col]; //The free variable contributes -value to the corresponding first
                                    //fixed variable column of the row.
                                }
                            }
                            ++free_var_idx;
                        }
                    }
                }

                for(int row=0;row<rows;++row){
                    if(row < fixed_variables_cols.size()){
                        //In the RREF, the last columns which are zero can be omitted.
                        particular_solution[fixed_variables_cols.at(row)][0]=mat[row][cols-1];
                    }
                }

                if(num_free_vars>0){
                    string text="A particular solution is the column A, with solution space B";
                    string latex="A particular solution is the column $A$, with solution space $B$. This means general solutions is of the form $A+Bv$ where $v$ is a column vector with height "+to_string(num_free_vars)+"\n";
                    MatrixSpaceStep *m=new MatrixSpaceStep{particular_solution, cols-1, 1, sol_space, cols-1, num_free_vars, text, latex};
                    steps->add_step(m);
                    steps->setAnswer(m->get_matrix_copy(),m->get_matrix_rows(),m->get_matrix_cols());
                }else{
                    MatrixSpaceStep *m=new MatrixSpaceStep{particular_solution, cols-1, 1, -1, true, "The unique solution (in column vector form) of the system is:"};
                    steps->add_step(m);
                    steps->setAnswer(m->get_matrix_copy(),m->get_matrix_rows(),m->get_matrix_cols());
                }

                RF::deallocate_matrix(particular_solution, cols-1);
                if(num_free_vars>0){
                    RF::deallocate_matrix(sol_space, cols-1);
                }
            }else{
                steps->add_step(new StepText{"The system of linear equations is not solvable."});
            }

            delete[] last_col; //No need to deep delete last col, since it just points to the last column of mat.
            RF::deallocate_matrix(mat,rows);
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
            steps->add_step(new StepText{"The inverse of the matrix is found by adjoining an identity matrix onto the given matrix and then with row or col operations.", //Console
            "The inverse of the matrix is found by adjoining an identity matrix onto the given matrix and then with row or col operations." //Latex
            });
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
                        string latex="General left inverse is $A+XB$, for arbitrary matrix $X$ (with size $"+to_string(cols)+" \\times "+to_string(rows-cols)+"$ ). This is such that $(A+XB) \\cdot \\text{input}=I$.";
                        MatrixSpaceStep *m=new MatrixSpaceStep{cpl, rows, rows, cols, true, text, latex};
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
                    string latex="General right inverse is $A+BX$, for arbitrary matrix $X$ (with size $"+to_string(cols-rows)+" \\times "+to_string(rows)+"$ ). This is such that $\\text{input} \\cdot (A+BX)=I$. ";
                    MatrixSpaceStep *m=new MatrixSpaceStep{cpl, cols, cols, rows, false, text, latex};
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
        if(rows!=cols){
            return;
        }
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            RF::promote_to_field(mat, rows, cols);
            steps=new StepsHistory;
            steps->add_step(new StepText{"The determinant is found using row/col operations that do not change the determinant.", //Console
            "The determinant is computed with row/col operations that do not change the determinant. Notice that swapping rows/cols multiplies the determinant by -1, and adding a multiple times row/col to another row/col does not change the determinant." //Latex
            });
            LinOpsRecorder rc{steps, mat, rows, cols};
            rc.capture_initial();

            LinearOperations o{mat, rows, cols, false, &rc};

            o.diagonalize_no_mult();

            RF val=mat[0][0].promote_one();
            for(int i=0;i<std::min(rows,cols);i++){
                val=val*mat[i][i];
            }
            if(o.get_num_swaps()%2 == 1){
                val = -val;
            }

            string text="Number of row/col swaps: "+to_string(o.get_num_swaps())+"\nMultiplying (-1)^"+to_string(o.get_num_swaps())
                +" by the diagonals of the matrix, the result is:\n"+val.to_string();
            string latex="$$\\text{Number of row/col swaps }= "+to_string(o.get_num_swaps())+"$$\n "
                "\\begin{align}\\text{Result }&=(-1)^{"+to_string(o.get_num_swaps())+"} \\cdot \\text{ Product of diagonals} \\\\\n"
                "&="+val.to_latex()+
                "\\end{align}";
            steps->add_step(new StepText{text,latex});

            RF::deallocate_matrix(mat,rows);

            RF** answer=new RF*[1];
            answer[0]=new RF[1]{val};
            steps->setAnswer(answer,1,1);
        }
    }

    void char_poly(R** mat_slow,int rows,int cols,StepsHistory*& steps){
        steps=nullptr;
        if(rows!=cols){
            return;
        }
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){

            //Only FRACTION COMPLEXIFY LONG or FINITE_FIELD, which means polynomials and rational polynomials are excluded.
            if( ( !Ring::is_type_subset(NestedRingType::FRACTION_COMPLEX_LONG,mat[0][0].get_type()) ) && mat[0][0].get_type().get_current_type()!=RingType::MOD_FIELD ){
                //Not subset of FRACTION COMPLEXIFY LONG, dealloc and remove
                RF::deallocate_matrix(mat,rows);
            }else{
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
                steps->add_step(new StepText{"The determinant is found using row/col operations that do not change the determinant.", //Console
                "The determinant is computed with row/col operations that do not change the determinant. Notice that swapping rows/cols multiplies the determinant by -1, and adding a multiple times row/col to another row/col does not change the determinant." //Latex
                });
                LinOpsRecorder rc{steps, mat, rows, cols};
                rc.capture_initial();

                LinearOperations o{mat, rows, cols, false, &rc};

                o.diagonalize_no_mult_no_div();

                RF val=mat[0][0].promote_one();
                for(int i=0;i<std::min(rows,cols);i++){
                    val=val*mat[i][i];
                }
                
                if(o.get_num_swaps()%2 == 1){
                val = -val;
                }

                string text="Number of row/col swaps: "+to_string(o.get_num_swaps())+"\nMultiplying (-1)^"+to_string(o.get_num_swaps())
                    +" by the diagonals of the matrix, the result is:\n"+val.to_string();
                string latex="$$\\text{Number of row/col swaps }= "+to_string(o.get_num_swaps())+"$$\n "
                    "\\begin{align}\\text{Result }&=(-1)^{"+to_string(o.get_num_swaps())+"} \\cdot \\text{ Product of diagonals} \\\\\n"
                    "&="+val.to_latex()+
                    "\\end{align}";
                steps->add_step(new StepText{text,latex});

                RF::deallocate_matrix(mat,rows);

                RF** answer=new RF*[1];
                answer[0]=new RF[1]{val};
                steps->setAnswer(answer,1,1);
            }
        }
    }

    void orthogonalize(R** mat_slow,int rows,int cols,StepsHistory*& steps){
        steps=nullptr;
        RF** mat=RF::copy_and_promote_if_compatible(mat_slow, rows, cols);
        if(mat!=nullptr){
            //Only FRACTION COMPLEXIFY LONG, which means polynomials and rational polynomials are excluded.
            if(!Ring::is_type_subset(NestedRingType::FRACTION_COMPLEX_LONG,mat[0][0].get_type())){
                //Not subset of FRACTION COMPLEXIFY LONG, dealloc and remove
                RF::deallocate_matrix(mat,rows);
            }else{
                RF::promote_to_field(mat,rows,cols);

                steps=new StepsHistory;
                steps->add_step(new StepText{"Do Gram-Schmidt process on the matrix. Treats the rows as row vectors.", //Console
                "Do Gram-Schmidt process on the matrix. Treats the rows as row vectors. For each row, the process computes the orthogonal projections to the previous rows, and then subtracts the orthogonal projections from the current row. This is equivalent to doing row operations." //Latex
                });
                LinOpsRecorder rc{steps, mat, rows, cols};
                rc.capture_initial();

                LinearOperations o{mat, rows, cols, false, &rc};

                o.orthogonalize_rows();

                steps->setAnswer(mat, rows, cols);
            }
        }
    }
}
