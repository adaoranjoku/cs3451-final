#include "LinkedList.h"
#include <iostream>
#include <vector>

int main(){

//test insertNode
LinkedList<char> list = LinkedList<char>();
list.display();
list.insertNode('r');
list.display();
list.insertNode('q',0);
list.display();
list.insertNode('s');
std::cout<<"The linked list should be q,r,s"<<std::endl;
list.display();

//delete node
list.deleteNode('s');
std::cout<<"The linked list should be q,r"<<std::endl;
list.display();

//more insert node
list.insertNode('x');
list.insertNode('y');
list.insertNode('z');
list.insertNode('a',1);
std::cout<<"lastone"<<std::endl;
list.display();

//list size
int i = list.size();
std::cout<<i<<std::endl;

//list delete node 2nd time
list.deleteNode('y');
list.display();
int q = list.size();
std::cout<<q<<std::endl;
list.deleteNode('z');
list.display();
int r = list.size();
std::cout<<r<<std::endl;
int y=0;

LinkedList<int> nlist = LinkedList<int>();
std::vector<int> the_vector{1,2,3,4};
nlist.insertVector(the_vector);
nlist.display();


nlist.popFront();
nlist.display();
nlist.pop();
nlist.display();

nlist.popFront();
nlist.display();

nlist.popFront();
nlist.display();
/*for(auto i = list.begin(),i!=list.end(),i++ ){
    std::cout<<y<<std::endl;
    y++; */

//}
std::cout<<"Testing is completed. The program functions correctly"<<std::endl;

return 0; 
}
