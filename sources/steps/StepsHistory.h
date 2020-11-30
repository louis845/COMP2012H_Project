#ifndef STEPS_STEPSHISTORY
#define STEPS_STEPSHISTORY
#include "steps/Step.h"
#include "math/R.h"

/**
 * Linked list of steps,.
*/
class StepsNode{
private:
    StepsNode *prev, *next;
    Step* step;
    StepsNode();

    ~StepsNode();

    StepsNode(const StepsNode&) = delete;

    StepsNode(StepsNode&&) = delete;

    StepsNode& operator=(const StepsNode&) = delete;

    StepsNode& operator=(StepsNode&&) = delete;

    friend class StepsHistory;
};

class StepsHistory{
private:
    StepsNode *front, *end, *current;
    /**
     * Current denotes the viewing node.
    */
    int length;

    RF** answer;
    int ans_rows;
    int ans_cols;
public:
    StepsHistory();

    ~StepsHistory();

    StepsHistory(const StepsHistory&) = delete;

    StepsHistory(StepsHistory&&) = delete;

    StepsHistory& operator=(const StepsHistory&) = delete;

    StepsHistory& operator=(StepsHistory&&) = delete;

    /**
     * Adds a step, does not copy/allocate the step.
    */
    void add_step(Step* s);

    bool has_current_node() const;

    int get_length() const;

    const Step& get_current_node() const;

    /**
     * Go to the next node
    */
    void next_node();

    /**
     * Go to the previous node
    */
    void previous_node();

    /**
     * Directly saves the pointer internally to StepsHistory. Does not perform copy. The previous answer, if exists, will be deallocated.
    */
    void setAnswer(RF** ans, int rows, int cols);

    /**
     * Allocates and returns a new R** matrix respresenting the result, and passes the rows and cols onto the references.
     * result would be nullptr if there is none. For example linear equations with no solution.
    */
    void getAnswer(R** &result, int& rows, int& cols);
};
#endif