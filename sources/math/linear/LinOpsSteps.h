#ifndef MATH_LINEAR_LINOPSSTEPS_H
#define MATH_LINEAR_LINOPSSTEPS_H
#include <string>
#include "steps/StepsHistory.h"
#include "math/R.h"
/**
 * Standard row operations steps recorder. DO NOT create this class dynamically. The StepsHistory should be in the same stack as LinOpsRecorder.
*/
class LinOpsRecorder{
public:
    StepsHistory* h;
    int rows;
    int cols;
    RF** matrix;
    /**
     * Steps history, does not delete it in destructor. Directly passes the matrix to LinOpsRecorder without copy, does not get deleted.
    */
    LinOpsRecorder(StepsHistory* h, RF** matrix, int rows, int cols);

    LinOpsRecorder(const LinOpsRecorder&) = delete;

    LinOpsRecorder(LinOpsRecorder&&) = delete;

    LinOpsRecorder& operator=(const LinOpsRecorder&) = delete;

    LinOpsRecorder& operator=(LinOpsRecorder&&) = delete;
    /**
     * Directly passes to the newly created LinOpsSteps
    */
    void capture_instance(std::string* console, std::string* latex, int length);

    /**
     * Captures initially the state of the matrix with no operations text.
    */
    void capture_initial();
};

class LinOpsSteps : public Step{
private:
    RF** matrix;
    int rows;
    int cols;
    std::string *console, *latex;
    int length;

    LinOpsSteps(RF** matrix,int rows,int cols, std::string* console, std::string* latex, int length);
    ~LinOpsSteps();

    virtual void print_to_console() const override;

    virtual std::string get_html_latex() const override;

    friend class LinOpsRecorder;
};

class MatrixSpaceStep : public Step{
private:
    RF** matrix;
    RF** space_matrix;
    int rows,rows_space;
    int cols,cols_space;
    std::string text;
public:

    /**
     * Deep copies the given matrix. cutoff and row_or_col indicates how to divide into submatrix. The cutoff is exclusive for the first matrix
     * i.e. the matrix does not contain the row/col cutoff. row_or_col=true indicates it is row. Pass cutoff=-1 for no cutoff. Here row_or_col will
     * be ignored.
    */
    MatrixSpaceStep(RF** matrix,int rows,int cols,int cutoff,bool row_or_col,const std::string& text);
    ~MatrixSpaceStep();

    virtual void print_to_console() const override;

    virtual std::string get_html_latex() const override;
};

#endif