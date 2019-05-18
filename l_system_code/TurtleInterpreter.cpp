
/*
    Oliver Levy
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Module.h"
#include "TurtleInterpreter.h"
#include <list>
#include <stack>
#include <glm/glm.hpp>


TurtleInterpreter::TurtleInterpreter(){
    worldPos = glm::mat4(1.0f);
    strings = new std::list<Module>(); 
    matrices  = new std::list<glm::mat4>();
    poses = new std::stack<glm::mat4>();
}

TurtleInterpreter::TurtleInterpreter(std::list<glm::mat4> input){

    worldPos = glm::mat4(1.0f); 
    strings = input;  
    matrices  = new std::list<glm::mat4>();
    poses = new std::stack<glm::mat4>();

}

std::list<glm::mat4> TurtleInterpreter::readList(){
    for(auto node = strings.begin(); node != strings.end(); ++node){
       TurtleInterpreter::readModule(*node); 
    }
    return matrices;

}

void TurtleInterpreter::readModule(Module& module){
    switch(module.letter()){
        case 'A': TurtleInterpreter::buildRotation(module.param(0),module.param(1)); break;       
        case 'B': TurtleInterpreter::buildTranslation(module.param(0)); break;
        case '[': poses.push(worldPos); break;
        case ']': worldPos = poses.top(); poses.pop(); break;
    }

}

void TurtleInterpreter::buildRotation(int degreesTheta, int degreesPsy){
    worldPos=glm::rotate(worldPos,degreesTheta, worldPos[2]);
    worldPos=glm::rotate(worldPos,degreesPsy, worldPos[1]);
}

void TurtleInterpreter::buildTranslation(int length){
    worldPos = glm::translate(worldPos, glm::vec3(0.0f,0.0f,length));
    matrices.push_back(worldPos);
}
 

