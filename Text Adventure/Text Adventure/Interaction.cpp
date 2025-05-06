#include "Interaction.h"
#include "Inventory.h"
#include "Item.h"
#include <iostream>
#include <vector>

Interaction::Interaction(string i_name, string i_description, string i_effectText)
{
	name = i_name;
	description = i_description;
	requiredItem;
	effectText = i_effectText;

}

string Interaction::getDescription()
{
	return description;
}

string Interaction::getRequiredItems()
{
	string itemsList;

	for (int i = 0; i < requiredItem.size(); i++)
	{
		itemsList += requiredItem.at((i - 1))->getName() + ", ";
	}
	if (!itemsList.empty())
	{
		itemsList = itemsList.substr(0, itemsList.size() - 2); //removes comma and space
	}

	return itemsList;
}

string Interaction::getEffectText()
{
	return effectText;
}

void Interaction::setDescription(string i_description)
{
	description = i_description;
}

void Interaction::addRequiredItem(Item* i_requiredItem)
{
	requiredItem.push_back(i_requiredItem);
}

void Interaction::setEffectText(string i_effectText)
{
	effectText = i_effectText;
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
