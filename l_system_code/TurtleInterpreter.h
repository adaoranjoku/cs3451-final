
/*
    Oliver Levy
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include <utility>
#include "Module.h"
#include <list>
#include <stack>
#include "Common.h"
#include "SceneGraph.h"
#include "OpenGLMesh.h"

class TurtleInterpreter
{

    glm::mat4 worldPos;
    std::list<Module> strings;
    std::stack<glm::mat4> poses;
   
    std::list<glm::mat4> matrices;
    std::list<float> lengths; 

public:

    TurtleInterpreter();
    TurtleInterpreter(std::list<Module> input);

    std::pair<std::list<glm::mat4>,std::list<float>> readList();
    void readModule(Module& module);
    void buildRotation(float degreesTheta, float degreesPsy);
    void buildTranslation(int length);
    void printCur();    
    void printWorld(glm::mat4 input);

};
