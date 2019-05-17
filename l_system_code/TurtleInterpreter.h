
/*
    Oliver Levy
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include "Module.h"
#include <List>
#include <stack>
#include <glm/glm.hpp>

class TurtleInterpreter
{

    glm::mat4 worldPos;
    std::list<glm::mat4> matrices;
    std::list<Module> strings;
    std::stack<glm::mat4> poses;
    

public:

    TurtleInterpreter();
    TurtleInterpreter(std::list<Module> input);

    std::list<glm::mat4> readList();
    void readModule(Module& module);
    void buildRotation(int degreesPhi, int degreesPsy);
    void buildTranslation(int length);
    

};
