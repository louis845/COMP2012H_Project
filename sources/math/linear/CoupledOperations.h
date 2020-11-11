#ifndef LINEAR_COUPLEDOPS
#define LINEAR_COUPLEDOPS

#include "linear/LinearOperations.h"
#include "steps/StepsHistory.h"

class CoupledOperations : public LinearOperations{
private:
    RF** matrix;
    int rows;
    int cols;

    void row_add(int from, int to, RF mult) override;
    void row_swap(int i, int j) override;
    void row_multiply(int i, int j) override;
public:
    /**
     * Creates a LinearOperations class to work on the given matrix. DOES NOT create a copy of RF. Also assumes that the type of RF is exactly the same!
     * See R::get_type().deep_equals
    */
    CoupledOperations(RF** matrix, const int& rows, const int& cols, RF** );
    
    const int& get_rows() const;
    const int& get_cols() const;
    const RF& get_element(int row, int col) const;

    /**
     * Does row operations on the matrix, recording the steps to recorder
    */
    void toRREF(StepsHistory& recorder);
};
#endif