#include "Rules.h"

Rules::Rules(std::vector<Rule> rules)
{
	_rules = rules;
}

LinkedList<Module> Rules::parse(Module m) const
{
	for (auto rule : _rules) {
		//Get first satisfied rule
		if (rule.satisfied(m)) {
			return rule.parse(m);
		}
	}
	//Default to returning this module
	auto list = LinkedList<Module>();
	list.insertNode(m);
	return list;
}
