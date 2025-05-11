#include "Location.h"
#include "Item.h"
#include <iostream>

Location::Location(string i_name, string i_description)
{
	locationName = i_name;
	locationDescription = i_description;
}

string Location::getLocationName()
{
	return locationName;
}

string Location::getLocationDescription()
{
	return locationDescription;
}

vector<Location*> Location::getLinks()
{
	return links;
}

vector<Interaction*> Location::getInteractions()
{
	return interactions;
}

void Location::setLocationName(string i_name)
{
	locationName = i_name;
}


void Location::setLocationDescription(string i_description)
{
	locationDescription = i_description;
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

