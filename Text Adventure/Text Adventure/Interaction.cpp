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

string Interaction::getDescription()
{
	return description;
}

string Interaction::getChoices()
{
	string choicesList;
	cout << "What do you do? \n";
	cout << "\n";

	for (int i = 0; i < choices.size(); i++)
	{
		choicesList += "[" + to_string(i) + "] " + choices[i].text + "\n";
	}
	if (getRequiredItems().length() > 0)
	{
		choicesList += "[" + to_string(choices.size() + 1) + "] Use item... \n";
	}
	if (!choicesList.empty())
	{
		choicesList = choicesList.substr(0, choicesList.size() - 2); //removes comma and space
	}

	return choicesList;
}

string Interaction::getRequiredItems()
{
	string itemsList;

	for (int i = 0; i < requiredItem.size(); i++)
	{
		if (Inventory().hasItem(requiredItem[i]))
		{
			itemsList += requiredItem.at((i - 1))->getName() + ", ";
		}

	}

	if (!itemsList.empty())
	{
		itemsList = itemsList.substr(0, itemsList.size() - 2); //removes comma and space
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
