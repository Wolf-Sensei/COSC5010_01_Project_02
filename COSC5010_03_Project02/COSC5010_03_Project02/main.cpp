/*
@File: main.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Handles input commands from the user
*/

#include <iostream>
#include <algorithm>	// transform
#include <exception>	// exeption
#include <ctime>	// time
#include <cstdlib>	// srand, rand
#include "CharacterInfo.h"

using std::cout;
using std::cin;
using std::endl;
using std::transform;
using std::stoi;
using std::stod;
using std::exception;
using std::rand;
using std::time;

void initEnemy(CharacterInfo& enemy, int level, int rest);
int getInt(string value);
double getDouble(string value);
string promptString(string prompt);
int promptInt(string prompt);
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

string notes =
"Notes:\n"
"Damage depends on damage type, and remaing stats.\n"
"Same damage type is full dmg, otherwise half.\n"
"Damage costs 20 STM or MP. Not enough then 1/4 the damage.\n"
"Enemies get harder the longer you go without rest.";

// Driver
int main() {
	// Init
	int rest = 0;
	bool fighting = false;
	CharacterInfo character("player.char");
	CharacterInfo enemy("enemy.char");
	if (!character.loadInfo()) {
		character.newCharacter();
		cout << "Failed to load info?" << endl;
	}
	if (enemy.loadInfo()) {
		if (!enemy.isDead()) fighting = true;
		rest = character.getLevel() + enemy.getLevel();
	}
	else {
		enemy.newCharacter("Huhu", Gender::MALE, "Panda", "Legend", DMGType::PHYSICAL, 8012, 412, 680, "Black and White", 8000, 12000, 15000, 9999, 612, 1000000);
	}
	string input;

	string name, race, _class, hairColor;
	int age, attHP, attMP, attSTM, attDMG, gold;
	double height, weight;
	Gender gender;
	DMGType dmgType;

	srand(time(NULL));

	// Displaying commands
	cout << commands << endl;

	// Playing
	while (true) {
		// Getting user input
		cout << "\nCommand: ";	// Showing prompt
		cin >> input;			// Getting command
		transform(input.begin(), input.end(), input.begin(), ::toupper);

		// Checking commands
		if (input.compare("NEW") == 0) {
			// Creating a new character
			cout << "Create default character? (y/n): ";
			cin >> input;
			transform(input.begin(), input.end(), input.begin(), ::toupper);

			// Checking response
			if (input.compare("Y") == 0) {
				// Default
				cout << "Creating default character...";
				character.newCharacter();
				cout << " Finished" << endl;
			}
			else {
				// Custom
				cout << "Creating custom character..." << endl;
				cout << "Note: Answers are seperated by spaces" << endl;
				// Physique
				cout << "=== Physique ===" << endl;
				name = promptString("What is your name? ");
				input = promptString("Gender <Male or Female> (m/f): ");
				transform(input.begin(), input.end(), input.begin(), ::toupper);
				if (input.compare("M") == 0) gender = Gender::MALE;
				else gender = Gender::FEMALE;
				race = promptString("Race: ");
				_class = promptString("Class: ");
				input = promptString("Damage Type <Magic or Physical> (m/p): ");
				cout << "Damage Type <Magic or Physical> (m/p): ";
				transform(input.begin(), input.end(), input.begin(), ::toupper);
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
				cout << "Finished" << endl;
			}
			cout << character.toString() << endl;
			fighting = false;
			rest = 0;
		}
		else if (input.compare("FIGHT") == 0) {
			// Checking if character is dead
			if (character.isDead()) {
				cout << "You are dead, unable to fight. Create a new character." << endl;
				continue;
			}
			// Checking if already fighting
			if (!fighting) {
				initEnemy(enemy, character.getLevel(), rest);
				cout << "You encounter " << enemy.getName() << endl;
				rest += 1;
				fighting = true;
			}
			
			// Fight enemy
			input = promptString("Use physical or magic attack (p/m): ");
			transform(input.begin(), input.end(), input.begin(), ::toupper);
			if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
			else dmgType = DMGType::MAGIC;
			character.fight(enemy, dmgType);

			// Check if enemy died
			if (enemy.isDead()) {
				cout << "You killed " << enemy.getName() << endl;
				fighting = false;
			}
		}
		else if (input.compare("RUN") == 0) {
			// Checking if character is dead
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
			// Checking if character is dead
			if (character.isDead()) {
				cout << "You are dead, unable to rest. Create a new character." << endl;
				continue;
			}
			else if (fighting) {
				cout << "You are fighting, unable to rest. Run away or finish the fight." << endl;
				continue;
			}
			// Attempting to rest
			if (character.rest(10)) {
				cout << "You rested and restored your HP, MP and STM" << endl;
				rest = 0;
			}
			else {
				cout << "Unable to rest, not enough gold." << endl;
				cout << "Resting costs 10 gold, you have: " << character.getGold() << endl;
			}
		}
		else if (input.compare("SELL") == 0) {
			// Checking if character is dead
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
			// Checking if character is dead
			if (character.isDead()) {
				cout << "You are dead, unable to buy attributes. Create a new character." << endl;
				continue;
			}
			else if (fighting) {
				cout << "You are fighting, unable to buy attributes. Run away or finish the fight." << endl;
				continue;
			}
			// Attemping to buy attribute
			if (character.buyAttPts(25)) {
				cout << "You bought an attribute point" << endl;
			}
			else {
				cout << "Unable to buy attribute, not enough gold." << endl;
				cout << "Buying an attribute costs 25 gold, you have: " << character.getGold() << endl;
			}
		}
		else if (input.compare("SPEND") == 0) {
			// Checking if character is dead
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
			cout << "Spend attribute point" << endl;
			if (character.spendAttPt(promptInt("HP(0), MP(1), STM(2), DMG(3) Cancel(4): "))) cout << "Attribute spent" << endl;
		}
		else if (input.compare("VIEW") == 0) {
			// Viewing character info
			cout << character.toString() << endl;
		}
		else if (input.compare("ENEMY") == 0) {
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

// Initalizes a new enemy
void initEnemy(CharacterInfo& enemy, int level, int rest) {
	// Init
	int i, i2;
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
	i = rand() % 8;	//0-7
	name = names[i];
	if (i < 4) gender = Gender::MALE;
	else gender = Gender::FEMALE;
	i = rand() % 8;	//0-7
	race = races[i];
	i = rand() % 6;	//0-5
	_class = classes[i];
	if (i < 3) dmgType = DMGType::PHYSICAL;
	else dmgType = DMGType::MAGIC;
	i = rand() % 9;	//0-8
	hairColor = colors[i];
	age = rand() % 300 + 50;
	height = rand() % 300 + 50;
	weight = rand() % 300 + 50;	
	// Stats
	i = rand() % 2 + 1;
	i2 = rand() % 2;
	if (i2) level += i;
	else level -= i;
	if (level < 1) level = 1;
	attHP = (rand() % 10 + 1) + level * 1.25 * rest;
	attMP = (rand() % 2 + 1) + level * 1.25 * rest;
	attSTM = (rand() % 2 + 1) + level * 1.25 * rest;
	attDMG = (rand() % 2 + 1) + level * 1.25 * rest;
	gold = (rand() % 11 + 5) + level * 1.5;

	enemy.newCharacter(name, gender, race, _class, dmgType, age, height, weight, hairColor, attHP, attMP, attSTM, attDMG, level, gold);
}

// Turns a string into an int and returns it.
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

string promptString(string prompt) {
	// Init
	string input;

	cout << prompt;
	cin >> input;

	return input;
}

// Prompts for an integer using the given prompt.
// Ensures the value isn't negative and is a valid positive int.
int promptInt(string prompt) {
	// Init
	string input;
	int i;

	while (true) {
		cout << prompt;
		cin >> input;
		i = getInt(input);
		if (i < 0) {
			cout << "Value needs to be positive." << endl;
			continue;
		}
		break;
	}

	return i;
}

double promptDouble(string prompt) {
	// Init
	string input;
	double d;

	while (true) {
		cout << prompt;
		cin >> input;
		d = getInt(input);
		if (d < 0) {
			cout << "Value needs to be positive." << endl;
			continue;
		}
		break;
	}

	return d;
}