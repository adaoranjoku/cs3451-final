
/*
    Oliver Levy
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#include <iostream>
#include <math.h>
#include "TurtleInterpreter.h"

//CONSTRUCTOR: empty
TurtleInterpreter::TurtleInterpreter(){
    prevLength = 0;
    worldPos = glm::mat4(1.0f);
    curRotate= glm::mat4(1.0f);
    
    strings = std::list<Module>(); 
    poses = std::stack<glm::mat4>();
    world_transforms=std::list<glm::mat4>();
    //poses = std::stack<TurtleInterpreter::innerValues>

}

//CONSTRUCTOR: with list of modules as input
TurtleInterpreter::TurtleInterpreter(std::list<Module> input){

    prevLength = 0;
    worldPos = glm::mat4(1.0f);
    curRotate= glm::mat4(1.0f);

    strings = input;  
    poses = std::stack<glm::mat4>();
    world_transforms=std::list<glm::mat4>();
}

/*
Goes through list of modules and calls readModule for each one 
Returns a struct with the rotation values, lengths and transforms for each branch.
*/
std::list<glm::mat4> TurtleInterpreter::readList(){
    for(auto node = strings.begin(); node != strings.end(); ++node){
       TurtleInterpreter::readModule(*node); 
    }
    return world_transforms;

}

/*
Reads the letter for a module and calls the corresponding update function
*/
void TurtleInterpreter::readModule(Module& module){
    switch(module.letter()){
        //if A, call rotate
        case 'A': std::cout<<"in A "<<module.letter()<<" "<<module.param(0)<<" "<<module.param(1)<<std::endl;       
            TurtleInterpreter::buildRotation(module.param(0),module.param(1));             
            break;       
        //if B, call translate
        case 'B': std::cout<<"in B "<< module.letter()<<" "<<module.param(0)<<std::endl;       
            TurtleInterpreter::buildTranslation(module.param(0)); 
            break;

        //if [, save current state on the stack
        case '[': poses.push(worldPos); break;
    
        //if ], pop the last state from the stack
        case ']': worldPos = poses.top(); poses.pop(); break;
    }

}

/*
Transform input parameters into degrees values.  
Rotate the current rotation matrix on x and z axis.

*/
void TurtleInterpreter::buildRotation(float theta, float psy){
    theta=60;
    psy=60;
    float rotation[16] ={
        cos(theta)*cos(psy),    -1*sin(theta),   sin(psy),  0, //x = v[0]
        sin(theta),             cos(theta),      0,         0, //y = v[1]      
        sin(psy),               0,               cos(psy),  0, //z = v[2]
        0,                      0,               0,         0, //0 = v[3] 
        }; 
    curRotate = glm::make_mat4(rotation);

/* 
    std::cout<<"IN ROTATION"<<std::endl;
    TurtleInterpreter::printWorld(worldPos);            

    float d = 60;

    curRotate=glm::rotate(curRotate,d,glm::vec3(0,0,1));

    std::cout<<"after rotation 1"<<std::endl;
    TurtleInterpreter::printWorld(curRotate);            

    curRotate=glm::rotate(curRotate,d,glm::vec3(1,0,0));

    std::cout<<"after rotation 2"<<std::endl;
    TurtleInterpreter::printWorld(curRotate);            
*/
}
/*
Create a branch by pushing a new set of transform, length and rotation values to their respective lists.

Rotation: The value of rotation is the rotation of the world transform mastrix (worldPos) with the lastest rotations set since the 
    last branch was set. Ex. B(4)A(7,7)B(5): the rotation value for B(5) is the worldPos value for B(4) along with rotation of A(7,7) -            represented by curRotate.
Length: The value of length is the parameter passed to the function.
worldPos: world transform of a segment in the coordinate space  of the last branch. Updated for the next branch with the current branch's          length as the transform and rotation.  
*/
void TurtleInterpreter::buildTranslation(int length){


    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1,length,1));
    glm::mat4 translate =  worldPos * glm::translate(glm::mat4(1.0f), glm::vec3(0,prevLength,0) );
    worldPos = translate * curRotate * scale;
    world_transforms.push_back(worldPos);
    prevLength=length;
  //  worldPos = worldPos * glm::inverse(scale);

    /*
    std::cout<<"IN TRANSLATE"<<std::endl;
    std::cout<<"before update, pushed to list"<<std::endl;
    TurtleInterpreter::printWorld(worldPos);            

    //set current branch's value
    ret.rotations.push_back(curRotate);
    ret.world_transforms.push_back(worldPos);
    ret.lengths.push_back((float)length);

    //set next values of worldPos
    worldPos = glm::translate(worldPos, glm::vec3(0.0f,((float)length)/2.0,0.0f));
    worldPos=glm::mat4(glm::vec4(curRotate[0]),glm::vec4(curRotate[1]),glm::vec4(curRotate[2]),glm::vec4(worldPos[3]));

    std::cout<<"after update, not yet pushed to list"<<std::endl;
    TurtleInterpreter::printWorld(worldPos);            
    */
}

/*
Prints the values of a 4x4 matrix
*/
void TurtleInterpreter::printWorld(glm::mat4 input){
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            std::cout<<input[j][i]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;

}
 
/*
Prints all of the matrices in the world_transforms list
*/
void TurtleInterpreter::printCur(){
    for(auto matrix:world_transforms){
        TurtleInterpreter::printWorld(matrix);            
    }

}
