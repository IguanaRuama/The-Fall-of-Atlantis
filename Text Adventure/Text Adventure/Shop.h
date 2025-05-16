#pragma once
#include <string>
#include <vector>
#include "Item.h"
#include "Inventory.h"
#include "Currency.h"

using namespace std;

class Shop
{
private:
	vector<Item*> stock;

public:
	Shop();

	void displayItems();
	void buyItem(Item* i_item, Inventory& i_inventory, Currency& i_money);
	void addItem(Item* i_item);
	void sellItem(int index, Inventory& i_inventory, Currency& i_money);
};

