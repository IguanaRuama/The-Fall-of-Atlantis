#pragma once
#include <string>
#include <vector>

using namespace std;

class Item
{
private:
	string name;
	string description;
	vector<string> actions;
	string action;
	int cost;

public:
	Item(string i_name, string i_description, int i_cost);

	string getName();
	string getDescription();
	string getActions();
	int getCost();

	void setName(string i_name);
	void setDescription(string i_description);
	void setDefaultActions();
	void setCost(int i_cost);
	
	void addAction(string i_action);
	void removeAction(string i_action);


};