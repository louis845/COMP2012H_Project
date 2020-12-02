#include "steps/StepsHistory.h"

StepsNode::StepsNode(){
    prev=nullptr;
    next=nullptr;
    step=nullptr;
}

StepsNode::~StepsNode(){
    delete step;
}

StepsHistory::StepsHistory(){
    current=front=end=nullptr;
    length=0;

    ans_rows=0;
    ans_cols=0;
    answer=nullptr;
}

StepsHistory::~StepsHistory(){
    StepsNode* node=front;
    while(node!=nullptr){
        StepsNode* next=node->next;
        delete node;
        node=next;
    }

    if(answer!=nullptr){
        for(int i=0;i<ans_rows;i++){
            delete[] answer[i];
        }
        delete[]answer;
    }
}

void StepsHistory::add_step(Step* s){
    ++length;
    if(front==nullptr){
        current=front=end=new StepsNode{};
        front->step=s;
    }else{
        StepsNode* newnode=new StepsNode{};
        end->next=newnode;
        newnode->prev=end;
        newnode->step=s;
        end=newnode;
    }
}

bool StepsHistory::has_current_node() const{
    return current!=nullptr;
}

int StepsHistory::get_length() const{
    return length;
}

const Step& StepsHistory::get_current_node() const{
    return *(current->step);
}

void StepsHistory::setAnswer(RF** matrix,int rows,int cols){
    if(answer!=nullptr){
        for(int i=0;i<ans_rows;i++){
            delete[] answer[i];
        }
        delete[] answer;
    }
    answer=matrix;
    ans_rows=rows;
    ans_cols=cols;
}

void StepsHistory::getAnswer(R**& ans, int& rows, int& cols){
    if(answer!=nullptr){
        rows=ans_rows;
        cols=ans_cols;
        ans=new R*[rows];
        for(int i=0;i<rows;i++){
            ans[i]=R::array_copy(answer[i],cols);
        }
    }else{
        ans=nullptr;
        rows=-1;
        cols=-1;
    }
}

void StepsHistory::next_node(){
    if(current->next!=nullptr){
        current=current->next;
    }
}

void StepsHistory::previous_node(){
    if(current->prev!=nullptr){
        current=current->prev;
    }
}


void StepsHistory::last_node(){
    if(end!=nullptr){
        current=end;
    }
}