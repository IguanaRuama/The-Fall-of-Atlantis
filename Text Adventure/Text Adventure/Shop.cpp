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
	stock.push_back(new Item("Blunt Dagger", "Clearly used."));
	stock.back()->setCost(5);

	stock.push_back(new Item("Cloth Cape", "A dark, large, hooded garment."));
	stock.back()->setCost(5);
}

void Shop::displayItems()
{
	cout << " (Story of markets) items: \n";
	for (int i = 0; i < stock.size(); ++i)
	{
		cout << "[" << (i) << "] " << stock[i]->getName()
			<< "  -  " << stock[i]->getDescription()
			<< "  -  " << stock[i]->getCost() << " gold \n";
	}
}

void Shop::buyItem(int index, Inventory& i_inventory, Currency& i_money)
{
	if (index < 0 || index > stock.size())
	{
		cout << " (some yap about needing to try again)" << endl;
		return;
	}

	Item* item = stock[index];
	if (i_money.getMoneyAmount() >= item->getCost())
	{
		i_money.removeMoney(item->getCost());
		i_inventory.addItem(item);
		cout << "Yap about what item you got" << item->getName() << endl;
	}
	else
	{
		cout << "sm about scowling works cause you have no money" << endl;
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
