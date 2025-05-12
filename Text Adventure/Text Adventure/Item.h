#pragma once
#include <string>
#include <vector>

using namespace std;

struct ItemAction
{
	string text;
	string effectText;

	ItemAction(const string& t, const string& e)
		: text(t), effectText(e) {}
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
	string getEffect(int i_eIndex);

	void setName(string i_name);
	void setDescription(string i_description);
	void setDefaultActions();
	void setCost(int i_cost);
	void setEffect(ItemAction i_action, string i_effect); //here
	
	void addAction(string i_name, string i_effect);
	void removeAction(int i_aIndex);


};