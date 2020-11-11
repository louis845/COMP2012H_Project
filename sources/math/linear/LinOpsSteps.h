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
     * directly passes to the newly created LinOpsSteps
    */
    void capture_instance(std::string* console, std::string* latex, int length);
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

    friend class LinOpsRecorder;
};

#endif