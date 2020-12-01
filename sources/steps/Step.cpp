#include "steps/Step.h"
#include <iostream>

using namespace std;
Step::Step(){

}

Step::~Step(){

}

StepText::StepText(const string& s, const string& ls) : text(s), latex_text(ls){

}

StepText::StepText(const string& s) : StepText(s,s){
    
}

void StepText::print_to_console() const{
    for(int i=0;i<100;i++){
        cout<<"\n";
    }
    cout<<text<<"\n";
}

string StepText::get_html_latex() const{
    return latex_text;
}