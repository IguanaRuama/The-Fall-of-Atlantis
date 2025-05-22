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

	void takeItemChoice(Item* i_item);
	void addItem(Item* i_item);
	void discardItem(Item* i_item); //used in inventory when player chooses to discard, prompts with quesions
	void removeItem(Item* i_item); // used in funtions, solely to remove it from a vector
	bool hasItem(Item* i_item);

	vector<Item*>& getItems();
	void outputInventory(Inventory* i_playerInventory);
	string outputDescription(int i_itemIndex);

};

