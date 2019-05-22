#include "Rules.h"

Rules::Rules(std::vector<std::shared_ptr<Rule>> rules)
{
	_rules = rules;
}

Rules::Rules()
{
	_rules = std::vector<std::shared_ptr<Rule>>();
}

void Rules::addRule(Rule* rule)
{
	auto ptr = std::shared_ptr<Rule>(rule);
	_rules.push_back(ptr);
}

std::list<Module> Rules::parse(Module m) const
{
	for (auto& rule : _rules) {
		//Get first satisfied rule
		if (rule->satisfied(m)) {
			return rule->parse(m);
		}
	}
	//Default to returning this module
	return std::list<Module>(1, m);
}

std::list<Module> NullRules::parse(Module m) const
{
	return std::list<Module>(1, m);
}
