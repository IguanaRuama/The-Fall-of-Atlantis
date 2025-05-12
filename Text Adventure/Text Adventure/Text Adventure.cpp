
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

int stage; //could potentially have menus as stages etc
int familyBond = 0;

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
	//Intro to stage 1

	cout << "With each breath the tase of salt remains prevalent in the City of Atlantis; a flavour you've grown awfully atuned to as breaths come deeper\n"
		" and shorter following another spar with your brother in arms. Though as spears flew and laughter ached more that bruises, it's a time you've\n"
		" grown to adore. Your boots are heavy with the dust of training grounds, yet oh-so light as you meet the cobble of your front porch.\n"

		"\n As always, you are greeted with an image you believe is a blessing from Hestia-- Phylas, your spirited young son, chasing shadows with an\n"
		" olive branch, and Anthusa, a woman whose creation could only be that of Aphrodites beauty and Philophrosynes strength; your wife, watching \n"
		" with a calm that doesn’t quite reach her eyes.\n"

		"\n All thats needed to complete the scene is you, Rhesus. \n";

	//Location creation
	
	Location frontPatio = Location("The Patio", "The humble entrance to your home. Vines of ivy and blooming myrtle sprawl lazily across the\n"
		" frames of your front door, weaving through the beams of the pergola overhead; casting shaodws over the clay floor below. A lone crate stands\n"
		" proud against one wall; dipping in the centre from years of use as a bench. A small basin adjacent, once holding a pool of water, now homes \n"
		" a cluster of Phylas' mighty 'weapons'; including, but not limited to, a variety of sticks and stones. Though quaint, it is a place of rest;\n"
		" of honeyed wine and whispered words at dusk; of quiet songs shared in fading light.", "Leave the Home.");
		
		Location kitchen = Location("The Kitchen", "The kitchen of your home lies at the heart of your dwelling, warm and fragrant, a sanctuary of hearth\n"
			" and habit. The walls are etched faintly with symbols of good fortune passed down from your grandmother’s hand. A central hearth, circular\n"
			" and blackened with the memory of a thousand meals, still smolders faintly, its coals murmuring beneath a clay grate. Clay amphorae line the\n"
			" walls, their bellies full of olive oil, lentils, figs, and salted fish, while bundles of rosemary, thyme, and bay leaf dangle from the rafters\n"
			" like offerings to a domestic goddess. A narrow stone counter hugs one wall, its surface worn from years of chopping and kneading. Mortars and\n"
			" pestles rest nearby, flanked by small terracotta jars of spices whose scents drift  through the air—- cumin, coriander, crushed pepperberry.\n"
			" Light filters through a single window, illuminating motes of flour and smoke that dance like spirits in the late afternoon stillness.This is\n"
			" Anthusa’s domain, and her quiet artistry lingers in every corner, in the way the knives are always sharp and the bread always warm, as if the\n"
			" home itself were alive and content.\n", "Head to the kitchen");

		Location childrenRoom = Location("Phylas' Room", "Phylas’ room is a small alcove tucked just off the main corridor, little more than a sleeping space\n"
			" by grown men's standards—- but to a child, it is a kingdom. The walls are daubed with faded mural fragments: dolphins leaping through crested waves\n"
			" , a chariot racing the wind, a crude drawing of a cyclops added in charcoal—- his proud contribution to the family’s artistry. A low bed, stuffed\n"
			" with straw and layered with woven blankets, sits beneath a narrow window that spills sunlight across the floor in golden shafts. The bed is\n"
			" perpetually tousled, tangled with feathers and toy soldiers carved from olive wood. Beside it stands a squat wooden chest crammed with treasures only\n"
			" Phylas could catalog—- seashells, pebbles, a knucklebone or two, a slingshot carved by yourself. A small oil lamp rests on a shelf overhead, its bowl\n"
			" charred with soot, often left burning late into the night during imagined campaigns against mythical beasts.The air smells faintly of pine and sun\n"
			"-- warmed linen, touched with the earthy scent of dust stirred by endless adventures. In this room, time still plays, and the weight of the world is\n"
			" kept, mercifully, at the threshold.\n", "Make your way to Phylas' room.");

		Location bedroom = Location("Anthusa and I's Bedroom", "Our bedroom sits in the quiet rear wing of the home, where the stone walls stay cool even in the\n"
			" high heat of summer. It is a room of stillness and soft shadows, where the sea’s hush can be heard faintly through a narrow window. The bed itself is\n"
			" broad, hewn from dark cypress wood, its legs carved with swirling motifs of waves and lotus blooms. Woven blankets in deep indigo and earthen red are\n"
			" folded neatly atop the straw-filled mattress, though one is always found askew by morning. Along one wall rests a low table bearing a bronze mirror, a\n"
			" comb carved from bone, and a small tray of scented oils; lavender, myrrh, and sweet almond—- subtle reminders of Anthusa’s quiet rituals. A rack above\n"
			" holds their cloaks and tunics, smelling faintly of cedar, and tucked beneath the bed is a box of letters, old charms, and other small things kept not for\n"
			" use, but for memory. It is a room not of grandeur, but of comfort—- a refuge from the clang of spears and the shuffle of daily life, where whispered\n"
			" conversations linger longer than dreams and silence is shared like bread between those who know one another well.\n", "Head into your bedroom.");

		Location courtyard = Location("The Courtyard", "The courtyard of your home is small and plain, but it breathes with life. Open to the sky, its walls are whitewashed\n"
			" and worn, their corners chipped from years of weather and children's games. A narrow path of sunbaked tiles leads to a low stone basin at the center,\n"
			" where rainwater collects and the occasional sparrow dips its beak. There's no fountain, no marble, only what was built with strong hands and patience.\n"
			" A fig tree leans against one wall, gnarled and generous, its roots pushing up through the cracked stone.Around it, Anthusa has coaxed a modest garden\n"
			" from the soil: sprigs of thyme and rosemary grow alongside onions, beans, and a squat lemon bush that bears fruit when the seasons are kind. Clay pots\n"
			" hold whatever didn’t take in the ground—parsley, garlic, a struggling vine that Phylas insists will one day reach the roof. A bench made from two planks\n"
			" and a pair of old amphorae sits beneath a patch of shade, often cluttered with a tunic or half-mended sandal. Phylas uses the space as his battlefield, his\n"
			" temple, his ship deck—whatever the day demands. Anthusa tends her garden in the quiet moments, the hem of her chiton dusty with soil, her hands stained green\n"
			" from herbs.And you, Rhesus, have ended many long days here with a waterskin and aching shoulders, staring up at the sky and wondering, perhaps, how long peace\n"
			" might last. It is not a grand place. But it is home.", "Wander into the courtyard.");

		//Add links

		frontPatio.addLink(&kitchen);

		kitchen.addLink(&frontPatio);
		kitchen.addLink(&childrenRoom);
		kitchen.addLink(&bedroom);
		kitchen.addLink(&courtyard);

		childrenRoom.addLink(&kitchen);
		childrenRoom.addLink(&bedroom);
		childrenRoom.addLink(&courtyard);

		bedroom.addLink(&childrenRoom);
		bedroom.addLink(&kitchen);
		bedroom.addLink(&bedroom);

		courtyard.addLink(&kitchen);
		courtyard.addLink(&childrenRoom);
		courtyard.addLink(&bedroom);

		//Add items

		Item patioKey = Item("Patio Key", "A familiar sight that unlocks your front door", 0);
		patioKey.setDefaultActions();
		patioKey.addAction("Use", "The door opens with a soft 'thud', a noise symbolic of a hard days end.");

		Item spices = Item("Bundle of Spices", "A small pouch consisting of a variety of spices; courtesy of your wife", 5);
		spices.setDefaultActions();

		Item fish = Item("A raw fish", "A slimy, smelly, creature of the sea", 15);
		fish.setDefaultActions();

		Item knife = Item("A Noticably Sharp Kitchen Knife", "A knife who often resides in your kitchen, perfectly sharpened by the hands of your wife", 10);
		knife.setDefaultActions();

		Item cloak = Item("A Hooded Cloak", "A large garment that could easily conceal its wearer.", 2);
		cloak.setDefaultActions();

		Item oils = Item("Anthusa's Oils", "A collection of oils including made by your lover.", 5);
		oils.setDefaultActions();

		Item comb = Item("Anthusa's Comb of Bone", "A family heirloom; a comb carved from animal bone.", 5);
		comb.setDefaultActions();

		Item toy = Item("Phylas' Wooden Soldier", "A handmade miniature soldier belonging to your son.", 5);
		toy.setDefaultActions();

		Item slingshot = Item("Phylas' Slingshot", "A small, yet concerningly deadly, toy of your son.", 5);
		slingshot.setDefaultActions();

		Item vegetables = Item("Bundle of Vegetables", "A small bag containing fresh vegetables from your garden", 10);
		vegetables.setDefaultActions();

		//Add interactions

		Interaction enterHome("Enter your home.", "Simple yet sturdy, the door to your home shields the warmth of your family from the hardships of the world. A worn lock finds its own home at\n"
			" the edge of the wooden barrier.");

		Interaction explorePatio("Explore the patio", "You stand before your humble home; Phylas remains locked in imaginary battle whilst Anthusa oversees-- her lips curling as he\n"
			" slays whatever beast dared cross hin.");
		explorePatio.addChoice("Join Phylas.", "With a swift scoop you lift Phylas into the air, swinging and spinning the fearsome hero as his stern facade erupts into childish\n"
			" giggles. The sight is enough to bring a subtle smile to Anthusa's face accompanied by a quiet tutting. With arms weary from sparring, you place your son back upon\n"
			" his feet, leaving the 'soldier' back to his battle."); //finished here



		enterHome.addChoice("Attempt to open the door", "You attempt to push the door open, yet to no avail as the lock stands sturdy.");
		enterHome.addChoice("Kick the door", "Stern look from wife.");
		enterHome.setRequiredItem(&patioKey);
		
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



