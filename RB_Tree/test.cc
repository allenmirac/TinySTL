#include "RB_Tree.h"
#include <iostream>
using namespace std;
int main(){
    RB_Tree<int> rb(11);
    rb.Insert_Node(123);
    rb.Insert_Node(124);
    rb.Insert_Node(125);
    rb.display();
    return 0;
}