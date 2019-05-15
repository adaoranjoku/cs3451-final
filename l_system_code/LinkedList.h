/*

    Oliver Levy
    CS77 - Technical Project
    Created 14 May 2019

*/


#include <iostream>

template <class T> const_iterator;

template <class T>
struct node
{
    T obj;
    node *next;

};


template <class T>
class LinkedList
{
    typedef T * iterator;
    typedef const T * const_iterator;
    private:
        node<T> *startNode;
        node<T> *tailNode;         
        friend class const_iterator<T>;

    public:
        //Constructor: takes data of type T
        LinkedList(){
           startNode = NULL;
           tailNode = NULL; 
        }
        // destructor?

        void insertNode(T t, int pos = -1){
            node<T> *theNode = new node<T>;
            theNode->obj =t;
            theNode->next=NULL;
            if(startNode == NULL){
                startNode = theNode;
                tailNode = theNode;
                theNode = NULL;
            }    
            else if(pos == -1){
                tailNode->next = theNode;
                tailNode = theNode;
            }
            else{
                if(pos == 0){
                    theNode->next=startNode;
                    startNode = theNode;
                }
                else{
                    int i = 1;
                    node<T> *pNode = new node<T>;
                    pNode = startNode; 
                    while(i<pos){
                        pNode = pNode->next;
                        i += 1;
                    } 
                    theNode->next = pNode->next;
                    pNode->next = theNode;
                    pNode=NULL;
                    delete pNode;
                }
            }
        }

        int size(){
            int i = 0;

            node<T> *pNode = new node<T>;
            pNode = startNode;

            while(pNode != NULL){
                i++;
                pNode = pNode->next;          
            }
            return i;
        }


        void deleteNode(T t){
            node<T> *pNode = new node<T>;
            node<T> *cNode = new node<T>;
            pNode = startNode;
            
            cNode = startNode;
            while(pNode != NULL){
                if(pNode->obj == t && pNode==startNode){
                    startNode = pNode->next;
                    pNode->next = NULL;
                    
                }
                else if(pNode->obj == t && pNode==tailNode){
                    cNode->next = NULL;
                    tailNode=cNode;
                }
                else if(pNode->obj == t){
                    cNode->next = pNode->next;
                    pNode = pNode->next;
                }   
                cNode=pNode;
                pNode = pNode->next;
            }
            delete pNode;
        }

        iterator begin(){return startNode;}
        iterator end(){return tailNode;}


        void display(){
            node<T> *pNode = new node<T>; 
            pNode = startNode;
            while(pNode != NULL){
    
                std::cout << pNode->obj<<" ";
                pNode = pNode->next;
            }
            std::cout<<std::endl;
            delete pNode;
        }


};


template <class T>
class const_iterator{


};
