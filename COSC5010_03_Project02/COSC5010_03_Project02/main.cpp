/*
@File: main.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Handles input commands from the user
*/

#include <iostream>
#include <algorithm>
#include <exception>
#include "CharacterInfo.h"

using std::cout;
using std::cin;
using std::endl;
using std::transform;
using std::stoi;
using std::exception;

void initEnemy(CharacterInfo enemy);
int getInt(string value);
double getDouble(string value);

// List of commands that can be used by the user
string commands = "Commands:\n"
"New: Create a new character.\n"
"Fight: fights an enemy.\n"
"Run: Runaway from a fight.\n"
"Rest: Rest your chracter - Cost 10 gold.\n"
"Sell: Sell your items.\n"
"View: See your character information.\n"
"Help: List commands.\n"
"Exit: Exit the game.";

// Driver
int main() {
	// Init
	bool fighting = false;
	CharacterInfo character("Player");
	CharacterInfo enemy("Enemy");
	string input;

	string name, race, _class, hairColor;
	int age, attHP, attMP, attSTM, attDMG;
	double height, weight;
	Gender gender;
	DMGType dmgType;

	// Displaying commands
	cout << commands << endl;

	// Playing
	while (true) {
		// Getting user input
		cout << "Command: ";	// Showing prompt
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
				cout << "Creating cusome character..." << endl;
				// Physique
				cout << "What is your name? ";
				string name;
				cin >> name;
				cout << "Gender (m/f): ";
				cin >> input;
				transform(input.begin(), input.end(), input.begin(), ::toupper);
				if (input.compare("M") == 0) gender = Gender::MALE;
				else gender = Gender::FEMALE;
				cout << "Race: ";
				cin >> race;
				cout << "Class: ";
				cin >> _class;
				cout << "Damage Type <Magic vs Physical> (m/p): ";
				cin >> input;
				transform(input.begin(), input.end(), input.begin(), ::toupper);
				if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
				else dmgType = DMGType::MAGIC;
				while (true) {
					cout << "Age: ";
					cin >> input;
					age = getInt(input);
					if (age < 0) {
						cout << "Age needs to be positive.";
						continue;
					}
					break;
				}

				// Stats
			}
		}
		else if (input.compare("FIGHT") == 0) {
			// Checking if character is dead
			if (character.isDead()) {
				cout << "You are dead, unable to fight. Create a new character." << endl;
				continue;
			}
			// Checking if already fighting
			if (!fighting) {
				initEnemy(enemy);
				cout << "You encounter a " << enemy.getName() << endl;
				fighting = true;
			}
			
			// Fight enemy
			cout << "Use magic or physical attack? (m/p): ";
			cin >> input;
			transform(input.begin(), input.end(), input.begin(), ::toupper);
			if (input.compare("P") == 0) dmgType = DMGType::PHYSICAL;
			else dmgType = DMGType::MAGIC;
			character.fight(enemy, dmgType);

			// Check if enemy died
			if (enemy.isDead()) {
				cout << "You killed the " << enemy.getName() << endl;
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
			cout << "You ran away from the " << enemy.getName() << endl;
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
		else if (input.compare("VIEW") == 0) {
			// Viewing character info
			cout << character.toString() << endl;
		}
		else if (input.compare("HELP") == 0) {
			// Displaying commands
			cout << commands << endl;
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
void initEnemy(CharacterInfo enemy) {
	enemy.newCharacter();
	enemy.setName("I'm an enemy");
}

int getInt(string value) {
	int r;

	try {
		r = stoi(value);
		if (r < 0);
	}
	catch (exception& e) {
		r = -1;
	}

	return r;
}

double getDouble(string value) {
	try {
		return 0;
	}
	catch (exception& e) {

	}
}