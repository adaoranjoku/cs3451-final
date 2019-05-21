
/*
    Oliver Levy
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#include <iostream>
#include "TurtleInterpreter.h"

TurtleInterpreter::TurtleInterpreter(){
    worldPos = glm::mat4(1.0f);
    strings = std::list<Module>(); 
    matrices  = std::list<glm::mat4>();
    lengths = std::list<float>();
    poses = std::stack<glm::mat4>();

}

TurtleInterpreter::TurtleInterpreter(std::list<Module> input){

    worldPos = glm::mat4(1.0f); 
    strings = input;  
    matrices  = std::list<glm::mat4>();
    lengths = std::list<float>();
    poses = std::stack<glm::mat4>();

}

std::pair<std::list<glm::mat4>,std::list<float>> TurtleInterpreter::readList(){
    for(auto node = strings.begin(); node != strings.end(); ++node){
       TurtleInterpreter::readModule(*node); 
    }
    std::pair<std::list<glm::mat4>,std::list<float>> return_pair(matrices,lengths); 
    return return_pair;

}

void TurtleInterpreter::readModule(Module& module){
    switch(module.letter()){
        case 'A': std::cout<<"in A "<<module.letter()<<" "<<module.param(0)<<" "<<module.param(1)<<std::endl;       
            TurtleInterpreter::buildRotation(module.param(0),module.param(1)); 
            
            break;       
        case 'B': std::cout<<"in B "<< module.letter()<<" "<<module.param(0)<<std::endl;       
            TurtleInterpreter::buildTranslation(module.param(0)); 
            break;
        case '[': poses.push(worldPos); break;
        case ']': worldPos = poses.top(); poses.pop(); break;
    }

}

void TurtleInterpreter::buildRotation(float degreesTheta, float degreesPsy){
    //glm::quat myquaternion = glm::quat(glm::vec3(angle.x, , angle.z)); 
    
    std::cout<<"IN ROTATION"<<std::endl;
    TurtleInterpreter::printWorld(worldPos);            
    float d = 60;
    worldPos=glm::rotate(worldPos,d,glm::vec3(0,0,1));
    std::cout<<"after rotation 1"<<std::endl;

    TurtleInterpreter::printWorld(worldPos);            
    //degreesPsy
    worldPos=glm::rotate(worldPos,d,glm::vec3(1,0,0));
    std::cout<<"after rotation 2"<<std::endl;

    TurtleInterpreter::printWorld(worldPos);            
}

void TurtleInterpreter::buildTranslation(int length){


    std::cout<<"IN TRANSLATE"<<std::endl;

    std::cout<<"before translate, pushed to list"<<std::endl;
    TurtleInterpreter::printWorld(worldPos);            
    matrices.push_back(worldPos);
    lengths.push_back((float)length);


    std::cout<<"after translate, not yet pushed to stack"<<std::endl;
    TurtleInterpreter::printWorld(worldPos);            

    worldPos = glm::translate(worldPos, glm::vec3(0.0f,length,0.0f));

}

void TurtleInterpreter::printWorld(glm::mat4 input){
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            std::cout<<input[j][i]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;

}
 
void TurtleInterpreter::printCur(){
    for(auto matrix:matrices){
        TurtleInterpreter::printWorld(matrix);            
    }

}
