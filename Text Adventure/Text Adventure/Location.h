#pragma once
#include "Item.h"
#include "Interaction.h"
#include <string>
#include <vector>

using namespace std;

class Location
{
private:
	string locationName;
	string locationDecision;
	string locationDescription;
	vector<Location*>links;
	vector<Interaction*>interactions;

public:
	Location(string i_name, string i_decision, string i_description);

	string getLocationName();
	string getLocationDecision();
	string getLocationDescription();
	vector<Location*>getLinks();
	vector<Interaction*>getInteractions();

	void setLocationName(string i_name);
	void setLocationDecision(string i_decision);
	void setLocationDescription(string i_description);

	void addLink(Location* i_link);
	void addInteraction(Interaction* i_interaction);
};