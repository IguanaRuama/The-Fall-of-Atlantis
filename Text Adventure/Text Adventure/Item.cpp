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

string Item::getActions()
{
	string output = "============================================================================================================================= \n";
	for (int i = 0; actions.size(); i++)
	{
		output += "[" + to_string(i) + "] " + actions[i].text + "\n";
	}
	output += "============================================================================================================================= \n";

	return output;
	
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

void Item::setEffect(ItemAction i_action, string i_effect)
{ 
	actions.emplace_back(i_action.text, i_effect);
}

void Item::addAction(string i_name, string i_effect)
{
	actions.emplace_back(i_name, i_effect);
}

void Item::removeAction(int i_aIndex)
{
	if (i_aIndex >= 0 && i_aIndex < actions.size())
	{
		actions.erase(actions.begin() + i_aIndex);
	}
}
