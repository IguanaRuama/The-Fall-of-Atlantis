#include "Location.h"
#include "Item.h"
#include <iostream>

Location::Location(string i_name, string i_description, string i_travelDescription)
{
	locationName = i_name;
	locationDescription = i_description;
	locationTravelDescription = i_travelDescription;
}

string Location::getLocationName()
{
	return locationName;
}

string Location::getLocationDescription()
{
	return locationDescription;
}

string Location::getLocationTravelDescription()
{
	return getLocationTravelDescription();
}

string Location::outputLinks()
{
	string output = "============================================================================================================================= \n";
	output += "Travel: \n";

	for (int i = 0; i < links.size(); i++)
	{
		output += links[i]->getLocationName() + "            ";
	}

	output += "============================================================================================================================= \n";

	return output;
}

string Location::outputInteractions()
{
	string output = "============================================================================================================================= \n";
	output += "Interactions: \n";

	for (int i = 0; i < interactions.size(); i++)
	{
		output += "[" + to_string(i) + "] " + interactions[i]->getName() + "\n";
	}

	output += "============================================================================================================================= \n";

	return output;
}

vector<Location*> Location::getLinks()
{
	return vector<Location*>();
}

vector<Interaction*> Location::getInteractions()
{
	return vector<Interaction*>();
}

void Location::setLocationName(string i_name)
{
	locationName = i_name;
}


void Location::setLocationDescription(string i_description)
{
	locationDescription = i_description;
}

void Location::setLocationTravelDescription(string i_travelDescription)
{
	locationTravelDescription = i_travelDescription;
}

void Location::removeLink(int i_lIndex)
{
	auto it = find(links.begin(), links.end(), links[i_lIndex]);
	if (it != links.end())
	{
		links.erase(it);
	}
}

void Location::addLink(Location* i_link)
{
	links.push_back(i_link);
}

void Location::addInteraction(Interaction* i_interaction)
{
	interactions.push_back(i_interaction);
}

void Location::removeInteraction(Interaction* i_interaction, vector<Interaction*>i_interactions)
{
	auto it = find(interactions.begin(), interactions.end(), i_interaction);

	// If found, remove it from the vector
	if (it != interactions.end())
	{
		interactions.erase(it);
	}
}

