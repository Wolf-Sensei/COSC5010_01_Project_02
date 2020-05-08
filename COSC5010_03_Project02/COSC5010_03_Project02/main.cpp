/*
@File: main.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Handles input commands from the user and plays the game
*/

#include <algorithm>	// transform
#include <cstdlib>		// srand, rand
#include <ctime>		// time
#include <exception>	// exeption
#include <iostream>		// cout, cin
#include <sstream>		// istringstream
#include "CharacterInfo.h"

using std::exception;
using std::istringstream;
using std::cout;
using std::cin;
using std::endl;

// === Functions ===

// Initalizes a new enemy of appropriate level and strength
// Stats increase based on level, and how many encounters the player has been in without rest
// Creates the enemy from a random set of names, races, classes, and colors.
// Everything is basically random
void initEnemy(CharacterInfo& enemy, int level, int encounters);
// Turns a string into an int and returns it.
// If it failed, returns -1
int getInt(string value);
// Turns a string into an double and returns it
// If it failed, returns -1
double getDouble(string value);
// Prompts the user for a string using the given prompt
// @toUpper: whether the string should be returned in uppercase or not
string promptString(string prompt, bool toUpper = false);
// Prompts the user for an integer using the given prompt.
// Ensures the value isn't negative and is a valid positive int.
// Will keep asking until a valid integer is given
int promptInt(string prompt);
// Prompts the user for an double using the given prompt.
// Ensures the value isn't negative and is a valid positive double.
// Will keep asking until a valid double is given
double promptDouble(string prompt);

// List of commands that can be used by the user
string commands = "Commands:\n"
"New: Create a new character.\n"
"Fight: fights an enemy.\n"
"Run: Runaway from a fight.\n"
"Rest: Rest your chracter - Cost 10 gold.\n"
"Sell: Sell your items.\n"
"Buy: Buy an attribute point for 25 gold.\n"
"Spend: Spend an attribute point.\n"
"View: See your character information.\n"
"Enemy: See the enemy you are fighting.\n"
"Help: List commands.\n"
"Notes: List helpful notes.\n"
"Exit: Exit the game and saves character.";

// Notes used to help inform the user
string notes =
"Notes:\n"
"Damage depends on damage type, and remaing stats.\n"
"Same damage type is full dmg, otherwise half.\n"
"Damage costs 20 STM or MP. Not enough then 1/4 the damage.\n"
"Enemies get harder the longer you go without rest.";

// Driver
int main() {
	// Init
	int encounters = 0;		// Number of encounters without resting
	bool fighting = false;	// Whether the player is fighting or not
	CharacterInfo character("player.char");	// User character info
	CharacterInfo enemy("enemy.char");		// Enemy character info
	srand(time(NULL));		// Initalizing randomness

	// User input
	string input;	// input command
	vector<string> params;	// If params are added with the command (for fighting and spending)
	// Used for creating a new custom character
	string name, race, _class, hairColor;
	int age, attHP, attMP, attSTM, attDMG, gold;
	double height, weight;
	Gender gender;
	DMGType dmgType;

	// Attempting to load player character info
	if (!character.loadInfo()) {
		// Failed to load information - Either due to tampering or the file never existed
		// Creating default character
		character.newCharacter();
	}

	// Attempting to load enemy character info
	if (enemy.loadInfo()) {
		// Succeeded to load enemy
		// If enemy is alive then fighting will start - (Also can make it so you can fight the legendary panda Huhu)
		// Will set the number of encounters to the player + enemy level - (To avoid "skipping" hard encounters)
		if (!enemy.isDead()) fighting = true;
		encounters = character.getLevel() + enemy.getLevel();
	}
	else {
		// Failed to load information - Either due to tampering or the file never existed
		// Creating default enemy
		enemy.newCharacter("Huhu", Gender::MALE, "Panda", "Legend", DMGType::PHYSICAL, 8012, 412, 680, "Black and White", 8000, 12000, 15000, 9999, 612, 1000000);
	}
	
	// Displaying commands to user
	cout << commands << endl;

	// Playing game
	while (true) {
		// Getting user input
		input = promptString("\nCommand: ", true);
		istringstream iss(input);

		// Parsing parameters (if any)
		params.clear();
		while (iss) {
			iss >> input;
			params.push_back(input);
		}
		input = params[0];	// Getting command

		// Checking commands
		if (input.compare("NEW") == 0) {
			// Getting user input
			input = promptString("Create default character? (y/n): ", true);

			// Checking response
			if (input.compare("Y") == 0) {
				// Default
				cout << "Creating default character...";
				character.newCharacter();
			}
			else {
				// Custom
				cout << "Creating custom character..." << endl;
				// Physique
				cout << "=== Physique ===" << endl;
				name = promptString("What is your name? ");
				input = promptString("Gender <Male or Female> (m/f): ", true);
				if (input.compare("M") == 0) gender = Gender::MALE;
				else gender = Gender::FEMALE;
				race = promptString("Race: ");
				_class = promptString("Class: ");
				input = promptString("Damage Type <Physical or Magic> (p/m): ", true);
				if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
				else dmgType = DMGType::MAGIC;
				age = promptInt("Age (Years): ");
				height = promptDouble("Height (cm): ");
				weight = promptDouble("Weight (kg): ");
				hairColor = promptString("Hair Color: ");

				// Stats
				cout << "=== Stats ===" << endl;
				cout << "Note: Total attribute points used for HP, MP, STM, and DMG for a normal game is 35" << endl;
				cout << "Defaults are in ( )" << endl;
				attHP = promptInt("Attribute HP (10): ");
				attMP = promptInt("Attribute MP (10): ");
				attSTM = promptInt("Attribute STM (10): ");
				attDMG = promptInt("Attribute DMG (5): ");
				gold = promptInt("Starting gold (10): ");

				// Creating character
				character.newCharacter(name, gender, race, _class, dmgType, age, height, weight, hairColor, attHP, attMP, attSTM, attDMG, 1, gold);
			}
			// Finishing
			fighting = false;
			encounters = 0;
			cout << "Finished" << endl;
			cout << character.toString() << endl;
		}
		else if (input.compare("FIGHT") == 0) {
			// Checking if character is dead
			if (character.isDead()) {
				cout << "You are dead, unable to fight. Create a new character." << endl;
				continue;
			}
			// Checking if already fighting
			if (!fighting) {
				// Encountering an enemy
				initEnemy(enemy, character.getLevel(), encounters);
				cout << "You encounter " << enemy.getName() << endl;
				encounters += 1;
				fighting = true;
			}
			
			// Asking for damage type to use
			// Checking for paramters
			if (params.size() > 1) {
				// Setting
				input = params[1];
				if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
				else if (input.compare("M") == 0) dmgType = DMGType::MAGIC;
				else {
					// Prompt
					input = promptString("Use physical or magic attack (p/m): ", true);
					if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
					else dmgType = DMGType::MAGIC;
				}
			}
			else {
				// Prompt
				input = promptString("Use physical or magic attack (p/m): ", true);
				if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
				else dmgType = DMGType::MAGIC;
			}

			// Attacking enemy
			character.fight(enemy, dmgType);

			// Check if enemy or character died
			if (enemy.isDead()) {
				cout << "You killed " << enemy.getName() << endl;
				fighting = false;
			}
			else if (character.isDead()) {
				cout << enemy.getName() << " killed you" << endl;
				fighting = false;
			}
		}
		else if (input.compare("RUN") == 0) {
			// Checking if character is dead or is fighting
			if (character.isDead()) {
				cout << "You are dead, unable to run away. Create a new character." << endl;
				continue;
			}
			else if (!fighting) {
				cout << "There is nothing to run away from." << endl;
				continue;
			}

			// Running away
			fighting = false;
			cout << "You ran away from " << enemy.getName() << endl;
		}
		else if (input.compare("REST") == 0) {
			// Checking if character is dead or is fighting
			if (character.isDead())
				cout << "You are dead, unable to rest. Create a new character." << endl;
			// Checking if character is fighting
			else if (fighting)
				cout << "You are fighting, unable to rest. Run away or finish the fight." << endl;
			// Attempting to rest
			else if (character.rest(10)) {
				cout << "You rested and restored your HP, MP and STM" << endl;
				encounters = 0;
			}
			else {
				cout << "Unable to rest, not enough gold." << endl;
				cout << "Resting costs 10 gold, you have: " << character.getGold() << endl;
			}
		}
		else if (input.compare("SELL") == 0) {
			// Checking if character is dead or is fighting
			if (character.isDead()) {
				cout << "You are dead, unable to sell anything. Create a new character." << endl;
				continue;
			}
			else if (fighting) {
				cout << "You are fighting, unable to sell anything. Run away or finish the fight." << endl;
				continue;
			}

			// Selling items
			int size = character.getItemCount();
			int profit = character.sellItems();
			cout << "Sold " << size << " items for " << profit << " gold." << endl;
		}
		else if (input.compare("BUY") == 0) {
			// Checking if character is dead or is fighting
			if (character.isDead())
				cout << "You are dead, unable to buy attributes. Create a new character." << endl;
			else if (fighting)
				cout << "You are fighting, unable to buy attributes. Run away or finish the fight." << endl;

			// Attemping to buy attribute
			else if (character.buyAttPts(25)) {
				cout << "You bought an attribute point" << endl;
			}
			else {
				cout << "Unable to buy attribute, not enough gold." << endl;
				cout << "Buying an attribute costs 25 gold, you have: " << character.getGold() << endl;
			}
		}
		else if (input.compare("SPEND") == 0) {
			// Checking if character is dead or is fighting
			if (character.isDead()) {
				cout << "You are dead, unable to spend attributes. Create a new character." << endl;
				continue;
			}
			else if (fighting) {
				cout << "You are fighting, unable to spend attributes. Run away or finish the fight." << endl;
				continue;
			}
			else if (character.getAttPts() == 0) {
				cout << "No you have no attribute points to spend. Buy some or level up." << endl;
				continue;
			}

			// Spending point
			// Checking for paramters
			if (params.size() > 1) {
				input = params[1];
				age = getInt(input);	// Getting 0-3
				if (character.spendAttPt(age)) {
					cout << "Attribute spent" << endl;
					continue;
				}
			}
			// Prompting
			cout << "Spend attribute point" << endl;
			if (character.spendAttPt(promptInt("HP(0), MP(1), STM(2), DMG(3) Cancel(4): "))) cout << "Attribute spent" << endl;
		}
		else if (input.compare("VIEW") == 0) {
			// Viewing character info
			cout << character.toString() << endl;
		}
		else if (input.compare("ENEMY") == 0) {
			// Viewing enemy info
			cout << enemy.toString() << endl;
		}
		else if (input.compare("HELP") == 0) {
			// Displaying commands
			cout << commands << endl;
		}
		else if (input.compare("NOTES") == 0) {
			// Displaying notes
			cout << notes << endl;
		}
		else if (input.compare("EXIT") == 0) {
			// Exit game
			// Saveing character and enemy info
			character.saveInfo();
			enemy.saveInfo();
			break;
		}
		else {
			// Invalid command
			cout << "Invalid Command" << endl;
		}
	}

	return 0;
}

// Initalizes a new enemy of appropriate level and strength
// Stats increase based on level, and how many encounters the player has been in without rest
// Creates the enemy from a random set of names, races, classes, and colors.
// Everything is basically random
void initEnemy(CharacterInfo& enemy, int level, int encounters) {
	// Init
	int tmp;
	string name, race, _class, hairColor;
	int age, attHP, attMP, attSTM, attDMG, gold;
	double height, weight;
	Gender gender;
	DMGType dmgType;
	string names[] = { "Ze'roh", "Felow", "Hik", "Neslow", "Flayre", "Chibi", "Mayo", "Jim the Destroyer"};
	string races[] = { "Human", "Elf", "Dwarf", "Demon", "Slime", "Chimera", "Beast", "Goblin"};
	string classes[] = { "Warrier", "Tank", "Thief", "Mage", "Enchanter", "Necromancer"};
	string colors[] = { "Brown", "Blond", "Green", "Blue", "White", "Black", "Purple", "Red", "Cyan" };

	// Creating random character
	// Physique
	name = names[rand() % 8];
	if (rand() % 2) gender = Gender::MALE;
	else gender = Gender::FEMALE;
	race = races[rand() % 8];
	_class = classes[rand() % 6];
	if (rand() % 2) dmgType = DMGType::PHYSICAL;
	else dmgType = DMGType::MAGIC;
	age = (rand() % 301) + 10;
	height = (double)(rand() % 301) + 10;
	weight = (double)(rand() % 301) + 10;
	hairColor = colors[rand() % 9];
	// Stats
	tmp = rand() % 2 + 1;	// Level inc/dec 1-2
	if (rand() % 2) level += tmp;
	else level -= tmp;
	if (level < 1) level = 1;
	attHP = (rand() % 10 + 1) + level * 1.25 * encounters;
	attMP = (rand() % 2 + 1) + level * 1.25 * encounters;
	attSTM = (rand() % 2 + 1) + level * 1.25 * encounters;
	attDMG = (rand() % 2 + 1) + level * 1.25 * encounters;
	gold = (rand() % 11 + 5) + level * 1.5;

	enemy.newCharacter(name, gender, race, _class, dmgType, age, height, weight, hairColor, attHP, attMP, attSTM, attDMG, level, gold);

	// 10% chance to have item - if it gets an item, another chance to get another
	tmp = rand() % 101;	//0-100
	while (tmp < 10) {
		enemy.addItem(rand() % 10 + 1);
		tmp = rand() % 101;
	}
}

// Turns a string into an int and returns it.
// If it failed, returns -1
int getInt(string value) {
	int i;

	try {
		i = stoi(value);
	}
	catch (exception& e) {
		i = -1;
	}

	return i;
}

// Turns a string into an double and returns it
// If it failed, returns -1
double getDouble(string value) {
	// Init
	double d;

	try {
		d = stod(value);
	}
	catch (exception& e) {
		d = -1;
	}

	return d;
}

// Prompts the user for a string using the given prompt
// @toUpper: whether the string should be returned in uppercase or not
string promptString(string prompt, bool toUpper) {
	// Init
	string input;

	cout << prompt;
	getline(cin, input);
	if (toUpper) transform(input.begin(), input.end(), input.begin(), ::toupper);

	return input;
}

// Prompts the user for an integer using the given prompt.
// Ensures the value isn't negative and is a valid positive int.
// Will keep asking until a valid integer is given
int promptInt(string prompt) {
	// Init
	string input;
	int i;

	while (true) {
		cout << prompt;
		getline(cin, input);
		i = getInt(input);
		if (i < 0) {
			cout << "Value needs to be positive." << endl;
			continue;
		}
		break;
	}

	return i;
}

// Prompts the user for an double using the given prompt.
// Ensures the value isn't negative and is a valid positive double.
// Will keep asking until a valid double is given
double promptDouble(string prompt) {
	// Init
	string input;
	double d;

	while (true) {
		cout << prompt;
		getline(cin, input);
		d = getDouble(input);
		if (d < 0) {
			cout << "Value needs to be positive." << endl;
			continue;
		}
		break;
	}

	return d;
}