#ifndef LINEAR_LINEAROPS
#define LINEAR_LINEAROPS

#include "math/R.h"
#include "steps/StepsHistory.h"
#include "math/linear/LinOpsSteps.h"
#include <string>

class LinearOperations{
protected:
    virtual void row_add(int from, int to, const RF& mult);
    virtual void row_swap(int i, int j);
    virtual void row_multiply(int row, const RF& mult);
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
private:
    /**
     * Reduce to row echelon form
    */
    void ref_reduce(int cur_row, int cur_col);
    /**
     * Complete the steps, to reduced row echelon form
    */
    void complete_reduce(int cur_row, int cur_col);

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
    char row_col;

    RF*** matrix;
    int rows;
    int cols;
};
#endif