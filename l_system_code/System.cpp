/*
	Naman Goyal
	CS 77 Midterm Project on L Systems
	Created 15 May 2019
*/

#include "System.h"
#include <sstream>

LinkedList<Module> System::string_to_Modules(std::string st)
{
	//Make an alphabet string
	std::string alphabet;
	for (auto p : _alphabet) {
		alphabet += p.first;
	}

	int last_index = 0;
	std::size_t found;
	auto list = LinkedList<Module>();
	//iterate through the string
	while ((found = st.find(alphabet, last_index + 1)) != std::string::npos) {
		list.insertNode(one_Module(st.substr(last_index, found - last_index)));
		last_index = found;
	}
	return list;
}

Module System::one_Module(std::string st)
{
	char letter = st[0];
	std::vector<float> params(_alphabet[letter]);
	std::stringstream ss(st);

	for (int i = 0; i < _alphabet[letter]; i++) {
		ss >> params[i];
	}

	return Module(letter, params);
}

void System::iterate()
{
	_n++;

	auto temp_list = LinkedList<Module>();
	while (_curr_state.size != 0) {
		auto module = _curr_state.popFront();
		auto rules = _productions[module.letter()];
		auto result = rules.parse(module);

		//add from result to back of temp_list
	}

	_curr_state = temp_list;
}

void System::reset()
{
	_n = 0;
	_curr_state = _axiom;
}

System::System(std::map<char, int> alphabet, std::string axiom, std::map<char, Rules> productions)
{
	_alphabet = alphabet;
	_productions = productions;
	_n = 0;

	_axiom = string_to_Modules(axiom);
	_curr_state = _axiom;
}

LinkedList<Module> System::currentSystem(void) const
{
	return _curr_state;
}

void System::nextSystem(void)
{
	iterate();
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
	auto copy = _curr_state;
	while (copy.size() != 0) {
		Module curr_module = copy.popFront();

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
