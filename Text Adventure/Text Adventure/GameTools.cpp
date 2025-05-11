#include "GameTools.h"
#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <vector>
#include <chrono>
#include <windows.h>
#include <string>

void Tools::GameTools::clearScreen()
{
	system("cls");
}

void Tools::GameTools::delay(int i_secs)
{
	_Thrd_sleep_for(i_secs * 1000);
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

void Tools::GameTools::startMusic(int i_music)
{
	switch (i_music)
	{
	case 1:
		PlaySound(L"Intro.wav", NULL, SND_FILENAME | SND_ASYNC); //SFML if had time to implement
		break;
	default:
		break;
	}
}

void Tools::GameTools::stopMusic()
{
	PlaySound(NULL, 0, 0);
}

void Tools::GameTools::outputImage(int i_stage)
{
	switch (i_stage)
	{
	case 1:
		cout << "afloat atlantis \n";
		break;

	case 2:
		cout << "attacked atlantis \n";
		break;
	case 3:
		cout << "sunken atlantis \n";
		break;
	default:
		cout << "image selection error \n";
		break;
	}
}


