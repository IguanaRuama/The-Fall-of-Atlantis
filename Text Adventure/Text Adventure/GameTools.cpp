#include "GameTools.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <iostream>
#include <string>

namespace GameTools
{
	void GameTools::clearScreen()
	{
		system("cls");
	}

	void GameTools::delay(int i_secs)
	{
		_Thrd_sleep_for(i_secs * 1000);
	}

	void GameTools::exitGame()
	{
		cout << "Are you sure you sure you want to leave this adventure?	y/n \n";
		string input;
		cin >> input;

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

	void GameTools::startMusic(int i_music)
	{
	}

	void GameTools::stopMusic()
	{
	}

	void GameTools::outputImage(int i_stage)
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
}


