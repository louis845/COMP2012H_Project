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

    virtual std::string get_html_latex() const=0;
};

/**
 * Simple step for a single line of text. Can store a string of HTML for display onto the GUI WebEngine.
*/
class StepText : public Step{
public:
    const std::string text,latex_text;

    StepText(const std::string& text, const std::string& latex_text);

    StepText(const std::string& text);

    void print_to_console() const override;

    std::string get_html_latex() const;
};

#endif