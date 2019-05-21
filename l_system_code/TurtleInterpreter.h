
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
    glm::mat4 curRotate;


    std::list<Module> strings;
    std::stack<glm::mat4> poses;
  

    void readModule(Module& module);
    void buildRotation(float degreesTheta, float degreesPsy);
    void buildTranslation(int length);
    void printWorld(glm::mat4 input);

public:

    struct returnVar{ 
        std::list<glm::mat4> rotations;
        std::list<glm::mat4> world_transforms;
        std::list<float> lengths; 
    }ret;

    TurtleInterpreter();
    TurtleInterpreter(std::list<Module> input);

    returnVar readList();
    void printCur();    

};
