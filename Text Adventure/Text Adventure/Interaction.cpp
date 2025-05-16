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

void Interaction::outputChoices(Inventory* i_inventory)
{
	cout << "What do you do? \n";
	cout << "\n";

	for (int i = 0; i < choices.size(); i++)
	{
		cout << "[" << to_string(i) << "] " << choices[i].text << "\n";
	}

	vector<Item*> usableItems = getRequiredItems(i_inventory);
	for (int i = 0; i < usableItems.size(); i++)
	{
		cout << "[" << to_string(i + choices.size()) << "] Use the " << usableItems[i]->getName() << "\n";
	}

}

vector<Item*> Interaction::getRequiredItems(Inventory* i_inventory)
{
	vector<Item*> itemsList;

	for (int i = 0; i < requiredItem.size(); i++)
	{
		if (i_inventory->hasItem(requiredItem[i]))
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

vector<Choice> Interaction::getChoices()
{
	return choices;
}

void Interaction::setDescription(string i_description)
{
	description = i_description;
}

void Interaction::setRequiredItem(Item* i_requiredItem)
{
	requiredItem.push_back(i_requiredItem);
}

void Interaction::setEffect(const string& text, const string& i_effect)
{
	auto it = std::find_if(choices.begin(), choices.end(), [&](const Choice& c)
		{
			return c.text == text;
		});

	if (it != choices.end())
	{
		choices.emplace_back(text, i_effect);
	}
}

void Interaction::runFunction(function<void()> i_function)
{
	i_function();
}

void Interaction::addChoice(string i_text, string i_effect, function<void()>i_function)
{
	choices.emplace_back(i_text, i_effect, i_function);
}


void Interaction::removeChoice(const string& text)
{
	auto it = std::find_if(choices.begin(), choices.end(), [&](const Choice& c) 
		{
			return c.text == text;
		});

	if (it != choices.end())
	{
		choices.erase(it);
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
