#pragma once
#include <string>
#include <vector>
#include <functional>

using namespace std;

struct ItemAction
{
	string text;
	string effectText;
	function<void()> effectFunction;

	ItemAction(const string& t, const string& e, function<void()> eF = []() {})
		: text(t), effectText(e), effectFunction(eF) {}
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
	void setEffectAndFunction(ItemAction i_action, string i_effect, function<void()> i_function);
	
	void addAction(string i_name, string i_effect, function<void()> i_function);
	void removeAction(int i_aIndex);


};