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
	string locationTravelDescription;
	string locationDescription;
	vector<Location*>links;
	vector<Interaction*>interactions;

public:
	Location(string i_name, string i_description, string i_travelDescription);

	string getLocationName();
	string getLocationDescription();
	string getLocationTravelDescription();
	vector<Location*>getLinks();
	vector<Interaction*>getInteractions();

	void setLocationName(string i_name);
	void setLocationDescription(string i_description);
	void setLocationTravelDescription(string i_travelDescription);

	void removeLink(int i_lIndex);
	void addLink(Location* i_link);
	void addInteraction(Interaction* i_interaction);
};