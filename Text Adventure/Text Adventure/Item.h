#pragma once
#include <string>
#include <vector>

using namespace std;

struct ItemAction
{
	string name;
	string effectText;

	ItemAction(string& n, string& e) : name(n), effectText(e) {}
};

class Item
{
private:
	string name;
	string description;
	vector<ItemAction> actions;
	string action;
	int cost;

public:
	Item(string i_name, string i_description, int i_cost);

	string getName();
	string getDescription();
	string getActions();
	int getCost();
	string geEffect(int i_eIndex);

	void setName(string i_name);
	void setDescription(string i_description);
	void setDefaultActions();
	void setCost(int i_cost);
	void setEffect(string i_eIndex); //here
	

	void removeAction(string i_action);


};