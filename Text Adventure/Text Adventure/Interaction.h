#pragma once
#include <string>
#include <vector>
#include "Inventory.h"
#include "Item.h"


using namespace std;

struct Choice
{
	string text;
	string effectText;

	Choice(string& t, string& e) 
		: text(t), effectText(e) {}
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

	string getDescription();
	string getChoices();
	string getRequiredItems();
	string getEffect(int i_cIndex);
	
	void setDescription(string i_description);
	void setRequiredItem(Item* i_requiredItem);
	void setEffect(Choice i_choice, string i_effect);

	void addChoice(string i_text, string i_effect);
	void removeChoice(int i_cIndex);

	bool isAvailable(Inventory i_inventory);
};

