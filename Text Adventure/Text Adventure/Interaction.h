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

	Choice(string& t, string& e, function<void()> eF = []() {})
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
	string outputChoices();
	vector<Item*>& getRequiredItems();
	string getEffect(int i_cIndex);
	function<void()> getFunction(int i_cIndex);

	
	void setDescription(string i_description);
	void setRequiredItem(Item* i_requiredItem);
	void setEffect(Choice i_choice, string i_effect);
	void runFunction(function<void()> i_function);

	void addChoice(string i_text, string i_effect, function<void()>i_function);
	void removeChoice(int i_cIndex);

	bool isAvailable(Inventory i_inventory);
};

