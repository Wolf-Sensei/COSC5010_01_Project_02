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
    // Constructors
    CharacterInfo(string filename); // Default Constructor

    // Save & Load Info
    bool saveInfo();
    bool loadInfo();

    // Character Events
    void newCharacter();    // Sets default character stats
    void newCharacter(string name, Gender gender, string race, string _class, DMGType dmgType,
        int age, double height, double weight, string hairColor,
        int attHP, int attMP, int attSTM, int attDMG, int level, int gold);
    int levelUp();          // Leveup character, gain at/gl pts, inc level, reset xp, inc max xp

    // Character Actions
    bool fight(CharacterInfo& enemy, DMGType type);        // Get:items, gold, xp ; Cost: HP, Mana, STM ; Can die
    bool rest(int gCost); // Restore lost stats, lose gold, gain resting bonus
    int sellItems();                        // Sell items for gold

    // Character Utility
    bool buyAttPts(int gCost);      // Upgrade hp,mp,stm,dmg
    bool spendAttPt(int att);       // Increase max HP, MP, STM, or DMG

    // Utility
    // Character Info string
    void updateStats();
    string toString();

    // Getters & Setters & modifs
    // Setters
    // Physique
    void setName(string name);
    void setGender(Gender gender);
    void setRace(string race);
    void setClass(string _class);
    void setDmgType(DMGType type);
    bool setAge(int age);
    bool setHeight(double height);
    bool setWeight(double weight);
    void setHairColor(string color);
    // Stats
    bool setHP(int hp, bool max = false);
    bool setMP(int mp, bool max = false);
    bool setSTM(int stm, bool max = false);
    bool setDMG(int dmg);
    bool setAttribute(int att, int addition);
    // Growth
    bool setLevel(int level);
    bool setXP(int xp, bool max = false);
    bool setAttPts(int pts);
    // States
    void setIsDead(bool dead);
    // Inventory
    bool setGold(int gold);

    // Modify stat by +- amount
    // Stats
    int modHP(int hp, bool max = false);
    int modMP(int mp, bool max = false);
    int modSTM(int stm, bool max = false);
    int modDMG(int dmg);
    int modAttribute(int att, int amount);
    // Growth
    int modLevel(int level);
    int modXP(int xp, bool max = false);
    int modAttPts(int pts);
    // Inventory
    void addItem(int value);
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
    string getInfo();
    int getSig(string str);
    string XOR(string str);
    
    // === Values ===
private:
    // === File ===
    string filename;    // File to save data to and load data from

    // === Character ===
    // Physique
    string name;
    Gender gender;
    string race;
    string _class;
    DMGType dmgType;
    int age;
    double height;
    double weight;
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
    vector<Item> inventory;
    int gold;
};

#endif // !CHARACTERINFO_H