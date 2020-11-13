#include "steps/Step.h"
#include <iostream>

using namespace std;
Step::Step(){

}

Step::~Step(){

}

StepText::StepText(const string& s) : text(s){

}

void StepText::print_to_console() const{
    for(int i=0;i<100;i++){
        cout<<"\n";
    }
    cout<<text<<"\n";
}