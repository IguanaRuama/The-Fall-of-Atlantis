#include "Inventory.h"
#include "Item.h"
#include <string>
#include <vector>
#include <iostream>

Inventory::Inventory()
{
	items = {};
	totalSlots = 5;
	usedSlots = 0;
	itemIndex = 0;
}



void Inventory::takeItemChoice(Item* i_item)
{
	cout << "Do you wish to pick up the " << i_item->getName() << "?	(y/n)\n";
	string input;

	cin >> input;

	if (input == "y" || input == "Y")
	{
		addItem(i_item);
	}
	else if (input == "n" || input == "N")
	{
		cout << "You leave the " << i_item->getName() << " where you found it.\n";
	}
	else
	{
		cout << "Invalid input, try again.\n";
		takeItemChoice(i_item);
	}
}

void Inventory::addItem(Item* i_item)
{
	if (totalSlots == usedSlots) 
	{
		cout << "Your bag is full! Do you wish to swap an item or leave " << i_item->getName() << " behind? (swap/leave)" << endl;
		string input;
		cin >> input;
		if (input == "swap" || input == "Swap")
		{
			removeItem(i_item);
			items.push_back(i_item);

			cout << i_item->getName() << " has taken it's spot!" << endl;
		}
		else if (input == "leave" || input == "Leave")
		{
			cout<< i_item->getName() << " remains where you found them..." << endl;
		}
	}
	else
	{
		items.push_back(i_item);

		cout << "You stash away the " << i_item->getName() << " in your bag." << endl;
		usedSlots++;

	}

}

void Inventory::discardItem(Item* i_item)
{
	bool itemRemoved = false;

	cout << "What item would you like to leave behind?" << endl;
	string input;
	cin >> input;

	for (int i = 0; i < (items.size()); i++)
	{
		if (items[i]->getName() == input)
		{
			items.erase(items.begin() + i);

			cout << input << " has been left behind..." << endl;
			itemRemoved = true;
			usedSlots++;
		}
	}
	
	if (!itemRemoved)
	{
		cout << "You rummage in your bag, yet you cannot find " << input << "." << endl;
		cout << "Try again? (y/n)" << endl;
		cin >> input;

		if (input == "y")
		{
			discardItem(i_item);
		}
		else if (input != "n")
		{
			cout << "Invalid Input" << endl;
		}
	}

	//add error check

}

void Inventory::removeItem(Item* i_item)
{
	auto it = find(items.begin(), items.end(), i_item);
	if (it != items.end())
	{
		items.erase(it);
		usedSlots--;
	}
}


bool Inventory::hasItem(Item* i_item)
{
	for (int i = 0; i < items.size(); i++)
	{
		if(i_item->getName() == items[i]->getName())
		{
			return true;
		}
	}
	return false;
}

vector<Item*>& Inventory::getItems()
{
	return items;
}

string Inventory::outputInventory()
{
	string output = "============================================================================================================================= \n";
	output += "Inventory: \n";

	for (int i = 0; i < items.size(); i++)
	{
		output += outputDescription(i) + "\n";
	}

	output += "============================================================================================================================= \n";

	return output;
}

string Inventory::outputDescription(int i_itemIndex)
{
	string output = items[i_itemIndex]->getName() + ": " + items[i_itemIndex]->getDescription();
	return output;
}


