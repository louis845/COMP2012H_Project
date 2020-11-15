#ifndef LINEAR_COUPLEDOPS
#define LINEAR_COUPLEDOPS

#include "math/linear/LinearOperations.h"
#include "steps/StepsHistory.h"

class CoupledOperations : public LinearOperations{
private:
    RF*** couple;
    int couple_rows;
    int couple_cols;

    RF& A(const int&, const int&) const;
protected:
    virtual void row_add(int from, int to, const RF& mult) override;
    virtual void row_swap(int i, int j) override;
    virtual void row_multiply(int row, const RF& mult) override;
public:
    /**
     *  When no transpose the matrices are placed in order (matrix, couple). When transpose it is placed
     * (matrix
     * couple)
     * This is to enable the operations to act on both. It is assumed the row or col of couple matches matrix. Only need to give couple_dim.
     * The RF** couple will not be deallocated. For the other arguments see LinearOperations
    */
    CoupledOperations(RF** matrix, const int& rows, const int& cols, bool transpose, LinOpsRecorder* recorder, RF** couple, int couple_dim);
    ~CoupledOperations();
    
    const int& get_rows() const;
    const int& get_cols() const;
    const RF& get_element(int row, int col) const;
};
#endif