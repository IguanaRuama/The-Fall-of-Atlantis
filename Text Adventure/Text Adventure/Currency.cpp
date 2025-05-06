#include "Currency.h"
#include <iostream>
#include <vector>

Currency::Currency(int i_moneyAmount, string i_description)
{
	moneyAmount = i_moneyAmount;
	description = i_description;
}

void Currency::setMoneyAmount(int i_moneyAmount)
{
	moneyAmount = i_moneyAmount;
}

void Currency::setDescription(string i_description)
{
	description = i_description;
}

void Currency::addMoney(int i_amount)
{
	moneyAmount = moneyAmount + i_amount;
}

void Currency::removeMoney(int i_amount)
{
	moneyAmount = max(0, moneyAmount - i_amount);
}

int Currency::getMoneyAmount()
{
	return moneyAmount;
}

string Currency::getDescription()
{
	return description;
}
