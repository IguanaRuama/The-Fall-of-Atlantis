#pragma once
#include <string>
#include <vector>
#include "Inventory.h"
#include "Item.h"

using namespace std;

class Interaction
{
private:
	string name;
	string description;
	vector<Item*>requiredItem;
	string effectText;

public:
	Interaction(string i_name, string i_description, string i_effectText);

	string getDescription();
	string getRequiredItems();
	string getEffectText();
	
	void setDescription(string i_description);
	void addRequiredItem(Item* i_requiredItem);
	void setEffectText(string i_effectText);

	bool isAvailable(Inventory i_inventory);
};

