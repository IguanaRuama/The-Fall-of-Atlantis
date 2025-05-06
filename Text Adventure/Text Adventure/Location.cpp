#include "Location.h"
#include "Item.h"
#include <iostream>

Location::Location(string i_name, string i_decision, string i_description)
{
	locationName = i_name;
	locationDecision = i_decision;
	locationDescription = i_description;
}

string Location::getLocationName()
{
	return locationName;
}

string Location::getLocationDecision()
{
	return locationDecision;
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

void Location::setLocationDecision(string i_decision)
{
	locationDecision = i_decision;
}

void Location::setLocationDescription(string i_description)
{
	locationDescription = i_description;
}

void Location::addLink(Location* i_link)
{
	links.push_back(i_link);
}

void Location::addInteraction(Interaction* i_interaction)
{
	interactions.push_back(i_interaction);
}

