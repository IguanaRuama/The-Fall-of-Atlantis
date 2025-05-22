
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <windows.h>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <limits>


#include "Inventory.h"
#include "Interaction.h"
#include "Location.h"
#include "Item.h"
#include "Currency.h"
#include "Shop.h"
#include "GameTools.h"

using namespace std;

thread timerThread;
atomic<bool> timerRunning(false);
atomic<bool> paused(false);

mutex mtx;
condition_variable cv;

string input;
int choice;
int stage = 1;
int familyBond = 0;
bool escaped;
bool stageEnded = false;

Inventory playerInventory = Inventory();
Currency playerCurrency = Currency(0, "Drachmae-- a shimmering coin forged of gold; small trinkets that hold plentiful meaning.");
Location* currentLocation;

//DECLARING ITEMS

Item patioKey = Item("Patio Key", "A familiar sight that unlocks your front door", 0);

Item spices = Item("Bundle of Spices", "A small pouch consisting of a variety of spices; courtesy of your wife", 5);
Item fish = Item("A cured fish", "A slimy, smelly, cooked creature of the sea", 15);
Item knife = Item("A Noticably Sharp Kitchen Knife", "A knife who often resides in your kitchen, perfectly sharpened by the hands of your wife", 10);

Item toy = Item("Phylas' Wooden Soldier", "A handmade miniature soldier belonging to your son.", 5);
Item slingshot = Item("Phylas' Slingshot", "A small, yet slightly dangerous, toy of your sons.", 5);

Item mensCloak = Item("Your Hooded Cloak", "A large garment that could easily conceal its wearer.", 2);
Item oils = Item("Anthusa's Oils", "A collection of oils including made by your lover.", 5);
Item comb = Item("Anthusa's Comb of Bone", "A family heirloom; a comb carved from animal bone.", 5);
Item broach = Item("Anthusa's Copper Broach", "A family heirloom of Anthusa's, now passed down to you", 10);

Item vegetables = Item("Bundle of Vegetables", "A small bag containing fresh vegetables from your garden", 10);

Item womensCloak = Item("Stolen Womens Cloak", "A threadbare shawl, once a warm embrace against cold nights, now frayed and faded from endless wear.Its muted earth tones are stained and softened\n"
	" by time, with loose threads whispering of better days. Hidden beneath its folds is the faint scent of smoke and whispered prayers, a quiet shield against the world’s harshness.", 3);

Item childsCloak = Item("Stolen Childs Cloak", "A small, too-short cloak that barely shields the wearer from the chill. Its fabric is thin and worn, with patches stitched hurriedly to cover tears.\n"
	" Once bright, the colors have dulled to muted pastels, and the edges are frayed from countless days of play and hardship. Despite its worn state, it carries a faint hint of childhood hope—-\n"
	"fragile but stubbornly alive.", 2);

Item enemyBroach = Item("Athenian Broach", "shaped in the likeness of an owl, its eyes wide and watchful, the symbol of Athena worked with masterful precision. The edges are etched with fine laurel\n"
	" leaves, and the clasp bears the unmistakable geometry of Athenian craftsmanship—- precise, austere, and proud. Though simple in size, its presence commands attention, marking its wearer as a\n"
	" soldier or servant of Athens. In times of war, such an emblem could mean danger—- or passage.", 15);


void onTimerEnd(int& stage) //Triggers when a stage is ended
{
	timerRunning = false;
	switch (stage)
	{
	case 1:

		Tools::GameTools::clearScreen();
		Tools::GameTools::title();

		cout << "The day shifts in the smallest of ways. A faint tremor passes beneath your feet—- so gentle it could be mistaken for a passing cart-—\n"
			" but something in it makes you pause. The fig tree rustles though there is no wind. Far off, beyond the rooftops, a dark smudge lingers\n"
			" on the horizon. Ships, perhaps... but too many. Too still. No one else seems to notice. Phylas hums to himself as he balances a stick along\n"
			" the courtyard wall, and Anthusa wipes her hands on her apron, focused on her herbs. But your stomach knots. A feeling—- subtle, sharp, and\n"
			" certain—- takes hold. Without sounding the alarm, you urge your family to gather what you need and head to the town center. Just in case.\n";

		cout << "Press 'Enter' to continue.\n";
		getline(cin, input);
		break;
		
	case 2:

		Tools::GameTools::clearScreen();
		Tools::GameTools::title();

		cout << "A low rumble rolls beneath your feet—- at first like the purr of distant thunder, but quickly swelling into something deeper, hungrier.\n"
			" The clay cups on the vendor’s stall rattle, one toppling and shattering unnoticed. Townsfolk begin to pause, eyes darting, the festive rhythm\n"
			" of the market dissolving into murmurs. Another tremor hits, stronger this time. Roof tiles shiver. Hanging wares sway on their ropes. A dog yelps\n"
			" and bolts.\n"
			"\n Then, someone shouts from the harbor wall.\n"
			"\n You turn just in time to see the sails: stark white against the blue, their crests marked with the owl of Athens. Warships.More than a dozen.\n"
			"\n The crowd erupts. Some flee, shouting warnings.Others stand frozen in disbelief as the quake lurches again, harder now—cracking stone, toppling\n"
			"fruit carts. Dust clouds bloom in the distance.\n"
			"\n Atlantis is shifting.Atlantis is under siege.\n"
			"\nFrom here, nothing will remain untouched.\n";
		
		cout << "Press 'Enter' to continue.\n";
		getline(cin, input);
		break;

	case 3:
		if(escaped)
		{
			cout << "You Escaped!\n";
			break;
		}
		else if (!escaped)
		{
			cout << "You didnt escape Atlantis, maybe next time\n";
			break;
		}

	}
}

void startTimer(int secs, int stage) //Starts 5 min timer
{
	timerRunning = true;
	cout << "Time is limited. Rhesus has " << (secs / 60) << " minutes to complete Stage " << stage << "\n";

	int remaining = secs;

	while (remaining > 0 && timerRunning)
	{
		this_thread::sleep_for(chrono::seconds(1));
		unique_lock<mutex> lock(mtx);

		// Wait if paused
		cv.wait(lock, [] { return !paused.load(); });

		--remaining;
	}

	if (remaining == 0)
	{
		onTimerEnd(stage);
	}
}

void pauseTimer() //pauses timer
{
	paused = true;
	cout << "\nTimer paused.\n";
}

void resumeTimer() //resumes timer
{
	paused = false;
	cout << "Timer resumed.\n";
	cv.notify_all();
}

void nextStage(int& stage) //skips stage
{
	if (stage < 3)
	{
		timerRunning = false;

		stage++;
		cout << "You rush to the next stage of the invasion " << stage << "!\n";

	}
	else
	{
		cout << "invalid\n";
	}
}

void startScreen() //outputs starter text
{
	Tools::GameTools::title();

	cout << "You are an Atlantean warrior, Rhesus, caught in the chaos of the Athenians invasion of the Atlantis during its fall .With your family by your side, your mission is to survive the unfolding war and navigate through three stages\n"
		"Each stage lasts five intense minutes—- within that time, you must complete critical tasks, gather and manage essential items in your inventory, and make strategic decisions that will determine your fate.\n"

		"\n Some decisions will have lasting affects on your family relationships, and some choices may be irriversable; so choose wisely.\n"

		"\n Explore multiple locations within each stage to uncover new opportunities and challenges.\n"

		"\n Will you outwit the Athenians and protect your family, or will the tides of war claim you all? Your journey begins now.\n"

		"\ Press 'Enter' to continue...";

	getline(cin, input);

	cout << "With each breath the tase of salt remains prevalent in the City of Atlantis; a flavour you've grown awfully atuned to as breaths come deeper\n"
		" and shorter following another spar with your brother in arms. Though as spears flew and laughter ached more that bruises, it's a time you've\n"
		" grown to adore. Your boots are heavy with the dust of training grounds, yet oh-so light as you meet the cobble of your front porch.\n"

		"\n As always, you are greeted with an image you believe is a blessing from Hestia-- Phylas, your spirited young son, chasing shadows with an\n"
		" olive branch, and Anthusa, a woman whose creation could only be that of Aphrodites beauty and Philophrosynes strength; your wife, watching \n"
		" with a calm that doesn’t quite reach her eyes.\n"

		"\n All thats needed to complete the scene is you, Rhesus. \n";
	cout << "Press 'Enter' to continue.\n";

	getline(cin, input);

	return;
}

void itemActionsMenu(Inventory* inventory, Item* item, int stage, Location* location) //menu options accossiated with items
{
	Tools::GameTools::titleAndLocation(location);
	cout << "\n" << item->getName() << "Actions:\n";

	while (true)
	{
		item->outputActions();

		cout << "[" << item->getActions().size() << "] Back\n> ";

		cin >> input;
		choice = stoi(input);

		if (choice == item->getActions().size())
		{
			return;
		}
		else if (choice >= 0 && choice < item->getActions().size())
		{
			cout << "\n" << item->getEffect(choice) << "\n";
			item->runFunction(item->getFunction(choice));

			break;
		}
		else
		{
			cout << "Invalid input.\n";
		}
	}
}

void choicesMenu(Inventory* inventory, Interaction* interaction, int stage, Location* location) //menu options accossiated with choices
{
	Tools::GameTools::titleAndLocation(location);
	cout << "\n" << interaction->getDescription() << "\n";

	while (true) 
	{
		cout << "\n Available Choices:\n";
		interaction->outputChoices(&playerInventory);


		cout << "[" << interaction->getChoices().size() + interaction->getRequiredItems(&playerInventory).size() << "] Back\n> ";

		cin >> input;
		choice = stoi(input);

		if (choice == (interaction->getChoices().size() + interaction->getRequiredItems(&playerInventory).size()))
		{
			return;
		}
		else if (choice >= 0 && choice < interaction->getChoices().size())
		{
			cout << "\n" << interaction->getEffect(choice) << "\n";
			interaction->runFunction(interaction->getFunction(choice));

			return;
		} 
		else if (choice >= interaction->getChoices().size() && choice < (interaction->getChoices().size() + interaction->getRequiredItems(inventory).size()))
		{
			choice = choice - interaction->getChoices().size();
			itemActionsMenu(inventory, interaction->getRequiredItems(inventory)[choice], stage, location);

			return;
		}
		else
		{
			cout << "Invalid input.\n";
		}
	}
}

void interactionMenu(Location* location, int stage) //menu options accossiated with interactions
{
	Tools::GameTools::titleAndLocation(location);
	if (location->getInteractions().empty()) 
	{
		cout << "There’s nothing to interact with here.\n";
		return;
	}

	while (true) 
	{
		cout << "\n Available Interactions:\n";
		location->outputInteractions();

		cout << "[" << location->getInteractions().size() << "] Back.\n";

		cin >> input;
		choice = stoi(input);

		if (choice == location->getInteractions().size())
		{
			return;
		}
		else if (choice >= 0 && choice <= location->getInteractions().size() - 1)
		{
			if (location->getInteractions()[choice] != nullptr)
			{
				choicesMenu(&playerInventory, location->getInteractions()[choice], stage, location);
			}
			else
			{
				cout << "Invalid interaction. Returning to menu.\n";
			}
		}
		else 
		{
			cout << "Invalid option.\n";
		}
	}
}

void travelMenu(Location*& location, int stage) //menu options accossiated with travel
{
	Tools::GameTools::titleAndLocation(location);
	while (true) 
	{
		location->outputLinks();
		cout << "[" << location->getLinks().size() << "] Back.\n ";

		cin >> input;
		choice = stoi(input);

		if (choice == location->getLinks().size())
		{
			return;
		}
		else if (choice >= 0 && choice < location->getLinks().size())
		{
			location = location->getLinks()[choice];
			currentLocation = location;

			break;
		}
		else 
		{
			cout << "Invalid choice.\n";
		}
	}
}

void gameplayMenu(int& stage, Location* location) //Main menu
{
		Tools::GameTools::titleAndLocation(location);

		cout << "What would you like to do?\n"
			"[1] Interact.\n"
			"[2] Travel.\n"
			"[3] View Inventory\n"
			"[4] Pause.\n"
			"[5] Skip to Next Stage\n"
			"[6] Quit\n";

		cin >> input;
		choice = stoi(input);

		switch (choice)
		{
		case 1:
			
			interactionMenu(location, stage);
			break;

		case 2:
			Tools::GameTools::titleAndLocation(location);
			travelMenu(location, stage);
			break;

		case 3:
			
			playerInventory.outputInventory(&playerInventory);

			break;

		case 4:
			pauseTimer();

			Tools::GameTools::clearScreen();
			Tools::GameTools::title();

			cout << "Enter 1 to resume or 2 to quit.\n";
			cin >> input;
			choice = stoi(input);

			if (choice == 1)
			{
				cout << "Game resumed...\n";
				Tools::GameTools::delay(3);

				resumeTimer();
			}
			else if (choice == 2)
			{
				Tools::GameTools::exitGame();
			}
			else
			{
				cout << "Invalid Input. Please enter 1 or 2.\n";
				cin >> input;
			}
			break;
			
		case 5:
			nextStage(stage);
			break;
			
		case 6:
			Tools::GameTools::exitGame();
		}
}

void stageOne()
{

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
			" Phylas could catalog—- seashells, pebbles, a knucklebone or two. A small oil lamp rests on a shelf overhead, its bowl\n"
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

		//PATIO INTERACTIONS

		patioKey.setDefaultActions();


		Interaction enterHome("Enter your home.", "Simple yet sturdy, the door to your home shields the warmth of your family from the hardships of the world. A worn lock finds its own home at\n"
			" the edge of the wooden barrier.");

		frontPatio.addInteraction(&enterHome);

		enterHome.addChoice("Attempt to open the door", "You attempt to push the door open, yet to no avail as the lock stands sturdy. Your hands rummage in your satchel for\n"
			" your key... Yet to no avail... Guess you'll have to find another way in.",
			[&]()
			{});
		enterHome.addChoice("Kick the door", "Your boot strikes the door with a heavy thud, loud enough to rattle the hinges and startle a bird from the fig tree. Just as you step\n"
			"back to try again, a sharp voice cuts through the courtyard.\n"
			"\n Rhesus!\n"
			"\n You turn to see Anthusa standing a few paces away, hands on her hips, a basket of herbs dangling forgotten at her side. Beside her, Phylas is frozen mid-swing with a\n"
			" stick, eyes wide with delight at the spectacle.\n"
			"\n 'Have you truly forgotten where we keep the key?' she asks, brows arched, exasperation tempered with a hint of irritation. 'Or were you hoping to impress the door with\n"
			" your martial training?' She quips... Where was that spare key again?",
			[&]()
			{});

		enterHome.setRequiredItem(&patioKey);
		patioKey.addAction("Use key.", "With a soft click and the groan of old wood easing on its hinges, the door yields inward, releasing a draft of cool, earthy air from within the\n"
			"shaded interior. The scent of home—clay, herbs, and sun-warmed stone—spills into the courtyard as the threshold opens, quiet and unassuming, as if nothing had disturbed its routine.\n"
			" Anthusa and Phylas follow you in; your wife wandering to the kitchen with your son running to his room.\n"
			" to his room",
			[&]()
			{
				cout << "You can now access the kitchen via the travel menu! " << currentLocation->getLocationName() << endl;
				currentLocation->addLink(&kitchen);
				frontPatio.removeInteraction(&enterHome, frontPatio.getInteractions());
				//ONE TIME EVENT
			});
		//add key and link

		Interaction explorePatio("Explore the patio", "You stand before your humble home; Phylas remains locked in imaginary battle whilst Anthusa oversees-- her lips curling as he\n"
			" slays whatever beast dared cross hin.");

		frontPatio.addInteraction(&explorePatio);

		explorePatio.addChoice("Join Phylas.", "With a swift scoop you lift Phylas into the air, swinging and spinning the fearsome hero as his stern facade erupts into childish\n"
			" giggles. The sight is enough to bring a subtle smile to Anthusa's face accompanied by a quiet tutting. With arms weary from sparring, you place your son back upon\n"
			" his feet, leaving the 'soldier' back to his battle.",
			[&]() 
			{
				familyBond += 1;
				explorePatio.removeChoice("Join Phylas.");
			});
		//+1 fam

		explorePatio.addChoice("Greet Anthusa.", "You step toward Anthusa, her hands paused in their work as she looks up at you with that familiar, quiet smile. The sunlight\n"
			" catches in her hair, and for a moment, the world narrows to just the two of you. She brushes a bit of soil from her palms onto her tunic and says, 'Back already\n"
			"? Or did the soldiers grow tired of you showing them how it's done?' There’s warmth in her voice—teasing, but gentle—the kind that wraps around your tired bones like\n"
			"a well-worn blanket. She slips a small cloth pouch into your hand. 'Here—- three drachmae. I took the olives and greens to the market this morning.The baker paid fair.\n"
			" She shrugs, modest as ever. 'Might be enough for oil... or whatever it is you warriors waste coin on when you're pretending not to be sore.'",
			[&]() {
				playerCurrency.addMoney(5);
				explorePatio.removeChoice("Greet Anthusa.");
			});
		// +5 money

		explorePatio.addChoice("Investigate basin", "Once used to collect rainwater in better seasons, the old stone basin now sits dry and repurposed, its cracked rim softened by time\n"
			" and a layer of dust. Inside, it holds the treasures of a child’s world: a crooked fleet of stick ships with fig-leaf sails, a trio of round stones painted as warriors,\n"
			" and a clay shard with a scratched-on face, proudly dubbed “Cyclops.” Bits of string, feathers, and bent twigs are tucked into the corners like tools awaiting orders. What\n"
			" was once a vessel for life-giving water now cradles the wild imaginings of Phylas—battles waged in silence, kingdoms built and toppled before supper, and dreams sturdy\n"
			"enough to weather any war.",
			[&]()
			{});

		explorePatio.addChoice("Investigate crate", "Pushed up against the sun-bleached wall just beside the doorway, the old wooden has corners softened by years of weather and wear.\n"
			" The wood is greyed and splintered in places, patched once with a strip of cloth now stiff with age. Anthusa sometimes rests her basket on it while working in the garden, and\n"
			" Phylas uses it as a pirate ship or watchtower depending on the day.\n"
			"\n Behind it, wedged into the gap between crate and wall, is a flicker of silver. There lies the family's quiet secret: a flat stone with just enough give to lift free, beneath\n"
			" which rests the spare key tothe house. It's a hiding spot born of necessity, known only to the three of you—- and now, dusted with the faint mark of your bootprints from earlier\n"
			" , it feels like the quiet keeper of a dozen small emergencies and daily forgettings.",

			[&]() 
			{
				if (!playerInventory.hasItem(&patioKey))
				{
					playerInventory.takeItemChoice(&patioKey);
					explorePatio.setEffect("Investigate crate", "Pushed up against the sun-bleached wall just beside the doorway, the old wooden has corners softened by years of weather and wear.\n"
						" The wood is greyed and splintered in places, patched once with a strip of cloth now stiff with age. Anthusa sometimes rests her basket on it while working in the garden, and\n"
						" Phylas uses it as a pirate ship or watchtower depending on the day.\n");
				}

			});
		// add key

		
		//KITCHEN INTERACTIONS

		spices.setDefaultActions();
		fish.setDefaultActions();
		knife.setDefaultActions();

		Interaction explorekitchen = Interaction("Enter the kitchen.", "The kitchen breathes with warmth and memory, tucked at the heart of your home like a glowing ember. A round hearth smolders\n"
			" low at its center, the coals still alive beneath a clay grate. Light from a narrow window drapes the room in a haze of smoke and flour dust, dancing with the scents of cumin,\n"
			" coriander, and pepperberry. Amphorae line the walls like silent sentinels, their sealed mouths hiding oil, grain, and the salty tang of preserved fish. A stone counter stretches\n"
			" beneath bunches of drying herbs, its surface smoothed by years of labor—kneading, chopping, grinding. There, a mortar still holds traces of spice, and nearby, a familiar knife\n"
			" rests idle in the dusting of flour. You can  hear Anthusa humming from beside the counter. This place, built by hand and habit, has always been hers-— and yours by way of her care.");
		
		kitchen.addInteraction(&explorekitchen);


		explorekitchen.addChoice("Investigate amphorae.", "Lined neatly against the kitchen wall, the clay amphorae stand tall and cool to the touch, their wide mouths sealed with waxed cloth.\n"
			" As you lift the cover from one marked with a faded fish symbol, a salty scent rises to meet you. Nestled in layers of dried herbs and coarse brine, a few small, cured fish\n"
			" glisten faintly in the dim light—ready to be taken, traded, or cooked.",
			[&]() 
			{
				if (!playerInventory.hasItem(&fish))
				{
					playerInventory.takeItemChoice(&fish);
				}

			});
		//add fish to inventory

		explorekitchen.addChoice("Investigate motar and pestle.", "Set in the corner of the worn stone counter, the mortar is heavy and well-used, its bowl dusted with traces of ground spice.\n"
			" Resting beside it, a small bundle of fresh herbs- —rosemary, thyme, and bay leaf—has been tied with twine, ready for use. The scent is sharp and earthy, rising as you reach out\n"
			" to take it, a piece of the kitchen’s soul wrapped in green.",
			[&]() 
			{
				if (!playerInventory.hasItem(&spices))
				{
					playerInventory.takeItemChoice(&spices);
				}
			});
		//add herbs

		//make a one time interaction
		Interaction takeKnifeEncounter = Interaction("Investigate the stone counter.", "The stone counter runs along the kitchen wall, its surface worn smooth by years of chopping, kneading, and quiet labor .Amoung the\n"
			" scattered sprigs of herbs and a dusting of flour lies a single iron-bladed knife—well-used, its wooden handle darkened by time and touch. It rests just within reach, unassuming\n"
			" but sharp, a tool of the home that could just as easily become something else.\n"
			"\n As your fingers curl around the knife’s worn handle, Anthusa looks up from her basket with a puzzled frown. 'Rhesus?' she says softly, brushing a lock of hair from her brow.\n"
			" 'What do you need that for?' There’s no anger in her voice—only curiosity, and a hint of concern.\n"
			"\n She watches you now, herbs forgotten in her hands.'You’re not headed back to the barracks, are you?' Her tone shifts, just slightly. Not accusing, but wary.");

		kitchen.addInteraction(&takeKnifeEncounter);

		takeKnifeEncounter.addChoice("Take the knife anyway; just encase.", "You lift the knife from the counter, its weight familiar in your hand. 'Just in case,' you mutter, not meeting Anthusa’s\n"
			" eyes. 'For protection.'\n"
			"\n She doesn’t answer at first. Her gaze lingers on you, steady and unreadable, a bundle of thyme still held loosely in her fingers.The silence stretches.You expect a rebuke, maybe a\n"
			" sigh—- but instead, she simply turns back to her work, quieter than before.\n"
			"\n The excuse hangs in the air between you, thin and unconvincing. Whatever danger you’re guarding against, she’s not sure she believes it-— or worse, that you do.",
			[&]() 
			{
				playerInventory.takeItemChoice(&knife);
				familyBond -= 1;

				kitchen.removeInteraction(&takeKnifeEncounter, kitchen.getInteractions());
			});
		//add knife -1 family

		takeKnifeEncounter.addChoice("Set the knife back down; theres no need to worry your wife.", "Your hand hesitates over the knife, then slowly pulls back. The kitchen hums with quiet—- the soft\n"
			" crackle of the hearth, the rustle of herbs in Anthusa’s hands. She doesn’t look up, and you say nothing. The blade remains where it is, untouched, just another tool in its place. The\n"
			" moment passes like breath, unnoticed, and the day carries on.",
			[&]() 
			{
				kitchen.removeInteraction(&takeKnifeEncounter, kitchen.getInteractions());
			});


		//CHILDRENS ROOM

		toy.setDefaultActions();
		slingshot.setDefaultActions();

		Interaction exploreChildsRoom = Interaction("Admire Phylas' toys.", "Phylas’ toy collection is a patchwork of imagination and found treasure—wooden soldiers with chipped paint, smooth river stones\n"
			" pretending to be treasure, and a bundle of twine tangled into what he insists is a sea serpent. Each piece is worn with play and affection, scattered across his room in small, heroic\n"
			" scenes. To anyone else, it’s a mess; to Phylas, it’s an empire in motion.");

		childrenRoom.addInteraction(&exploreChildsRoom);

		exploreChildsRoom.addChoice("Rummage in the wooden chest", "You kneel beside the squat wooden chest and lift the lid, greeted by the soft clatter of childhood treasures. Digging past\n"
			" smooth pebbles, bits of string, and a shell with a hole in it, your fingers close around a familiar shape—roughly carved, worn at the edges. It’s a small wooden soldier, notched at the\n"
			" shoulder, with a charcoal mark across its chest like your old training tunic. Phylas must have made it to resemble you. Its stance is crooked but proud, a tiny guardian standing amid the\n"
			" clutter of a child’s world.",
			[&]()
			{
				if (!playerInventory.hasItem(&toy))
				{
					playerInventory.takeItemChoice(&toy);
				}

			});
		// take toy

		Interaction takeSlingshotEncounter = Interaction("Admire his drawings.", "You pause by the wall, eyes tracing the faded mural of dolphins and chariots, now joined by Phylas’ own additions—wobbly\n"
			" stick-figures locked in battle, a towering cyclops looming over a tiny spear-wielding hero. There’s charm in the rough lines, a fierce little heart behind every stroke. As you smile, your gaze\n"
			" drifts upward to a dusty shelf, where a familiar shape catches your eye: the slingshot you carved for him last spring, its yew frame still bound with the same bit of worn leather. You hadn’t\n"
			" realized he kept it so close. If you want to take it with you, you’ll need to choose your words carefully—- it's still his, after all.");

		childrenRoom.addInteraction(&takeSlingshotEncounter);

		takeSlingshotEncounter.addChoice("Gently request the slingshot from your son.", "You gesture toward the shelf with a faint smile, eyes on the slingshot nestled between a pile of pebbles and a wax-sealed seashell.\n"
			" 'You’ve kept the old slingshot,' you say, stepping closer. 'Mind if I borrow her for a bit? Just in case I run into any troublemakers.'\n"
			"\n Phylas looks up from the floor where he’s been arranging his toy soldiers into a lopsided battle line.He blinks, then gives a small, reluctant nod.\n"
			"\n 'Alright... but you better bring it back. He’s got a name now—- Thornstriker.'\n"
			"\n You lift it gently, the leather still taut, the weight of it surprisingly familiar in your grip.",
			[&]()
			{
				playerInventory.addItem(&slingshot);
				childrenRoom.removeInteraction(&takeKnifeEncounter, childrenRoom.getInteractions());

			});
			//add slingshot

		takeSlingshotEncounter.addChoice("Teasingly request the slingshot from your son.", "You smirk, crossing your arms as you eye the shelf like a rival admiring a worthy opponent.\n"
			"\n 'Still keeping your finest weapon up high, eh? You wouldn’t mind if your old man took it for one last heroic battle, would you?'\n"
			"\n Phylas lights up, practically bouncing as he hops to his feet.\n"
			"\n 'Only if you promise to shout something cool when you use it!' he says, puffing out his chest. 'Like—- 'for Atlantis!'\n"
			"\n You laugh and pluck the slingshot from its perch. 'for Atlantis' it is.'",
			[&]()
			{
				playerInventory.addItem(&slingshot);
				familyBond += 1;
				childrenRoom.removeInteraction(&takeKnifeEncounter, childrenRoom.getInteractions());
			});
		// add slingshot + +1 fam

		takeSlingshotEncounter.addChoice("Take the slingshot; you made it after-all.", "You step forward without a word, reaching for the slingshot like it’s just another tool you’re owed. Your fingers close around the\n"
			" wooden grip—then stop, held in place by the weight of a young stare.\n"
			"\n 'Don’t,' Phylas says flatly. He’s not pouting, not whining—just watching you, quiet and firm. 'That one stays with me.'\n"
			"\nFor a moment, you consider arguing. But something in his tone—- the way he sits a little straighter, the way his fingers curl protectively around a nearby toy-- makes you step back. The\n"
			" slingshot stays on the shelf.\n"
			"\n He doesn’t say anything else. Doesn’t need to.The silence that follows is colder than before.",
			[&]()
			{
				familyBond-=1;
				childrenRoom.removeInteraction(&takeKnifeEncounter, childrenRoom.getInteractions());
			});
		// -1 fam

		takeSlingshotEncounter.addChoice("Utilise sentimental restraint-- you don't need it, its your sons.", "You reach out, then pause—fingers brushing the wooden frame, the smooth curve you once carved by firelight.\n"
			"\n You smile faintly and lower your hand.\n"
			"\n 'You’ve kept it well,' you murmur. 'Better it stays here, where it belongs.'\n"
			"\n Phylas looks up at you, surprised.Then, more softly, 'I keep it close in case monsters come through the window.'\n"
			"\n He taps his chest. 'But I sleep easier knowing you made it.'\n"
			"\n You ruffle his hair gently and let the moment stand. The slingshot stays on the shelf, right where it should be.\n",
			[&]()
			{
				familyBond+=1;
				childrenRoom.removeInteraction(&takeKnifeEncounter, childrenRoom.getInteractions());
			});


		//BEDROOM

		mensCloak.setDefaultActions();
		oils.setDefaultActions();
		comb.setDefaultActions();
		broach.setDefaultActions();

		Interaction exploreBedroom = Interaction("Wander through your bedroom.", "A woolen cloak, frayed at the hem from years of use, hangs over a peg near the doorway, still carrying the faint scent of ash and laurel.\n"
			" On a low wooden shelf sits a small collection of clay vials—- Anthusa’s precious oils—pressed from lavender, myrtle, and crushed olives, their stoppers stained with use. Beside them lies a delicate\n"
			" bone comb, its teeth worn smooth from years of tending curls and quiet mornings. The room holds little in the way of luxury, but every item speaks of care, routine, and a life gently shared.");

		bedroom.addInteraction(&exploreBedroom);

		exploreBedroom.addChoice("Investigate the cloak", "You step toward the cloak, its coarse wool brushing softly against the wall where it hangs. The fabric is thick, weathered, and heavy with the scent of smoke and\n"
			" sun-— your scent. Faint stains from old marches and scattered threads at the hem speak of long days on the road and nights beneath uncertain skies. As your fingers trace the worn edges, a small copper\n"
			" brooch glints at the collar—Anthusa’s handiwork, fastened there long ago. It's more than just a soldier’s cloak. It’s a quiet ledger of where you’ve been… and who waited for you to come home.",
			[&]()
			{
				playerInventory.takeItemChoice(&mensCloak);
				if (playerInventory.hasItem(&mensCloak))
				{
					playerInventory.addItem(&broach);
					exploreBedroom.removeChoice("Investigate the cloak");
				}
			});
		// add cloak

		exploreBedroom.addChoice("Inspect Anthusa's oils.", "Lined neatly along a low shelf, Anthusa’s oils sit in small clay vials, their rounded forms smoothed by years of gentle use. You lift one—lavender, by the\n"
			" faint, sweet scent that clings to the rim—and the cool clay warms quickly in your hand. Each stopper is stained with traces of its contents: gold from pressed olives, pale green from crushed bay, a\n"
			" dark swirl where rosemary once steeped. They aren’t luxuries, not here—they’re tools of care, of comfort. These quiet jars hold the rhythms of the home: healing, soothing, softening what the world\n"
			" wears down.",
			[&]()
			{
				playerInventory.takeItemChoice(&oils);
			});
		//add oils

		exploreBedroom.addChoice("Admire Anthusa's comb.", "Resting beside the oils, the bone comb lies quiet and familiar, its ivory surface smoothed to a gentle sheen by years of use. The teeth are fine, some slightly\n"
			" crooked from time or tugged tangles, but none broken—a testament to its care. You pick it up, light as breath, and feel the faint warmth still clinging to it. Traces of oil linger between the prongs,\n"
			" scented with myrtle and rosemary. This was never just for grooming—- it was part of Anthusa’s morning ritual, each stroke a moment of calm, a reclaiming of order before the day began. It’s a humble thing,\n"
			" yet it hums with the presence of its owner.",
			[&]()
			{
				playerInventory.takeItemChoice(&comb);
				if (playerInventory.hasItem(&comb))
				{
					exploreBedroom.removeChoice("Admire Anthusa's comb.");
				}
			});
		//add comb

		//COURTYARD

		vegetables.setDefaultActions();

		Interaction exploreGarden = Interaction("Admire the courtyard.", "Near the fig tree—- bent with age, yet generous with fruit—- sits a burlap sack, half-tucked beneath the garden bench. Inside, a handful of vegetables\n"
			" remain from Anthusa’s morning at the market: onions, a few soft tomatoes, and sprigs of greens still damp from dew and handling. She must have dropped it here in her rush to tend the garden, or to chase\n"
			" Phylas from climbing the lemon bush again. It rests quietly now, a simple thing—- humble proof of care, of labor, of the home’s quiet pulse.");

		courtyard.addInteraction(&exploreGarden);

		exploreGarden.addChoice("Inspect the sack.", "You crouch beside the bench and pull the sack into better light. The rough fabric is damp in places, darkened with soil and the scent of crushed herbs. Inside, a modest harvest\n"
			" nestles together—wilted greens, a few onions with dirt still clinging to their roots, and two bruised tomatoes softening at the edges. It’s what remains after a morning at the market, set aside and nearly forgotten.\n"
			" The weight of it is light, but it carries the shape of a day’s work—- Anthusa’s careful hands, trading quietly for coin, never wasting what could still feed a home.",
			[&]()
			{
				if (!playerInventory.hasItem(&vegetables))
				{
					playerInventory.takeItemChoice(&vegetables);
				}
			});
		//add veges


		//GAMEPLAY

		stageEnded = false;
		currentLocation = &frontPatio;

		timerRunning = true;
		thread timerThread(startTimer, 5 * 60, stage);

		while (!stageEnded)
		{
			gameplayMenu(stage, currentLocation);
			if (stage > 1)
			{
				stageEnded = true;
			}
		}

		timerRunning = false;
		if (timerThread.joinable())
			timerThread.join();
}

void stageTwo()
{
	//LOCATIONS
	Location townCentre = Location("The Town Centre", "The town centre sprawls with the vibrant chaos of daily life, a mosaic of color, sound, and scent pressed\n"
		" between whitewashed walls and sun-drenched streets. Stalls crowd the square, their awnings fluttering like sails—heaped with figs, honeycakes, woven linens,\n"
		" and copper trinkets that catch the light. Merchants shout over one another in good-natured rivalry, their voices rising like gulls over a sea of barter and\n"
		" banter. The scent of grilled fish mingles with sweet wine and crushed mint from nearby vendors. Street performers dot the open spaces—flute players weaving\n"
		" bright melodies, a masked actor declaiming some comic verse, a boy balancing on amphorae while a crowd cheers. Children dart between legs, chasing each other\n"
		" with scraps of bread or toy spears. It is a place of rhythm and routine, where worry has no place—- at least, not yet.", "Make your way back to town centre");
	Location alleyway = Location("The Alleyway", "You slip from the clamor of the town center into a narrow alley, where the light fades and the walls close in. The air\n"
		" here is thick with the scent of refuse and damp stone, far from the perfume of spices and sunlit chatter. Broken crates line the walls, and a thin stream of\n"
		" water trickles from somewhere unseen. Ahead, half-shrouded in shadow, a small group of beggars linger—- too still, too quiet. Their cloaks are worn, their\n"
		" eyes sharp, tracking you the moment you appear. One of them shifts, just slightly, as if to give way to your path. They speak low among themselves, but their\n"
		" voices hush when yours draws near. You don't know what happened to them—- but they could aid you...", "Slip into the narrow alleyway, away from the noise and watchful eyes.\n");
	Location northMarket = Location("The Northern Markets", "The northern market hums with a slightly different rhythm—less theatrical than the central square, more purposeful.\n"
		" Stalls here are packed tighter, with traders calling out in clipped, businesslike tones. Baskets of grain, amphorae of oil, and bolts of undyed cloth are stacked\n"
		" in careful rows, catering more to households and traders than passersby. The scent of cured meat and fresh-baked barley loaves fills the air, mingling with the\n"
		" tang of vinegar and smoke from nearby braziers. You pass a group of old men playing a quiet game of knucklebones beside a spice vendor, while a woman counts coins\n"
		" with practiced speed beneath a striped canopy. There's less laughter here—but plenty of eyes.", "Head toward the northern market, where traders gather and everyday life unfolds in measured rhythms.");
	Location eastMarket = Location("The Eastern Markets", "The eastern market pulses with a livelier, more exotic energy—here, the scents of spices and foreign goods mingle in\n"
		" the warm air. Colourful fabrics from distant lands ripple from weathered stalls, their dyes vivid against the sun-bleached stone. Vendors hawk rare imports:\n"
		" fragrant frankincense, bright saffron, glistening beads of amber, and delicately carved ivory trinkets. Musicians with foreign instruments add a lilting\n"
		" soundtrack, drawing curious crowds. This market is a crossroads of culture and mystery, where every bargain might carry a story from beyond the sea.\n", "Make your way to the eastern market, where exotic goods and distant tales beckon.");
	Location southMarket = Location("The Southern Markets", "The southern market is a hub of craftsmanship and sturdy trade, where the clatter of blacksmiths’ hammers mingles\n"
		" with the scent of fresh-cut timber and heated metal. Stalls here display leather goods, pottery, and hand-forged tools—items born of hard work and skill. The air\n"
		" is thick with the tang of smoke and resin as artisans shape their wares before attentive customers. Here, the rhythm is steady and purposeful, a marketplace built\n"
		" on the strength and craft of those who shape the everyday life of Atlantis.", "Head to the southern market, where blacksmiths thrive.");
	Location westMarket = Location ("The Southern Market", "The western market is a vibrant hub of food and drink, alive with the rich aromas of roasting meats, freshly baked bread,\n"
		" and ripe fruits piled high on woven trays. Wooden carts laden with amphorae of wine and honeyed pastries line the sun-warmed streets, while vendors call out\n"
		" tempting offers of spiced olives, salted fish, and thick cheeses. The lively chatter of hungry customers blends with the clinking of clay cups and the rhythmic\n"
		" beat of a nearby drum circle, creating a festive atmosphere that invites both celebration and rest. This market pulses with the simple pleasures of daily life,\n"
		" where the senses feast as eagerly as the body.", "Head toward the western market, where the rich scents of food and drink fill the air and lively voices beckon.\n");

	//ADD LINKS

	townCentre.addLink(&northMarket);
	townCentre.addLink(&alleyway);
	townCentre.addLink(&eastMarket);
	townCentre.addLink(&southMarket);
	townCentre.addLink(&westMarket);
	northMarket.addLink(&townCentre);
	northMarket.addLink(&eastMarket);
	northMarket.addLink(&westMarket);
	eastMarket.addLink(&townCentre);
	eastMarket.addLink(&northMarket);
	eastMarket.addLink(&southMarket);
	southMarket.addLink(&townCentre);
	southMarket.addLink(&eastMarket);
	southMarket.addLink(&westMarket);
	westMarket.addLink(&townCentre);
	westMarket.addLink(&northMarket);
	westMarket.addLink(&southMarket);

	//ADD TOWNCENTRE INTERACTIONS

	Interaction scoutArea = Interaction("You survey the town centre carefully, eyes drifting toward the docks", "The docks curve around the town’s edge, split into three distinct harbors—one for fishing boats, another for merchant cargo,\n"
		" and a broad western bay that opens toward the sea. As sunset settles over the city, the piers remain active with the final labors of the day, though an uneasy stillness lingers beyond the horizon.");

	townCentre.addInteraction(&scoutArea);

	scoutArea.addChoice("Look towards the eastern piers where the day’s catch is hauled ashore, the scent of salt and fish thick in the air.", " Near the end of one pier, a couple works together in practiced silence—she untangles the last of the netting while he sorts through\n"
		" the catch, tossing smaller fish into a shallow basket. A lantern flickers beside them, casting golden light over their weathered faces and the silver gleam of\n"
		" fish laid out in rows. It’s a scene of quiet rhythm, untouched by the unease creeping at the edges of your thoughts.", [&]()
		{});

	scoutArea.addChoice("Peer south to where merchants and porters bustle over crates and cargo, their voices sharp in the evening light.", "The southern cargo docks buzz with fading energy, the last rays of sunlight glinting off bronze fittings and amphorae stacked in neat, precarious towers.\n"
		" Porters call out over the din, their arms laden with crates of oil, wine, and dyed fabrics that still carry the scent of distant markets. Merchants tally lists\n"
		" on wax tablets, some arguing over weights, others laughing as deals are sealed with handshakes and nods. A boy scrambles up a gangplank with a bundle of rope, while\n"
		" a grizzled shipmaster oversees the unloading of a vessel bearing the mark of a city far to the east. There’s a weary rhythm here, a familiar end-of-day haste—but it\n"
		" pulses with a kind of comfort, the hum of a city still believing that tomorrow will come just as it always has.", [&]()
		{});

	scoutArea.addChoice("Turn your gaze west, toward the quiet open water, where the light fades and distant shapes stir beneath the setting sun.", "The western bay lies quiet beneath the fading light, its waters calm and glassy, reflecting the last hues of sunset like a mirror. Far across the horizon,\n"
		" just beyond the reach of most eyes, dark shapes drift steadily—- sleek ships with Athenian sails, their movement slow but purposeful. The salt air carries no hint of\n"
		" alarm yet, and the fishermen and merchants nearby go about their business, unaware of the approaching shadow. But your gaze lingers, heart tightening as the distance\n"
		" between these silent vessels and your city slowly shrinks with the dying light.\n"
		"\n If-- no. When they dock, it'll be there...\n", [&]()
		{});

	Interaction eavesdrop = Interaction("Linger near a group of townsfolk, their hushed tones and wary glances suggesting more than idle gossip.", "You pause beside a shaded archway where a few dockworkers have gathered away from the bustle of the market, their voices hushed beneath the clamor of merchants\n"
		" and musicians. One of them leans close, his words nearly lost beneath the trill of a flute nearby.\n"
		"\n 'They're not traders—those ships out west. Athenian hulls, no doubt.'\n"
		"\n 'My cousin at the docks saw the crest himself.' Another scoffs nervously, keeping his eyes on the street.\n"
		"\n 'Word is, one of their men’s gone soft—- offering to smuggle folk out. Wants coin for it, though. A small fortune just to disappear.'\n"
		"\n A third spits into the dust.\n"
		"'I’d pay it. If it meant getting my daughters out before the spears start flying.'\n"
		" Their glances dart through the crowd, wary and uncertain, their secret barely veiled beneath the ordinary rhythm of town life.\n");

	townCentre.addInteraction(&eavesdrop);

	eavesdrop.addChoice("Continue to listen...", "You edge closer, feigning interest in a nearby stall as their whispers grow sharper. One murmurs, 'He’s posted down by the bay, standing\n"
		" guard near one of the ships. You can’t miss him—- wears a cracked bronze gauntlet on his left hand. Says little, watches even less, but if you’ve got coin, he might\n"
		" just look the other way.' The others nod, eyes flicking around nervously before the conversation trails off, leaving the image of the gauntleted soldier etched in your mind.", [&]()
		{
			townCentre.removeInteraction(&eavesdrop, townCentre.getInteractions());
			//ONE TIME EVENT
		});

	eavesdrop.addChoice("Back away-- what if they notice you?","Sensing the weight of their secret and the sharp glances flicking your way, you decide it’s best not to linger. Quietly\n"
		" stepping back from the shadowed archway, you weave through the bustling crowd, the hum of the market swallowing your footsteps as you slip away before suspicion can catch you.", [&]()
		{
			townCentre.removeInteraction(&eavesdrop, townCentre.getInteractions());
			//ONE TIME EVENT
		});

	//ALLEYWAY

	mensCloak = Item("A stolen mens cloak","Its faded fabric once served to shield its wearer from the harsh night and prying eyes, now just a ragged reminder of tougher times. Despite its wear,\n"
		" it still carries the faint scent of smoke and shadowed alleys", 3);

	womensCloak.setDefaultActions();
	childsCloak.setDefaultActions();

	Interaction speakWithBeggars = Interaction("Speak with the beggars.", "You step cautiously closer. One of the beggars, an older man with a crooked nose and hands wrapped in cloth, lifts his\n"
		"chin. 'Spare something for those left behind?' he rasps. His companions watch you, saying nothing, but their eyes speak the same question. There's no menace—only hunger,\n"
		" and a wary hope. You could offer them coin… or keep your purse close.");

	alleyway.addInteraction(&speakWithBeggars);
	
	if (playerCurrency.getMoneyAmount() > 5) 
	{
		speakWithBeggars.addChoice("Offer them 5 drachmae from your pouch.", "You draw a few worn coins from your pouch and extend them toward the nearest beggar—a wiry man with hollow cheeks and eyes\n"
			" like ash. His hand trembles slightly as he takes them, and for a moment, the group falls silent. Then, softly, murmured thanks ripple through the alley like wind through dry leaves.\n"
			" You leave with your purse lighter—- but not empty—- and the faint sense that even in shadowed places, good will is not without reward. When you glance back, you catch Anthusa watching\n"
			" you quietly. She says nothing, but the warmth in her eyes speaks volumes.", [&]()
			{
				playerCurrency.removeMoney(5);
				familyBond += 2;

				speakWithBeggars.removeChoice("Offer them a few coins from your pouch");
			});
	}
	
	Interaction intimidateBeggars = Interaction("Step forward, looming over them-- and demand they give you something in return for your mercy.", "You step into the dim light, letting your shadow fall long across the alley floor. The beggars freeze-- threehuddled figures in worn cloaks, faces tight with exhaustion and dread.\n"
		" Your voice cuts through the silence, low and hard. 'If you want to keep what little you have, you’ll pay me for walking away.' They exchange glances, uncertain if you’re bluffing or worse.\n"
		" One of the men shifts protectively toward the child, while the woman clutches something beneath her ragged shawl. The tension is thick, breath held like coin. If you press, you could take\n"
		" something: a man’s weatherworn cloak, a threadbare wrap from the woman, a too-small shawl from the child, or whatever coin they’ve managed to scrape together. None of it worth much—- but it\n"
		" would be yours...\n");

	alleyway.addInteraction(&intimidateBeggars);

	if (!playerInventory.hasItem(&mensCloak))
	{
		intimidateBeggars.addChoice("'Hand over the cloak. You won’t survive the night without it-- but I will.'", "Reluctantly, the man unclasps the threadbare garment from his shoulders and passes it over with shaking hands. He\n"
			" says nothing, but the fire in his eyes burns as he retreats further into the shadows. Your family looks on with unease, Anthusa avoids your gaze.\n", [&]()
			{
				playerInventory.addItem(&mensCloak);

				alleyway.removeInteraction(&intimidateBeggars, alleyway.getInteractions());

				familyBond -= 2;
			});
	}
	
	intimidateBeggars.addChoice("'That wrap of yours—- give it here. It’ll serve me better than it serves you'", "She hesitates, glancing at the child beside her. Then, slowly, she peels it off and hands it to you without a word.\n"
		" Her eyes brim with quiet fury. Anthusa’s jaw tightens; she won’t speak to you for a while.\n", [&]() 
		{
			playerInventory.addItem(&womensCloak);

			alleyway.removeInteraction(&intimidateBeggars, alleyway.getInteractions());

			familyBond -= 2;
		});
	
	intimidateBeggars.addChoice("'Even scraps have value. The little one’s shawl—now.'", "The boy flinches. His mother protests faintly, but neither of them stop you. The child removes the thin cloth and hands it over, trembling.\n"
		" Phylas clutches Anthusa’s hand tightly, eyes wide and afraid.\n", [&]() 
		{
			playerInventory.addItem(&childsCloak);

			alleyway.removeInteraction(&intimidateBeggars, alleyway.getInteractions());

			familyBond -= 2;
		});
	
	intimidateBeggars.addChoice("'I want your drachmae. All of it. I don’t care how little it is.'", "One of them tosses a small pouch your way—- clinking faintly, heavier with desperation than wealth. None of them speak.\n"
		" Anthusa says nothing, but disappointment weighs in the silence.\n", [&]() 
		{
			playerCurrency.addMoney(10);

			alleyway.removeInteraction(&intimidateBeggars, alleyway.getInteractions());

			familyBond -= 2;
		});

	//NORTHMARKET
	Interaction knucklebones = Interaction("Join the game of knucklebones", "A grinning man with a missing tooth beckons you over, but not before reminding you that a 5 drachmae bet is required to join the game. As you place\n"
		" your coins on the table, the thrill of chance hums beneath the noise of the market, promising small fortune or quick loss in equal measure.");

	northMarket.addInteraction(&knucklebones);

	if (playerCurrency.getMoneyAmount() <= 5)
	{
		knucklebones.addChoice("Play strategically", "You approach the knucklebones table with measured steps, studying the players and their throws. You’ve watched the patterns—the subtle ways the bones bounce, the tells in their grips. Carefully,\n"
			" you place your five drachmae bet, focusing on steady, conservative throws rather than wild gambles. Each toss is calculated, aiming to minimize losses rather than chase big wins. Despite your cautious play, luck\n"
			" isn’t on your side today. The bones slip awkwardly from your fingers, tumbling in unpredictable ways. Your carefully planned strategy crumbles as the coins on the table slide away, leaving you with nothing but the\n"
			" humbling lesson of chance.\n", [&]()
			{
				playerCurrency.removeMoney(5);

				northMarket.removeInteraction(&knucklebones, northMarket.getInteractions());
			});

		knucklebones.addChoice("Play risky", "With a confident grin, you stride to the knucklebones table, coins jangling in your fist. The crowd parts slightly, sensing your boldness. You slam down your five drachmae bet, eyes gleaming with daring.\n"
			" No second-guessing—each throw is wild, unpredictable, your hands moving fast and fearless. The bones spin and clatter with reckless energy, defying all odds. The crowd gasps as the dice land perfectly, the highest\n"
			" score glowing in your favor. Cheers ripple through the circle, and your winnings pile up before you. Tonight, luck favors the bold—- and you walk away richer, heart pounding with the rush of risk rewarded.\n", [&]()
			{
				playerCurrency.addMoney(5);

				northMarket.removeInteraction(&knucklebones, northMarket.getInteractions());
			});
	}
	
	Interaction stealFromWoman = Interaction("Step closer to the woman hunched over a small pile of coins.", "Her eyes dart up briefly, sharp and wary, then return to the dull gleam of the drachmae. The lines etched deep across her face tell a story of hard years and harder\n"
		" choices, a life marked by careful calculation and quiet desperation. Around her, the bustle of the market fades into a muted hum, as if her focus pulls the world in tight. You catch the faint scent of worn cloth and faint\n"
		" spices clinging to her ragged shawl. There’s something fragile yet unyielding about her-— a woman who’s held onto what little she has with fierce determination.\n");

	northMarket.addInteraction(&stealFromWoman);

	stealFromWoman.addChoice("Sneak some coins from her collection", "Your fingers move with practiced stealth, slipping through the folds of her ragged shawl while her eyes remain fixed on the small pile of coins. The weight of each drachmae\n"
		" is light, but the risk feels heavy in the air. Heart pounding quietly, you manage to pinch a few coins without so much as a flicker of suspicion crossing her wary gaze. She continues counting, unaware that her\n"
		" precious savings have just grown a little lighter. A small, silent victory — yours alone. You spot Anthusa and Phylas, standing quietly nearby. Their eyes hold a silent judgment,\n", [&]()
		{
			playerCurrency.addMoney(3);

			familyBond -= 1;

			northMarket.removeInteraction(&stealFromWoman, northMarket.getInteractions());
		});

	stealFromWoman.addChoice("Take the coins and run, bold and unapologetic.", "You snatch a fistful of coins from the woman’s purse and bolt, the clatter of her shouted protest trailing behind you. The crowded market parts just enough for your escape—- until\n"
		" a broad-shouldered merchant steps into your path. You crash into him, stagger, and fumble the stolen drachmae into your tunic. But in your haste, you don’t notice the telltale clink at your feet—two of your own coins spill\n"
		" onto the ground. They gleam in the dirt, not unlike the shame catching in your wife's eyes from across the square.", [&]()
		{
			playerCurrency.removeMoney(2);

			familyBond -= 1;

			northMarket.removeInteraction(&stealFromWoman, northMarket.getInteractions());
		});

	//East Market
	Interaction enterClothesShop = Interaction("Step into the stall and browse the rows of dyed tunics and worn sandals.", "You step into the clothier’s stall, where worn cloaks sway gently from a rope line. The air smells of wool and pressed olive oil. Amid faded tunics and tired\n"
		" sandals, a small collection of cloaks catches your eye—- a thick mantle fit for a man, a light green wrap for a woman, and a small ochre cloak just right for a child. None are new, but each could disguise\n"
		" your family, if you’re willing to part with the coin.");

	Shop clothesShop;

	eastMarket.addInteraction(&enterClothesShop);

	if (!playerInventory.hasItem(&mensCloak))
	{
		mensCloak = Item("Thick Mens Cloak", "A deep brown, coarse wool cloak with bone clasps and the faint scent of smoke. Heavy and well-- worn, it's built for hard days and colder nights—practical and lasting.", 10);
		clothesShop.addItem(&mensCloak);

		enterClothesShop.addChoice("Purchase mens cloak", "You pass a few worn drachmae across the stall’s table. The vendor nods, folds the cloak neatly, and presses it into your hands—- a quiet trade sealed in calloused fingers and shared necessity.", [&]()
			{
				clothesShop.buyItem(&mensCloak, playerInventory, playerCurrency);
			});
	}
	if (!playerInventory.hasItem(&womensCloak))
	{
		womensCloak = Item("Womans Green Wrap", "A faded green linen wrap, soft with age and patterned with near-invisible laurel leaves. Light and graceful, it's modest but enduring-— just like Anthusa.", 10);
		clothesShop.addItem(&womensCloak);

		enterClothesShop.addChoice("Purchase womens cloak", "You pass a few worn drachmae across the stall’s table. The vendor nods, folds the cloak neatly, and presses it into your hands—- a quiet trade sealed in calloused fingers and shared necessity.", [&]()
			{
				clothesShop.buyItem(&womensCloak, playerInventory, playerCurrency);
			});
	}
	if (!playerInventory.hasItem(&childsCloak))
	{
		childsCloak = Item("Child’s Ochre Cloak", "A sun-bleached, ochre cloak with a fish-shaped wooden toggle. Patched and faded, it still carries warmth and a touch of play, sized for a child chasing through the streets..", 5);
		clothesShop.addItem(&childsCloak);

		enterClothesShop.addChoice("Purchase childs cloak", "You pass a few worn drachmae across the stall’s table. The vendor nods, folds the cloak neatly, and presses it into your hands—- a quiet trade sealed in calloused fingers and shared necessity.", [&]()
			{
				clothesShop.buyItem(&childsCloak, playerInventory, playerCurrency);
			});
	}

	Interaction music = Interaction("You close your eyes for a moment, letting the lively music wash over you", "Despite the impending threat of enemy ships looming on the horizon, for a brief time the music carries you away, weaving a fragile moment of peace and escape amidst\n"
		" the gathering storm. The dancers whirl and the musicians play on, as if daring fate to break this fleeting celebration of life in Atlantis’s last hours.");

	eastMarket.addInteraction(&music);

	music.addChoice("Take Anthusa’s hand and whisper a playful invitation to dance", "You take Anthusa’s hand gently, feeling the warmth of her skin against yours. The music swells, and together you move in time with the lively rhythm, weaving through\n"
		" the crowd like a single, flowing shadow. Her eyes meet yours—- bright with a mix of joy and quiet resolve—as the world around you blurs into the vibrant colors and sounds of the eastern market. For this\n"
		" moment, fear and war fall away, replaced by the simple, timeless connection of dance.", [&]()
		{
			familyBond += 2;

			music.removeChoice("Take Anthusa’s hand and whisper a playful invitation to dance");
		});

	music.addChoice("You grin warmly and tease Phylas, 'Come on, boy—- show what your ancestors taught you!'", "You take Phylas’ hand, his small fingers fitting snugly in yours. Together, you move to the rhythm of the music swirling through the market-— his laughter bright and infectious\n"
		" as you both spin and sway, the worries of the impending ships momentarily forgotten in this rare moment of joy.", [&]()
		{
			familyBond += 2;

			music.removeChoice("You grin warmly and tease Phylas, 'Come on, boy—- show what your ancestors taught you!'");
		});

	//SOUTHERN MARKET

	Interaction enterWeaponShop = Interaction("You step into the weapon stalls, where the sharp gleam of blades and the scent of oiled leather fill the air.", "A collection of gleaming daggers is carefully arranged on a rough-hewn table—bronze and iron blades polished to catch the light. Some hilts are simply wrapped in worn leather,\n"
		" while others boast delicate carvings of mythic creatures and ancient gods. The sharp scent of oil and metal fills the cramped space, mingling with the dust stirred by your footsteps. The stall feels like\n"
		" a quiet sanctuary of craftsmanship amid the bustling market, promising weapons that offer both protection and honor.\n");

	Shop weaponShop;

	southMarket.addInteraction(&enterWeaponShop);

	if (!playerInventory.hasItem(&knife))
	{
		knife = Item("Brozen-Bladed Dagger", "A finely crafted dagger with a gleaming bronze blade, its edge sharp enough to slice through linen with ease. The hilt is wrapped in dark, worn leather, providing a firm grip, while delicate engravings of olive\n"
			" branches run along the guard—symbols of peace and victory..\n", 10);

		clothesShop.addItem(&knife);

		if (!playerInventory.hasItem(&broach))
		{
			enterClothesShop.addChoice("Purchase dagger", "You pass a few worn drachmae across the stall’s table. The blacksmith nods, and presses it into your hands—- a quiet trade sealed in calloused fingers and shared necessity.", [&]()
				{
					clothesShop.buyItem(&knife, playerInventory, playerCurrency);
				});
		}
		else if (playerInventory.hasItem(&broach))
		{
			enterClothesShop.addChoice("Inquire about dagger", "You eye the finely crafted bronze dagger resting on the worn cloth of the stall. As you reach out to inquire, the blacksmith’s gaze flickers to the bronze broach pinned to your cloak—- a symbol unmistakably\n"
				" tied to Anthusa’s family. His expression shifts from wary to knowing, and he leans in, lowering his voice. 'You carry the mark of pure Atlantian blood. A rare sight here, with Athenian soldiers soon to arrive by sea. I’m leaving this place soon—- danger\n"
				" approaches.' From beneath his counter, he produces a polished Athenian broach, its design bold and unmistakable.\n"
				"\n 'Take this,' he says quietly, pressing it into your hand. 'It’ll open doors with the soldiers—- help you secure passage out of Atlantis, should you need it.' The weight of the broach feels heavier with promise, a key to your escape in a city on the brink.\n", [&]()
				{
					if (!playerInventory.hasItem(&enemyBroach))
					{
						playerInventory.takeItemChoice(&broach);
						enterWeaponShop.removeChoice("Inquire about dagger");
					}
				});
		}
		
	}

	//WESTERN MARKET

	Interaction enterFoodShop = Interaction("Follow the aroma of spiced meats and sweet pastries to a bustling food stall.", "The stall is a riot of color and scent, its wooden counter laden with the bounty of the southern isles. Clay dishes overflow with marinated olives, their skins glistening with oil\n"
		" and herbs. Fresh bread—- round, golden loaves still warm from the oven—is stacked beside small bowls of whipped feta and garlic spread. Skewers of lamb, charred and spiced, sizzle gently over a clay\n"
		" brazier, while clusters of figs and pomegranates rest in woven baskets, their sweetness promised in every glint of juice. Amphorae of dark red wine lean against the stall’s frame, wax-sealed and\n"
		" fragrant. Behind it all, the vendor—- a thick-waisted woman with flour on her sleeves—- greets customers with a booming voice and a practiced smile.");

	Shop foodShop;

	westMarket.addInteraction(&enterFoodShop);

	if (playerCurrency.getMoneyAmount() >= 5) 
	{
		enterFoodShop.addChoice("Buy a honeyed pastry for Anthusa—- something sweet to lighten the day", "The vendor barely meets your eye as you pass over a few drachmae—his mind clearly elsewhere, perhaps on the ships spotted near the horizon. You choose the flakiest one, still warm, and bring it to Anthusa.\n"
			" She accepts it with a raised brow, then a smile that fades only slightly. “Bribing me before the sky falls?” she asks softly. You both know this moment, this simple indulgence, may be among the last\n"
			"untouched by fear.\n", [&]()
			{
				playerCurrency.removeMoney(5);
				familyBond += 1;

				enterFoodShop.removeChoice("Buy a honeyed pastry for Anthusa— - something sweet to lighten the day");
			});
	}

	if (playerCurrency.getMoneyAmount() >= 5)
	{
		enterFoodShop.addChoice("Pick out a skewer of spiced lamb for Phylas-— he's still growing, aftera all.", "You trade coin for a skewer, the vendor hurried now, eyes flicking to the harbor between each sale. You hand the food to Phylas, who takes it eagerly, gnawing with boyish hunger. 'Still warm,' he says through\n"
			" a mouthful, beaming. The grin doesn’t last long. He looks up at you, then beyond, toward the anger-smudged sky. Even the smallest joys feel sharp-edged now, precious in the shadow of what’s coming.\n", [&]()
			{
				playerCurrency.removeMoney(5);
				familyBond += 1;

				enterFoodShop.removeChoice("Buy a honeyed pastry for Anthusa— - something sweet to lighten the day");
			});
	}

	//GAMEPLAY

	stageEnded = false;
	currentLocation = &townCentre;

	timerRunning = true;
	thread timerThread(startTimer, 5 * 60, stage);

	while (!stageEnded)
	{
		gameplayMenu(stage, currentLocation);
		if (stage > 2)
		{
			stageEnded = true;
		}
	}

	timerRunning = false;
	if (timerThread.joinable())
		timerThread.join();

}

void stageThree()
{
	Location crossRoad = Location("The Decision", "Which way do you choose?","");
	Location cargoDock = Location("The Cargo Dock", "The southern cargo docks shudder under your feet as aftershocks ripple through the night. Lanterns sway violently, casting erratic shadows over stacked crates and overturned barrels. The air is thick with salt, smoke, and the sharp scent of fear. Porters scramble hurriedly, voices strained\n"
		" and hushed, loading what they can before the chaos swallows them. A distant rumble—whether thunder or the earth breaking—echoes as the silhouette of an Athenian warship looms ominously at\n"
		" the pier. The city trembles, and time runs out. Tonight, the docks aren’t just a place of trade—they’re a desperate gateway to survival.", "Head south to the cargo dock.\n");

	Location enemyBay = Location("The Bay", "The bay churns beneath a darkening sky, waves crashing violently against jagged rocks as the Athenian ships flood the harbor like a shadowed tide.Flames flicker along the shoreline, smoke twisting upward and blotting out the stars.The water is stained with oil and blood, while distant screams and\n"
		" the clash of steel echo over the roar of the surf.The earth trembles again—- a brutal reminder that the city itself is breaking apart, collapsing under both siege and nature’s fury.Every moment\n"
		" here is a fight for breath, a desperate bid to outrun the encroaching nightmare. You remember whispers of a soldier gone soft, who’s been secretly helping people escape through this chaos—-but which one?", "Head west, to the bay.");

	Location fishingDock = Location("The Fishing Dock", "The eastern piers are no longer quiet. The scent of salt and fish is drowned beneath smoke and the sharp sting of burning wood. Nets lie abandoned, torn and trampled, their catches scattered across blood-slick planks. That same couple—once a picture of calm—now wrestles with a tangle of\n"
		" rope, trying to ready their small boat as the pier groans under the tremors. The man clutches a gutting knife not for fish, but defense, eyes darting toward the shadows at the end of the dock.\n"
		" The woman scans the crashing dark, knuckles white on the tiller. The lantern’s gone—smashed or discarded—and in its place, firelight dances on the water, red and wild.", "Head east, to the fishing dock.");

	crossRoad.addLink(&cargoDock);
	crossRoad.addLink(&enemyBay);
	crossRoad.addLink(&fishingDock);

	//INTERACTIONS
		
	if (playerInventory.hasItem(&mensCloak) && playerInventory.hasItem(&womensCloak) && playerInventory.hasItem(&childsCloak))
	{
		Interaction sneakInShip = Interaction("Cloaks drawn tight, the three of you slip into the shifting maze of crates and shadow, breath held as the ship’s hold swallows you whole", "You crouch low, guiding your partner and Phylas into the narrow space between a stack of amphorae and a tarpaulin-covered pallet. The ship creaks as dockhands scurry overhead, their boots thudding just above your heads. Then, from the dark, a whisper.\n"
			"\n'Patér, ' Phylas breathes, voice small and trembling. 'I want to go home.'\n"
			"\n Your heart knots. He’s shaking—- clutching your cloak with small, sweat-slick fingers, eyes wide with the rising tide of panic.A single sound too loud, a single cry, and it’s over.");

		cargoDock.addInteraction(&sneakInShip);

		if (playerInventory.hasItem(&toy))
		{
			sneakInShip.addChoice("Sooth Phylas with his toy, a reminder from a simpler time.", "You reach into your satchel, fumbling past broken bread and damp cloth, until your fingers close around it-— his toy soldier, worn smooth by hours of play.\n"
				"The moment it appears, Phylas’s breath catches. He presses it to his cheek, clutching it like a lifeline. The fear doesn't vanish—- but it dulls. He stays quiet. The dark holds.", [&]()
				{
					cout << "The ship groans once more as it pulls from the dock, the sounds of the city swallowed by the sea. Phylas breathes slow against your chest, his fear quieted—- for now.Your partner rests her forehead\n"
						"against your shoulder, eyes shut, jaw tight. Above, the commotion fades into distance, and only the rhythmic churn of oars remains. You don’t know where this vessel leads, or what waits beyond the dark\n"
						" water. But for this moment, your family is with you.\n";

					Tools::GameTools::delay(3);

					cout << "\n Alive.\n";

					Tools::GameTools::delay(2);

					cout << "And that, perhaps, is enough.\n";
				});
		}
		if (familyBond <1)
		{
			sneakInShip.addChoice("Sooth Phylas with your words; a calming word from his Patér could be enough.", "You lean close, brushing your forehead against his. “Remember the game?” you whisper. 'Quiet as a shadow. Like we practiced.'\n"
				" His breath is ragged, but he nods. He trusts you. And so he buries his fear beside your heartbeat, and stays still.", [&]()
				{
					cout << "The ship groans once more as it pulls from the dock, the sounds of the city swallowed by the sea. Phylas breathes slow against your chest, his fear quieted—- for now.Your partner rests her forehead\n"
						"against your shoulder, eyes shut, jaw tight. Above, the commotion fades into distance, and only the rhythmic churn of oars remains. You don’t know where this vessel leads, or what waits beyond the dark\n"
						" water. But for this moment, your family is with you.\n";

					Tools::GameTools::delay(3);

					cout << "\n Alive.\n";

					Tools::GameTools::delay(2);

					cout << "And that, perhaps, is enough.\n";
				});
		}
		else
		{
			sneakInShip.addChoice("Panic", "Your words falter—there’s nothing left to give him but hope. His lip trembles. And then, a sob. Loud. Too loud.", [&]()
				{
					cout << "The boy’s whimpers slice through the quiet, breaking the fragile silence of the hold. Before you can hush him, heavy footsteps thunder down the stairs. Lantern light floods the cramped space, freezing you\n"
						" in place. Rough hands grab you and your family, pulling you into the harsh glare. There’s no fight left in you—only a cold sinking knowing that you’ve been caught. What happens next is unknown, swallowed by\n"
						" the shadows beyond the hold.\n";

					return;
				});
		}
	}
	else
	{
		Interaction failedSneakInShip = Interaction("You all dart toward the cargo ship, exposed and breathless, hoping the chaos hides your steps.", "You slip between crates, heart hammering, but the illusion of cover shatters under torchlight. A shout rings out—- sharp, foreign—- and rough hands seize you all before you can run. Steel flashes at your throat as you're dragged from the shadows. There’s no escape now,\n"
			" only the cold weight of defeat as you're forced to your knees, breath shallow, surrounded by voices that do not belong to your city.");

		cargoDock.addInteraction(&failedSneakInShip);

		failedSneakInShip.addChoice("Surrender", "", [&]()
			{
				cout << "The invaders close in, silent but for the clink of armor and the rasp of drawn blades.A soldier barks in a tongue not your own—- then hesitates. Eyes flicker to the shadows behind you, to movement in the dark.\n";

				Tools::GameTools::delay(3);

				cout << "\n Your heart strains to follow—- was that them ? Phylas' small frame, a flicker of Anthusa's dress? But the moment passes. A rough hand binds your wrists, and you’re pulled to your feet.You don’t look back.\n";

				Tools::GameTools::delay(3);

				cout << "You can’t.";

				return;

			});
	}

	//GAMEPLAY
	
	stageEnded = false;
	currentLocation = &crossRoad;

	timerRunning = true;
	thread timerThread(startTimer, 5 * 60, stage);

	while (!stageEnded)
	{
		gameplayMenu(stage, currentLocation);
		if (stage > 3)
		{
			stageEnded = true;
		}
	}

	timerRunning = false;
	if (timerThread.joinable())
		timerThread.join();
}

int main()
{
	startScreen();

	Tools::GameTools::clearScreen();
	Tools::GameTools::title();

	while (true)
	{
		switch (stage)
		{
		case 1:
			stageOne();
			break;
		case 2:
			stageTwo();
			break;
		case 3:
			stageThree();
			break;
		default:
			cout << "Invalid \n";
			return 0;
		}
	}

	return 0;
}



