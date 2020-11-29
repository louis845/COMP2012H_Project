#ifndef LINEAR_LINEAROPS
#define LINEAR_LINEAROPS

#include "debug.h"
#include "math/R.h"
#include "steps/StepsHistory.h"
#include "math/linear/LinOpsSteps.h"
#include <string>

class LinearOperations{
public:
    virtual void row_add(int from, int to, const RF& mult);
    virtual void row_swap(int i, int j);
    virtual void row_multiply(int row, const RF& mult);

    virtual void col_add(int from, int to, const RF& mult);
    virtual void col_swap(int i, int j);
    virtual void col_multiply(int col, const RF& mult);
public:
    /**
     * Creates a LinearOperations class to work on the given matrix. DOES NOT create a copy of RF. Also assumes that the type of RF is exactly the same!
     * See R::get_type().deep_equals. Notice that this classs does not deallocate the pointers passed to it. The 
    */
    LinearOperations(RF** matrix, const int& rows, const int& cols, bool transpose, LinOpsRecorder* recorder);

    /**
     * This just deletes the pointer to the matrix, does not deallocate any memory in the arguments passed in the constructor.
    */
    virtual ~LinearOperations();

    LinearOperations(const LinearOperations&) = delete;

    LinearOperations(LinearOperations&&) = delete;

    LinearOperations& operator= (const LinearOperations&) = delete;

    LinearOperations& operator= (LinearOperations&&) = delete;
    
    const int& get_rows() const;
    const int& get_cols() const;

    /**
     * Does row operations on the matrix, recording the steps to recorder. Reduces the matrix to reduced row echelon form.
    */
    void toRREF();

    /**
     * Diagonalizes the matrix without doing row/col multiplication operations. Virtual to let subclasses that disallow col operations to 'disable' this function
     * by throwing an error.
    */
    virtual void diagonalize_no_mult();

    /**
     * Diagonalizes the matrix without doing row/col multiplication operations, and also works for all Ring classes.
     * Virtual to let subclasses that disallow col operations to 'disable' this function by throwing an error.
    */
    virtual void diagonalize_no_mult_no_div();

    /**
     * Checks whether the matrix now is diagonal, with entries equal to one.
    */
    bool is_diagonally_one() const;
private:
    /**
     * Reduce to row echelon form
    */
    void ref_reduce(int cur_row, int cur_col);
    /**
     * Complete the steps, to reduced row echelon form
    */
    void complete_reduce(int cur_row, int cur_col);

    void diagonalize_submat(int cur_row,int cur_col);

    void diagonalize_nmd_submat(int cur_row,int cur_col);

    void findSmallestEuclideanFunc(int cur_row,int cur_col,int& non_zero_row, int& non_zero_col);

    /**
     * Pauses recording, any operations will be saved as a string. Creates an internal string array with length i.
    */
    void pause_recording(int length);

    /**
     * Continues recording, and saves any recorded operations into the recorder. Releases the string array.
    */
    void commit_recording_and_continue();

    RF& A(const int& row,const int& col) const;

    LinOpsRecorder* recorder;
    bool recording;
    int record_index,record_length;
    std::string *recorded_ops_in_pause, *recorded_latex_ops_in_pause;
    char row_col, row_col_T;

    RF*** matrix;
    int rows;
    int cols;
};

/**
 * In all of the functions here, StepsHistory is dynamically allocated if matrix contains compatible types,
 * else steps will be set to nullptr. Does not deallocate any pointers passed to this function.
*/
namespace LinearOperationsFunc{
    /**
     * Reduces the given matrix to row echelon, along with steps. StepsHistory is dynamically allocated if matrix contains compatible types,
     * else steps will be set to nullptr. Does not deallocate any pointers passed to this function.
    */
    void row_reduce(R** mat, int rows, int cols, StepsHistory* &steps);

    /**
     * See row_reduce.
    */
    void col_reduce(R** mat, int rows, int cols, StepsHistory* &steps);

    /**
     * Reduces the augmented matrix to obtain the result. Cols is the exact number columns of the augmented matrix.
    */
    void solve(R** mat, int rows, int cols, StepsHistory* &steps);

    /**
     * Finds the left/right/general inverse, if exists, of the matrix.
    */
    void invert(R** mat, int rows, int cols, StepsHistory* &steps);

    void determinant(R** mat,int rows,int cols,StepsHistory*& steps);

    void char_poly(R** mat,int rows,int cols,StepsHistory*& steps);
}
#endif