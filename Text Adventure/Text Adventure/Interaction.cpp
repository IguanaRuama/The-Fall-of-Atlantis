#include "Interaction.h"
#include "Inventory.h"
#include "Item.h"
#include <iostream>
#include <vector>

Interaction::Interaction(string i_name, string i_description)
{
	name = i_name;
	description = i_description;
	requiredItem;
	choices;
}

string Interaction::getName()
{
	return name;
}

string Interaction::getDescription()
{
	return description;
}

string Interaction::outputChoices()
{
	string choicesList;
	cout << "What do you do? \n";
	cout << "\n";

	for (int i = 0; i < choices.size(); i++)
	{
		choicesList += "[" + to_string(i) + "] " + choices[i].text + "\n";
	}
	if (getRequiredItems().size() > 0)
	{
		for (int i = 0; i < getRequiredItems().size(); i++)
		{
			choicesList += "[" + to_string(i + choices.size()) + "] " + " Use the " + getRequiredItems()[i]->getName() + "\n";
		}
	}
	if (!choicesList.empty())
	{
		choicesList = choicesList.substr(0, choicesList.size() - 2); //removes comma and space
	}

	return choicesList;
}

vector<Item*>& Interaction::getRequiredItems()
{
	vector<Item*> itemsList;

	for (int i = 0; i < requiredItem.size(); i++)
	{
		if (Inventory().hasItem(requiredItem[i]))
		{
			itemsList.push_back(requiredItem[i]);
		}
	}

	return itemsList;
}

string Interaction::getEffect(int i_cIndex)
{
	if (i_cIndex >= 0 && i_cIndex < choices.size()) {
		return choices[i_cIndex].effectText;
	}
	return "Invalid choice.";
}

function<void()> Interaction::getFunction(int i_cIndex)
{
	return choices[i_cIndex].effectFunction;
}

void Interaction::setDescription(string i_description)
{
	description = i_description;
}

void Interaction::setRequiredItem(Item* i_requiredItem)
{
	requiredItem.push_back(i_requiredItem);
}

void Interaction::setEffect(Choice i_choice, string i_effect)
{
	choices.emplace_back(i_choice.text, i_effect);
}

void Interaction::runFunction(function<void()> i_function)
{
	i_function();
}

void Interaction::addChoice(string i_text, string i_effect, function<void()>i_function)
{
	choices.emplace_back(i_text, i_effect, i_function);
}

void Interaction::removeChoice(int i_cIndex)
{
	if (i_cIndex >= 0 && i_cIndex < choices.size())
	{
		choices.erase(choices.begin() + i_cIndex);
	}
}

bool Interaction::isAvailable(Inventory i_inventory)
{
	for (int i = 0; i < requiredItem.size(); i++)
	{
		if (i_inventory.hasItem(requiredItem.at(i - 1)))
		{
			return true;
		}
	}
	return false;
}
