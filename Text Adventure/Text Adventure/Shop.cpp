#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Item.h"
#include "Inventory.h"
#include "Currency.h"
#include "Shop.h"

Shop::Shop()
{
	stock;
}

void Shop::displayItems()
{
	cout << " Stock: \n"
		"==================================================================================================================\n";
	for (int i = 0; i < stock.size(); ++i)
	{
		cout << "[" << (i) << "] " << stock[i]->getName()
			<< "  -  " << stock[i]->getDescription()
			<< "  -  " << stock[i]->getCost() << " gold \n";
	}
}

void Shop::buyItem(Item* i_item, Inventory& i_inventory, Currency& i_money)
{
	if (!i_item)
	{
		cout << "You hesitate—there’s nothing to buy." << endl;
		return;
	}

	if (i_money.getMoneyAmount() >= i_item->getCost())
	{
		i_money.removeMoney(i_item->getCost());
		i_inventory.addItem(i_item);
		cout << "You acquired: " << i_item->getName() << endl;
	}
	else
	{
		cout << "You scowl at your empty purse—- there’s not enough drachmae." << endl;
	}
}

void Shop::addItem(Item* i_item)
{
	stock.push_back(i_item);
}

void Shop::sellItem(int index, Inventory& i_inventory, Currency& i_money)
{
	vector<Item*>& inventoryItems = i_inventory.getItems();

	if (index < 0 || index > inventoryItems.size())
	{
		cout << "sm about wrong selection" << endl;
		return;
	}

	Item* itemToSell = inventoryItems[index];
	int sellPrice = itemToSell->getCost() / 2;

	i_money.addMoney(sellPrice);
	i_inventory.removeItem(itemToSell);
	stock.push_back(itemToSell);

	cout << "something about selling " << itemToSell->getName() << " for " << sellPrice << " gold." << endl;

}
