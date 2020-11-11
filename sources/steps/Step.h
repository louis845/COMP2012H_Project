#ifndef STEPS_STEP_H
#define STEPS_STEP_H

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

#endif