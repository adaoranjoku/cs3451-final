/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 14 May 2019
*/

#pragma once

#include <vector>

// Base class to represent different parametric modules
class Module
{
private:
	char l;
	std::vector<float> p;
public:
	// Constructors
	Module(char letter);
	Module(char letter, std::vector<float> params);
	// Getters
	char letter(void) const;
	float param(int in) const;
};