#include "Item.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

Item::Item(string i_name, string i_description, int i_cost)
{
	name = i_name;
	description = i_description;
	cost = i_cost;
}

string Item::getName()
{
	return name;
}

string Item::getDescription()
{
	return description;
}

vector<ItemAction> Item::getActions()
{
	return actions;
}

int Item::getCost()
{
	return cost;
}

string Item::getEffect(int i_eIndex)
{
	if (i_eIndex >= 0 && i_eIndex < actions.size()) {
		return actions[i_eIndex].effectText;
	}
	return "Invalid choice.";
}

function<void()> Item::getFunction(int i_cIndex)
{
	return actions[i_cIndex].effectFunction;
}

void Item::runFunction(function<void()> i_function)
{
	i_function();
}

void Item::setName(string i_name)
{
	name = i_name;
}

void Item::setDescription(string i_description)
{
	description = i_description;
}

void Item::setDefaultActions()
{
	actions = {
		ItemAction("Examine", "You take a closer look."),
		ItemAction("Discard", "You discard the item.")
	};
}

void Item::setCost(int i_cost)
{
	cost = i_cost;
}

void Item::setEffectAndFunction(ItemAction i_action, string i_effect, function<void()> i_function)
{ 
	actions.emplace_back(i_action.text, i_effect, i_function);
}

void Item::outputActions()
{
	for (int i = 0; i < actions.size(); i++)
	{
		cout << "[" << to_string(i) << "] " << actions[i].text << "\n";
	}
}

void Item::addAction(string i_name, string i_effect, function<void()> i_function)
{
	actions.emplace_back(i_name, i_effect, i_function);
}

void Item::removeAction(int i_aIndex)
{
	if (i_aIndex >= 0 && i_aIndex < actions.size())
	{
		actions.erase(actions.begin() + i_aIndex);
	}
}
