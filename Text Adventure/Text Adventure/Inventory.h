#pragma once
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

class Inventory
{
private:
	vector<Item*> items;
	int totalSlots;
	int usedSlots;
	int itemIndex;

public:
	Inventory();

	void addItem(Item* i_item);
	void discardItem(Item* i_item); //used in inventory when player chooses to discard, prompts with quesions
	void removeItem(Item* i_item); // used in funtions, solely to remove it from a vector
	bool hasItem(Item* i_item);

	vector<Item*>& getItems();
	string outputInventory();
	string outputDescription(int i_itemIndex);

};

