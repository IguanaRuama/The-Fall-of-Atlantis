#include "GameTools.h"
#include "Location.h"
#include "Location.h"
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <vector>
#include <chrono>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <stdlib.h>


void Tools::GameTools::clearScreen()
{
	system("cls");
}

void Tools::GameTools::title()
{
	std::cout << R"( _____                                                                                                                               _____ 
( ___ )-----------------------------------------------------------------------------------------------------------------------------( ___ )
 |   |                                                                                                                               |   | 
 |   |  _________  __              ________        __   __               ___        _       _   __                   _    _          |   | 
 |   | |  _   _  |[  |            |_   __  |      [  | [  |            .' ..]      / \     / |_[  |                 / |_ (_)         |   | 
 |   | |_/ | | \_| | |--.  .---.    | |_ \_|,--.   | |  | |    .--.   _| |_       / _ \   `| |-'| |  ,--.   _ .--. `| |-'__   .--.   |   | 
 |   |     | |     | .-. |/ /__\\   |  _|  `'_\ :  | |  | |  / .'`\ \'-| |-'     / ___ \   | |  | | `'_\ : [ `.-. | | | [  | ( (`\]  |   | 
 |   |    _| |_    | | | || \__.,  _| |_   // | |, | |  | |  | \__. |  | |     _/ /   \ \_ | |, | | // | |, | | | | | |, | |  `'.'.  |   | 
 |   |   |_____|  [___]|__]'.__.' |_____|  \'-;__/[___][___]  '.__.'  [___]   |____| |____|\__/[___]\'-;__/[___||__]\__/[___][\__) ) |   | 
 |___|                                                                                                                               |___| 
(_____)-----------------------------------------------------------------------------------------------------------------------------(_____))" << std::endl;

}

void Tools::GameTools::titleAndLocation(Location* i_location)
{
	Tools::GameTools::clearScreen();

	title();

	cout << i_location->getLocationName();
	cout << "\n";
	cout << i_location->getLocationDescription();
}

void Tools::GameTools::delay(int i_secs)
{
	std::this_thread::sleep_for((std::chrono::seconds(i_secs)));
	
}

void Tools::GameTools::exitGame()
{		
	cout << "Are you sure you sure you want to leave this adventure?	y/n \n";
	string input;
		
	if (input == "y")
	{
		cout << "We hope to see you soon solider... \n";
		delay(3);
		exit;
	}
	if (input == "n")
	{
		cout << "Very well, we must not wait another moment. \n";
		return;
	}
	else 
	{
		cout << "Invalid input, try again. \n";
		exit;
	}
}



