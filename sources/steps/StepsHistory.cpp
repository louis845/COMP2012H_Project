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
}

StepsHistory::~StepsHistory(){
    StepsNode* node=front;
    while(node!=nullptr){
        StepsNode* next=node->next;
        delete node;
        node=next;
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
