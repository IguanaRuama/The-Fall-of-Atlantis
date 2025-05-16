#pragma once
#include <string>
#include <vector>
#include < stdlib.h >
#include "Location.h"

using namespace std;

namespace Tools 
{
	class GameTools
	{
	public:
		static void clearScreen();
		static void title();
		static void titleAndLocation(Location* i_location);
		static void delay(int i_secs);
		static void exitGame();
		static void startMusic(int i_music);
		static void stopMusic();
		static void outputImage(int i_stage);
	};
}


