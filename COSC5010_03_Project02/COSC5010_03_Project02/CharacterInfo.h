/*
@File: CharacterInfo.h
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: header file for declaring the Info class object
*/

#ifndef CHARACTERINFO_H
#define CHARACTERINFO_H

#include <string>
#include <vector>
#include "Item.h"

using std::string;
using std::vector;

enum struct Gender {MALE, FEMALE};
enum struct DMGType {MAGIC, PHYSICAL};

class CharacterInfo {
    // === Functions === 
public:
    // =========================
    // === Constructors ===
    // =========================

    // Constructor takes in a filename for where character information is saved and loaded from
    // Also initalizes the character
    CharacterInfo(string filename); // Default Constructor

    // =========================
    // === Save & Load ===
    // =========================

    // Save character information into a file
    // Gathers character information and creates a hash-signature of it
    // Encrypts the character information
    // Signature and encrypted information are put into the file
    // Signature is used to ensure that the data cannot be tampered with
    // Encryption is used to hide character information in file
    // Returns true if the information was successfully saved and false otherwise
    bool saveInfo();

    // Load character information from a file
    // Reads in a signature line and encrypted data
    // After decrypting the data, a signature is generated from it
    // If the read and calculated signatures match then the data wasn't tampered
    // If the data was tampered with then returns false
    // Otherwise it sets the character data to what was loaded
    // Returns true if the information was loaded successfully and false otherwise
    bool loadInfo();

    // =========================
    // === Character Events ===
    // =========================

    void newCharacter();    // Creates a new character with default values

    // Creates a new character with the given values
    void newCharacter(string name, Gender gender, string race, string _class, DMGType dmgType,
        int age, double height, double weight, string hairColor,
        int attHP, int attMP, int attSTM, int attDMG, int level, int gold);

    // Levels up the character
    // level+1, attPts+5, glyphPts+1, xp->0, maxXP^
    // Restores HP, MP, STM
    // Returns current level
    int levelUp();

    // =========================
    // === Character Actions ===
    // =========================

    // Reduce stats and gain rewards based on current level
    // First does damage to enemy, if enemy dies then loots
    // Otherwise recieve damage
    // Returns true if the fight was successful, and false otherwise
    bool fight(CharacterInfo& enemy, DMGType type);

    // The character rests at an Inn.
    // The inn costs gold to rest at.
    // Character HP, MP, and STM are fully restored
    // Returns true if resting was successful and false otherwise
    bool rest(int gCost);

    // Sells all items in the characters inventory.
    // The value of all the items is put into the characters gold supply
    // Returns the amount of gold gained
    int sellItems();

    // =========================
    // === Character Utility ===
    // =========================

    // Buys an attribute pt for the character
    // Returns true if the purchase was successful and false otherwise
    bool buyAttPts(int gCost);

    // Increase a character attribute and increase
    // max HP, max MP, Max STM, or DMG
    // Returns true if point allocation was successful and false otherwise
    bool spendAttPt(int att);

    // =========================
    // === Utility ===
    // =========================

    void updateStats(); // Updates stats for max HP, MP, STM, and DMG
    string toString();  // Converts character info into an easily readable string

    // =========================
    // === Getters & Setters & Modifs ===
    // =========================

    // Setters
    // - Ensures values aren't negative
    // - Returns true if value was valid, and false otherwise
    // Physique
    void setName(string name) { this->name = name; }
    void setGender(Gender gender) { this->gender = gender; }
    void setRace(string race) { this->race = race; }
    void setClass(string _class) { this->_class = _class; }
    void setDmgType(DMGType type) { dmgType = type; }
    bool setAge(int age);
    bool setHeight(double height);
    bool setWeight(double weight);
    void setHairColor(string color) { hairColor = color; }
    // Stats
    // - Set HP/MP/STM/DMG ignores attributes;
    //   if attributes are updated HP/MP/STM/DMG will reset to proper values
    // - Set HP can cause the character to die
    bool setHP(int hp, bool max = false);
    bool setMP(int mp, bool max = false);
    bool setSTM(int stm, bool max = false);
    bool setDMG(int dmg);
    // - Attributes are for HP(0), MP(1), STM(2), and DMG(3)
    bool setAttribute(int att, int addition);
    // Growth
    // - Minimum level is 1
    bool setLevel(int level);
    // - If current XP is over maxXP, a levelup occurs
    bool setXP(int xp, bool max = false);
    bool setAttPts(int pts);
    // States
    void setIsDead(bool dead) { this->dead = dead; }
    // Inventory
    bool setGold(int gold);

    // Modifs
    // - Modifies a value by a set amount
    //   Can be positive or negative
    // - Ensures final values aren't negative
    //   If they are, puts them at the minimum values
    // - Returns the final value back
    // Stats
    // - Mod HP/MP/STM/DMG ignores attributes;
    //   if attributes are updated HP/MP/STM/DMG will reset to proper values
    // - Mod HP can cause the character to die
    int modHP(int hp, bool max = false);
    int modMP(int mp, bool max = false);
    int modSTM(int stm, bool max = false);
    int modDMG(int dmg);
    // - Attributes are for HP(0), MP(1), STM(2), and DMG(3)
    int modAttribute(int att, int amount);
    // Growth
    // - Does not chance max or current XP
    // - Max and current XP will go to proper values if a levelup occurs
    int modLevel(int level);
    // - If current XP is over maxXP, a levelup occurs
    int modXP(int xp, bool max = false);
    int modAttPts(int pts);
    // Inventory
    // - Adds an item to the character inventory with the given value
    void addItem(int value);
    // - Returns number of items that were cleared
    int clearInventory();
    int modGold(int gold);

    // Getters
    // Physique
    string getName() { return name; }
    Gender getGender() { return gender; }
    string getRace() { return race; }
    string getClass() { return _class; }
    DMGType getDMGType() { return dmgType; }
    int getAge() { return age; }
    double getHeight() { return height; }
    double getWeight() { return weight; }
    string getHairColor() { return hairColor; }
    // Stats
    int getHP(bool max = false);
    int getMP(bool max = false);
    int getSTM(bool max = false);
    int getDMG() { return DMG; }
    int* getAttributes() { return attributes; }
    // Growth
    int getLevel() { return level; }
    int getXP(bool max = false);
    int getAttPts() { return attributePts; }
    // States
    bool isDead() { return dead; }
    // Inventory
    vector<Item> getInventory() { return inventory; }
    int getItemCount() { return inventory.size(); }
    int getGold() { return gold; }

private:
    // =========================
    // === Save & Load ===
    // =========================

    // Returns the character information as a string seperated by newlines
    string getInfo();

    // Returns a hashed value of the given string (signature)
    // Is reproducable between runs
    int getSig(string str);

    // Returns a encrypted or decrypted string of the given info
    string XOR(string str);

    // =========================
    // === Values ===
    // =========================

    string filename;    // File to save data to and load data from

    // === Character ===
    // Physique
    string name;
    Gender gender;      // female or male
    string race;        // Human, Dwarf, Demon, Beast, etc
    string _class;      // Warrier, THief, Mage, etc
    DMGType dmgType;    // physical or magic
    int age;            // in years
    double height;      // in Cm
    double weight;      // in kg
    string hairColor;

    // Stats
    int maxHP, HP;      // Health
    int maxMP, MP;      // Mana
    int maxSTM, STM;    // Stamina
    int DMG;            // Damage
    int attributes[4];  // Addition attributes for HP, MP, STM, and dmg

    // Growth
    int level;          // Level
    int maxXP, XP;      // Experience needed to gain level
    int attributePts;   // Increase max hp, mp, stm, dmg

    // States
    bool dead;          // Whether the character is dead or not

    // Inventory
    vector<Item> inventory; // Inventory containing tiems (items only hold a gold value)
    int gold;
};

#endif // !CHARACTERINFO_H