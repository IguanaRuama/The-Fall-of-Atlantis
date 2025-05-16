#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Inventory.h"
#include "Item.h"

using namespace std;

struct Choice
{
	string text;
	string effectText;
	function<void()> effectFunction;

	Choice(const string& t, const string& e, function<void()> eF = []() {})
		: text(t), effectText(e), effectFunction(eF) {}
};

class Interaction
{
private:
	string name;
	string description;
	vector<Item*>requiredItem;
	vector<Choice>choices;

public:
	Interaction(string i_name, string i_description);

	string getName();
	string getDescription();
	vector<Item*> getRequiredItems(Inventory* i_inventory);
	string getEffect(int i_cIndex);
	function<void()> getFunction(int i_cIndex);
	vector<Choice> getChoices();

	
	void setDescription(string i_description);
	void setRequiredItem(Item* i_requiredItem);
	void setEffect(const string& text, const string& i_effect);
	void runFunction(function<void()> i_function);

	void outputChoices(Inventory* i_inventory);
	void addChoice(string i_text, string i_effect, function<void()>i_function);
	void removeChoice(const string& text);

	bool isAvailable(Inventory i_inventory);
};

