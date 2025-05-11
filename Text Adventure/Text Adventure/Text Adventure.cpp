
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <windows.h>

#include "Inventory.h"
#include "Interaction.h"
#include "Location.h"
#include "Item.h"
#include "Currency.h"
#include "Shop.h"
#include "GameTools.h"

using namespace std;

atomic<bool> timerRunning;
Inventory playerInventory = Inventory();
Currency playerCurrency = Currency(0, "A shimmering coin forged of gold--  small trinkets that hold plentiful meaning.");

void onTimerEnd(int stage)
{
	timerRunning = false;
	switch (stage)
	{
	case 1:
		cout << "stage 1 transition\n";
		break;
		
	case 2:
		cout << "stage 2 transition\n";
		break;

	case 3:
		cout << "stage 3 transition\n";
		break;
	}
}

void startTimer(int secs, int stage)
{
	this_thread::sleep_for(chrono::seconds(secs));
	timerRunning = true;

	cout << "something about time is limited, player has" << (secs/60) << " minutes \n";

	onTimerEnd(stage);
}

void startMenu()
{

}

void stageOne()
{
	cout << "With each breath the tase of salt remains prevalent in the City of Atlantis; a flavour you've grown awfully atuned to as breaths come deeper\n"
		" and shorter following another spar with your brother in arms. Though as spears flew and laughter ached more that bruises, it's a time you've\n"
		" grown to adore. Your boots are heavy with the dust of training grounds, yet oh-so light as you meet the cobble of your front porch.\n"

		"\n As always, you are greeted with an image you believe is a blessing from Hestia-- Phylas, your spirited young son, chasing shadows with an\n"
		" olive branch, and Anthusa, a woman whose creation could only be that of Aphrodites beauty and Philophrosynes strength; your wife, watching \n"
		" with a calm that doesn’t quite reach her eyes.\n"

		"\n All thats needed to complete the scene is you, Rhesus. \n";
	
		Location frontPatio = Location("The Front Door", "The humble entrance to your home. Vines of ivy and blooming myrtle sprawl lazily across the\n"
		" frames, weaving through the beams of the pergola overhead; casting shaodws over the clay floor below. A lone crate stands proud against\n"
		" one wall; dipping in the centre from years of use as a bench. A small basin adjacent, once holding a pool of water, now homes a cluster\n"
		" of Phylas' mighty 'weapons'; including, but not limited to, a variety of sticks and stones. Though quaint, it is a place of rest;\n"
		" of honeyed wine and whispered words at dusk; of quiet songs shared in fading light.");

		Interaction enterHome("Walk through the front door.", "Simple yet sturdy, the door to your home shields the warmth of your family from the hardships of the world. A worn lock finds its own home at\n"
			" the edge of the wooden barrier.");

		enterHome.()
}

int main()
{
	stageOne();
	//function intro
	//function main menu
	// stage 1
	//stage 2
	//stage 3
	//end

}



