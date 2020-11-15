#ifndef STEPS_STEP_H
#define STEPS_STEP_H

#include <string>
class Step{
public:
    Step();
    virtual ~Step();
    Step(const Step&) = delete;

    Step(Step&&) = delete;

    Step& operator=(const Step&) = delete;

    Step& operator=(Step&&) = delete;

    virtual void print_to_console() const=0;
};

/**
 * Simple step for a single line of text.
*/
class StepText : public Step{
public:
    const std::string text;
    StepText(const std::string& text);

    void print_to_console() const override;
};

#endif