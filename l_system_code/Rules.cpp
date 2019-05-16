#include "Rules.h"

Rules::Rules(std::vector<Rule> rules)
{
	_rules = rules;
}

Rules::Rules()
{
	_rules = std::vector<Rule>();
}

void Rules::addRule(Rule rule)
{
	_rules.push_back(rule);
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

LinkedList<Module> NullRules::parse(Module m) const
{
	auto list = LinkedList<Module>();
	list.insertNode(m);
	return list;
}
