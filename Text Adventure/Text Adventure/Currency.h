#pragma once
#include <string>
#include <vector>
#include "Item.h"

using namespace std;

class Currency
{
private:
	int moneyAmount;
	string description;

public:
	Currency(int i_moneyAmount, string i_description);

	void setMoneyAmount(int i_moneyAmount);
	void setDescription(string i_description);
	void addMoney(int i_amount);
	void removeMoney(int i_amount);

	int getMoneyAmount();
	string getDescription();
};

