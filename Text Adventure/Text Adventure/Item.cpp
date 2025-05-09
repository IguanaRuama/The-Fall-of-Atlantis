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
		output += "[" + to_string(i) + "] " + action + "\n";
	}
	output += "============================================================================================================================= \n";

	return output;
	
}

int Item::getCost()
{
	return cost;
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
	actions = { "Examine", "Discard" };
}

void Item::setCost(int i_cost)
{
	cost = i_cost;
}

void Item::addAction(string i_action)
{
	actions.push_back(i_action);
}

void Item::removeAction(string i_action)
{
	auto it = find(actions.begin(), actions.end(), i_action);
	if (it != actions.end())
	{
		actions.erase(it);
	}

}
