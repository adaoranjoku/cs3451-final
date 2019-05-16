/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 15 May 2019
*/

#include "System.h"
#include <sstream>
#include <regex>

std::list<Module> System::string_to_Modules(std::string& st)
{
	//Make the regex
	std::stringstream pattern;
	for (auto p : _alphabet) {
		pattern << p.first;

		if (p.second == 0) { 
			pattern << "|";
			continue;
		}
		pattern << "\\(";
		for (int i = 0; i < p.second; i++) {
			pattern << "[-+]?[0-9]*\\.?[0-9]+,";
		}
		pattern.seekp(-1, pattern.cur);
		pattern << "\\)|";
	}
	std::regex rpattern = std::regex(pattern.str());

	//Iterate using the regex
	auto beginning = std::sregex_iterator(st.begin(), st.end(), rpattern);
	auto list = std::list<Module>();
	for (auto i = beginning; i != std::sregex_iterator(); i++) {
		std::string match = i->str();
		list.push_back(one_Module(match));
	}
	list.pop_back();
	return list;
}

Module System::one_Module(std::string& st)
{
	char letter = st[0];
	auto params = std::vector<float>();
	std::stringstream ss(st);

	if (_alphabet[letter] != 0) {
		ss.seekg(1, ss.cur);		// Move past the letter
	}

	for (int i = 0; i < _alphabet[letter]; i++) {
		ss.seekg(1, ss.cur);		// Move past the parens or commas
		float p;
		ss >> p;
		params.push_back(p);	
	}

	return Module(letter, params);
}

void System::iterate(void)
{
	_n++;

	auto temp_list = std::list<Module>();
	for (Module mod : _curr_state) {

		auto rules = _productions[mod.letter()];
		auto result = rules.parse(mod);

		temp_list.splice(temp_list.end(), result);
	}

	_curr_state = temp_list;
}

void System::reset()
{
	_n = 0;
	_curr_state = _axiom;
}

System::System(std::map<char, int>& alphabet, std::string& axiom, std::map<char, Rules>& productions)
{
	_alphabet = alphabet;
	_productions = productions;
	_n = 0;

	_axiom = string_to_Modules(axiom);
	_curr_state = _axiom;
}

std::list<Module> System::currentSystem(void) const
{
	return _curr_state;
}

void System::moveToIteration(int i)
{
	if (i < _n) {
		reset();
	}
	while (_n != i) {
		iterate();
	}
}

std::string System::toString(void) const
{
	std::stringstream ss;
	for (auto curr_module : _curr_state) {
		char letter = curr_module.letter();
		int param_num = _alphabet.find(letter)->second;

		ss << letter;

		if (param_num == 0) {
			continue;
		}

		ss << '(';

		for (int i = 0; i < _alphabet.find(letter)->second; i++) {
			ss << curr_module.param(i) << ",";
		}

		ss.seekp(-1, ss.cur);
		ss << ')';
	}

	return ss.str();
}
