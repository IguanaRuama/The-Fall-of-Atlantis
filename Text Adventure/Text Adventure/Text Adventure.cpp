
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

atomic<bool> timerRunning(false);
atomic<bool> paused(false);
mutex mtx;
condition_variable cv;

string input;
int choice;
int stage;
int familyBond = 0;
bool familyPresent = true;

Inventory playerInventory = Inventory();
Currency playerCurrency = Currency(0, "Drachmae-- a shimmering coin forged of gold; small trinkets that hold plentiful meaning.");
Location* currentLocation;

void onTimerEnd(int stage)
{
	timerRunning = false;
	switch (stage)
	{
	case 1:
		cout << "The day shifts in the smallest of ways. A faint tremor passes beneath your feet—so gentle it could be mistaken for a passing cart-—\n"
			" but something in it makes you pause. The fig tree rustles though there is no wind. Far off, beyond the rooftops, a dark smudge lingers\n"
			" on the horizon. Ships, perhaps... but too many. Too still. No one else seems to notice. Phylas hums to himself as he balances a stick along\n"
			" the courtyard wall, and Anthusa wipes her hands on her apron, focused on her herbs. But your stomach knots. A feeling—- subtle, sharp, and\n"
			" certain—- takes hold. Without sounding the alarm, you urge your family to gather what you need and head to the town center. Just in case.\n";
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
	timerRunning = true;
	cout << "Time is limited. Rhesus has " << (secs / 60) << " minutes.\n";

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

void pauseTimer()
{
	paused = true;
	cout << "\nTimer paused.\n";
}

void resumeTimer()
{
	paused = false;
	cout << "Timer resumed.\n";
	cv.notify_all();
}

void nextStage(int& stage)
{
	// Ensure we don't transition past the last stage
	if (stage < 3)
	{
		cout << "You rush to the next stage of the invasion" << stage << "!\n";
		onTimerEnd(stage); // Transition to next stage
		stage++;
	}
	else
	{
		cout << "end";
	}
}

void startMenu()
{

}

void itemActionsMenu(Inventory* inventory, Item* item, int stage)
{
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

void choicesMenu(Inventory* inventory, Interaction* interaction, int stage)
{
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
			itemActionsMenu(inventory, interaction->getRequiredItems(inventory)[choice], stage);

			return;
		}
		else
		{
			cout << "Invalid input.\n";
		}
	}
}

void interactionMenu(Location* location, int stage)
{
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
				choicesMenu(&playerInventory, location->getInteractions()[choice], stage);
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

void travelMenu(Location*& location, int stage) 
{
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

void gameplayMenu(int stage, Location* location)
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
			travelMenu(location, stage);
			break;

		case 3:
			playerInventory.outputInventory();

			cout << "Press 'Enter' to return to menu.\n";

			//ADD INVENTORY MENU TO FUNCTIONS, ALLOW PLAYERS TO EXAMINE + DISCARD IN MENU,
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
				Tools::GameTools::exitGame;
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
			break;
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

		Item patioKey = Item("Patio Key", "A familiar sight that unlocks your front door", 0);
		patioKey.setDefaultActions();


		Interaction enterHome("Enter your home.", "Simple yet sturdy, the door to your home shields the warmth of your family from the hardships of the world. A worn lock finds its own home at\n"
			" the edge of the wooden barrier.");

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
				cout << "Adding kitchen to current location: " << currentLocation->getLocationName() << endl;
				currentLocation->addLink(&kitchen);
			});
		//add key and link

		Interaction explorePatio("Explore the patio", "You stand before your humble home; Phylas remains locked in imaginary battle whilst Anthusa oversees-- her lips curling as he\n"
			" slays whatever beast dared cross hin.");
		explorePatio.addChoice("Join Phylas.", "With a swift scoop you lift Phylas into the air, swinging and spinning the fearsome hero as his stern facade erupts into childish\n"
			" giggles. The sight is enough to bring a subtle smile to Anthusa's face accompanied by a quiet tutting. With arms weary from sparring, you place your son back upon\n"
			" his feet, leaving the 'soldier' back to his battle.",
			[&]() 
			{
				familyBond += 1;
			});
		//+1 fam

		explorePatio.addChoice("Greet Anthusa.", "You step toward Anthusa, her hands paused in their work as she looks up at you with that familiar, quiet smile. The sunlight\n"
			" catches in her hair, and for a moment, the world narrows to just the two of you. She brushes a bit of soil from her palms onto her tunic and says, 'Back already\n"
			"? Or did the soldiers grow tired of you showing them how it's done?' There’s warmth in her voice—teasing, but gentle—the kind that wraps around your tired bones like\n"
			"a well-worn blanket. She slips a small cloth pouch into your hand. 'Here—- three drachmae. I took the olives and greens to the market this morning.The baker paid fair.\n"
			" She shrugs, modest as ever. 'Might be enough for oil... or whatever it is you warriors waste coin on when you're pretending not to be sore.'",
			[&]() {
				playerCurrency.addMoney(5);
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
				cout << "Inventory size before: " << playerInventory.getItems().size() << "\n";
				playerInventory.takeItemChoice(&patioKey);
				cout << "Inventory size after: " << playerInventory.getItems().size() << "\n";
			});
		// add key

		frontPatio.addInteraction(&enterHome);
		frontPatio.addInteraction(&explorePatio);
		
		//KITCHEN INTERACTIONS

		Item spices = Item("Bundle of Spices", "A small pouch consisting of a variety of spices; courtesy of your wife", 5);
		spices.setDefaultActions();

		Item fish = Item("A cured fish", "A slimy, smelly, cooked creature of the sea", 15);
		fish.setDefaultActions();

		Item knife = Item("A Noticably Sharp Kitchen Knife", "A knife who often resides in your kitchen, perfectly sharpened by the hands of your wife", 10);
		knife.setDefaultActions();
		Interaction explorekitchen = Interaction("", "");
		
		explorekitchen.addChoice("Investigate amphorae.", "Lined neatly against the kitchen wall, the clay amphorae stand tall and cool to the touch, their wide mouths sealed with waxed cloth.\n"
			" As you lift the cover from one marked with a faded fish symbol, a salty scent rises to meet you. Nestled in layers of dried herbs and coarse brine, a few small, cured fish\n"
			" glisten faintly in the dim light—ready to be taken, traded, or cooked.",
			[&]() 
			{
				playerInventory.takeItemChoice(&fish);
			});
		//add fish to inventory

		explorekitchen.addChoice("Investigate motar and pestle.", "Set in the corner of the worn stone counter, the mortar is heavy and well-used, its bowl dusted with traces of ground spice.\n"
			" Resting beside it, a small bundle of fresh herbs- —rosemary, thyme, and bay leaf—has been tied with twine, ready for use. The scent is sharp and earthy, rising as you reach out\n"
			" to take it, a piece of the kitchen’s soul wrapped in green.",
			[&]() 
			{
				playerInventory.addItem(&spices);
			});
		//add herbs

		//make a one time interaction
		Interaction takeKnifeEncounter = Interaction("Investigate the stone counter.", "The stone counter runs along the kitchen wall, its surface worn smooth by years of chopping, kneading, and quiet labor .Amoung the\n"
			" scattered sprigs of herbs and a dusting of flour lies a single iron-bladed knife—well-used, its wooden handle darkened by time and touch. It rests just within reach, unassuming\n"
			" but sharp, a tool of the home that could just as easily become something else.\n"
			"\n As your fingers curl around the knife’s worn handle, Anthusa looks up from her basket with a puzzled frown. 'Rhesus?' she says softly, brushing a lock of hair from her brow.\n"
			" 'What do you need that for?' There’s no anger in her voice—only curiosity, and a hint of concern.\n"
			"\n She watches you now, herbs forgotten in her hands.'You’re not headed back to the barracks, are you?' Her tone shifts, just slightly. Not accusing, but wary.");

		takeKnifeEncounter.addChoice("Take the knife anyway; just encase.", "You lift the knife from the counter, its weight familiar in your hand. 'Just in case,' you mutter, not meeting Anthusa’s\n"
			" eyes. 'For protection.'\n"
			"\n She doesn’t answer at first. Her gaze lingers on you, steady and unreadable, a bundle of thyme still held loosely in her fingers.The silence stretches.You expect a rebuke, maybe a\n"
			" sigh—- but instead, she simply turns back to her work, quieter than before.\n"
			"\n The excuse hangs in the air between you, thin and unconvincing. Whatever danger you’re guarding against, she’s not sure she believes it-— or worse, that you do.",
			[&]() 
			{
				playerInventory.addItem(&knife);
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

		kitchen.addInteraction(&explorekitchen);
		kitchen.addInteraction(&takeKnifeEncounter);

		//CHILDRENS ROOM

		Item toy = Item("Phylas' Wooden Soldier", "A handmade miniature soldier belonging to your son.", 5);
		toy.setDefaultActions();

		Item slingshot = Item("Phylas' Slingshot", "A small, yet slightly dangerous, toy of your sons.", 5);
		slingshot.setDefaultActions();

		Interaction exploreChildsRoom = Interaction("Admire Phylas' toys.", "Phylas’ toy collection is a patchwork of imagination and found treasure—wooden soldiers with chipped paint, smooth river stones\n"
			" pretending to be treasure, and a bundle of twine tangled into what he insists is a sea serpent. Each piece is worn with play and affection, scattered across his room in small, heroic\n"
			" scenes. To anyone else, it’s a mess; to Phylas, it’s an empire in motion.");

		exploreChildsRoom.addChoice("Rummage in the wooden chest", "You kneel beside the squat wooden chest and lift the lid, greeted by the soft clatter of childhood treasures. Digging past\n"
			" smooth pebbles, bits of string, and a shell with a hole in it, your fingers close around a familiar shape—roughly carved, worn at the edges. It’s a small wooden soldier, notched at the\n"
			" shoulder, with a charcoal mark across its chest like your old training tunic. Phylas must have made it to resemble you. Its stance is crooked but proud, a tiny guardian standing amid the\n"
			" clutter of a child’s world.",
			[&]()
			{
				playerInventory.addItem(&toy);
			});
		// take toy

		Interaction takeSlingshotEncounter = Interaction("Admire his drawings.", "You pause by the wall, eyes tracing the faded mural of dolphins and chariots, now joined by Phylas’ own additions—wobbly\n"
			" stick-figures locked in battle, a towering cyclops looming over a tiny spear-wielding hero. There’s charm in the rough lines, a fierce little heart behind every stroke. As you smile, your gaze\n"
			" drifts upward to a dusty shelf, where a familiar shape catches your eye: the slingshot you carved for him last spring, its yew frame still bound with the same bit of worn leather. You hadn’t\n"
			" realized he kept it so close. If you want to take it with you, you’ll need to choose your words carefully—- it's still his, after all.");

		takeSlingshotEncounter.addChoice("Gently request the slingshot from your son.", "You gesture toward the shelf with a faint smile, eyes on the slingshot nestled between a pile of pebbles and a wax-sealed seashell.\n"
			" 'You’ve kept the old slingshot,' you say, stepping closer. 'Mind if I borrow her for a bit? Just in case I run into any troublemakers.'\n"
			"\n Phylas looks up from the floor where he’s been arranging his toy soldiers into a lopsided battle line.He blinks, then gives a small, reluctant nod.\n"
			"\n 'Alright... but you better bring it back. He’s got a name now—- Thornstriker.'\n"
			"\n You lift it gently, the leather still taut, the weight of it surprisingly familiar in your grip.",
			[&]()
			{
				playerInventory.addItem(&slingshot);
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
			});
		// -1 fam

		takeSlingshotEncounter.addChoice("Utilise sentimental restraint-- you don't need it, its your sons.", "You reach out, then pause—fingers brushing the wooden frame, the smooth curve you once carved by firelight.\n"
			"\n You smile faintly and lower your hand.\n"
			"\n 'You’ve kept it well,' you murmur. 'Better it stays here, where it belongs.'\n"
			"\n Phylas looks up at you, surprised.Then, more softly, 'I keep it close in case monsters come through the window.'\n"
			"\n He taps his chest. 'But I sleep easier knowing you made it.'\n"
			"\n You ruffle his hair gently and let the moment stand. The slingshot stays on the shelf, right where it should be.",
			[&]()
			{
				familyBond+=1;
			});

		childrenRoom.addInteraction(&exploreChildsRoom);
		childrenRoom.addInteraction(&takeSlingshotEncounter);

		//BEDROOM

		Item cloak = Item("A Hooded Cloak", "A large garment that could easily conceal its wearer.", 2);
		cloak.setDefaultActions();

		Item oils = Item("Anthusa's Oils", "A collection of oils including made by your lover.", 5);
		oils.setDefaultActions();

		Item comb = Item("Anthusa's Comb of Bone", "A family heirloom; a comb carved from animal bone.", 5);
		comb.setDefaultActions();

		Interaction exploreBedroom = Interaction("Wander through your bedroom.", "A woolen cloak, frayed at the hem from years of use, hangs over a peg near the doorway, still carrying the faint scent of ash and laurel.\n"
			" On a low wooden shelf sits a small collection of clay vials—- Anthusa’s precious oils—pressed from lavender, myrtle, and crushed olives, their stoppers stained with use. Beside them lies a delicate\n"
			" bone comb, its teeth worn smooth from years of tending curls and quiet mornings. The room holds little in the way of luxury, but every item speaks of care, routine, and a life gently shared.");

		exploreBedroom.addChoice("Investigate the cloak", "You step toward the cloak, its coarse wool brushing softly against the wall where it hangs. The fabric is thick, weathered, and heavy with the scent of smoke and\n"
			" sun-— your scent. Faint stains from old marches and scattered threads at the hem speak of long days on the road and nights beneath uncertain skies. As your fingers trace the worn edges, a small copper\n"
			" brooch glints at the collar—Anthusa’s handiwork, fastened there long ago. It's more than just a soldier’s cloak. It’s a quiet ledger of where you’ve been… and who waited for you to come home.",
			[&]()
			{
				playerInventory.addItem(&cloak);
			});
		// add cloak

		exploreBedroom.addChoice("Inspect Anthusa's oils.", "Lined neatly along a low shelf, Anthusa’s oils sit in small clay vials, their rounded forms smoothed by years of gentle use. You lift one—lavender, by the\n"
			" faint, sweet scent that clings to the rim—and the cool clay warms quickly in your hand. Each stopper is stained with traces of its contents: gold from pressed olives, pale green from crushed bay, a\n"
			" dark swirl where rosemary once steeped. They aren’t luxuries, not here—they’re tools of care, of comfort. These quiet jars hold the rhythms of the home: healing, soothing, softening what the world\n"
			" wears down.",
			[&]()
			{
				playerInventory.addItem(&oils);
			});
		//add oils

		exploreBedroom.addChoice("Admire Anthusa's comb.", "Resting beside the oils, the bone comb lies quiet and familiar, its ivory surface smoothed to a gentle sheen by years of use. The teeth are fine, some slightly\n"
			" crooked from time or tugged tangles, but none broken—a testament to its care. You pick it up, light as breath, and feel the faint warmth still clinging to it. Traces of oil linger between the prongs,\n"
			" scented with myrtle and rosemary. This was never just for grooming—it was part of Anthusa’s morning ritual, each stroke a moment of calm, a reclaiming of order before the day began. It’s a humble thing,\n"
			" yet it hums with the presence of its owner.",
			[&]()
			{
				playerInventory.addItem(&comb);
			});
		//add comb

		bedroom.addInteraction(&exploreBedroom);

		//COURTYARD

		Item vegetables = Item("Bundle of Vegetables", "A small bag containing fresh vegetables from your garden", 10);
		vegetables.setDefaultActions();

		Interaction exploreGarden = Interaction("Admire the courtyard.", "Near the fig tree—- bent with age, yet generous with fruit—- sits a burlap sack, half-tucked beneath the garden bench. Inside, a handful of vegetables\n"
			" remain from Anthusa’s morning at the market: onions, a few soft tomatoes, and sprigs of greens still damp from dew and handling. She must have dropped it here in her rush to tend the garden, or to chase\n"
			" Phylas from climbing the lemon bush again. It rests quietly now, a simple thing—- humble proof of care, of labor, of the home’s quiet pulse.");

		exploreGarden.addChoice("Inspect the sack.", "You crouch beside the bench and pull the sack into better light. The rough fabric is damp in places, darkened with soil and the scent of crushed herbs. Inside, a modest harvest\n"
			" nestles together—wilted greens, a few onions with dirt still clinging to their roots, and two bruised tomatoes softening at the edges. It’s what remains after a morning at the market, set aside and nearly forgotten.\n"
			" The weight of it is light, but it carries the shape of a day’s work—- Anthusa’s careful hands, trading quietly for coin, never wasting what could still feed a home.",
			[&]()
			{
				playerInventory.addItem(&vegetables);
			});
		//add veges

		courtyard.addInteraction(&exploreGarden);

		//GAMEPLAY

		stage = 1;
		currentLocation = &frontPatio;

		cout << "With each breath the tase of salt remains prevalent in the City of Atlantis; a flavour you've grown awfully atuned to as breaths come deeper\n"
			" and shorter following another spar with your brother in arms. Though as spears flew and laughter ached more that bruises, it's a time you've\n"
			" grown to adore. Your boots are heavy with the dust of training grounds, yet oh-so light as you meet the cobble of your front porch.\n"

			"\n As always, you are greeted with an image you believe is a blessing from Hestia-- Phylas, your spirited young son, chasing shadows with an\n"
			" olive branch, and Anthusa, a woman whose creation could only be that of Aphrodites beauty and Philophrosynes strength; your wife, watching \n"
			" with a calm that doesn’t quite reach her eyes.\n"

			"\n All thats needed to complete the scene is you, Rhesus. \n";
		cout << "Press 'Enter' to continue.\n";
		getline(cin, input);

		thread timerThread(startTimer, 5 * 60, stage);
		
		while (true)
		{
			gameplayMenu(stage, currentLocation);
		}

		//inventory doesn't display
		//travel link doesnt display
		//add new desciption to open door when key used
		//

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
	Location alleyway = Location("", "", "");
	Location northMarket = Location("", "", "");
	Location eastMarket = Location("", "", "");
	Location southMarket = Location("", "", "");
	Location westMarket = Location ("", "", "");

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

}

int main()
{
	Tools::GameTools::title();
	stageOne();
	//function intro
	//function main menu
	// stage 1
	//stage 2
	//stage 3
	//end

}



