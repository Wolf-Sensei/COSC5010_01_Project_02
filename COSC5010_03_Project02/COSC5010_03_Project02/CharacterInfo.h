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

enum struct Gender {male, female};

class CharacterInfo {
    // === Functions === 
public:
    // Constructors
    CharacterInfo(); // Default Constructor

    // Save & Load Info
    bool saveInfo();
    bool loadInfo();

    // Character
    bool newCharacter();    // Sets default character stats

    // Character Info string
    string toString();

    // Getters & Setters & modifs
    // Physique
    bool setName(string name);
    bool setGender(Gender gender);
    bool setRace(string race);
    bool setClass(string _class);
    bool setAge(int age);
    bool setHeight(double height);
    bool setWeight(double weight);
    bool setHairColor(string color);
    // Stats
    bool setHP(int hp, bool max = false);
    bool setMP(int mp, bool max = false);
    bool setSTM(int stm, bool max = false);
    bool setDMG(double dmg);
    bool setCritC(float critC);
    bool setCritM(float critM);
    bool setGlyph(int glyph, double multiplier);
    // Growth
    bool setLevel(int level);
    bool setXP(double xp, bool max = false);
    bool setAttPts(int pts);
    bool setGlyphPts(int pts);
    // States
    bool setIsReseting(bool resting);
    bool setIsDead(bool dead);
    // Inventory
    bool setGold(int gold);

    // Modify stat by +- amount
    // Stats
    int modHP(int hp, bool max = false);
    int modMP(int mp, bool max = false);
    int modSTM(int stm, bool max = false);
    double modDMG(int dmg);
    float modCritC(float critC);
    float modCritM(float cirtM);
    double modGlyph(int glyph, double multiplier);
    // Growth
    int modLevel(int level);
    double modXP(double xp, bool max = false);
    int modAttPts(int pts);
    int modGlyphPts(int pts);
    // States
    bool toggleIsResting();
    bool toggleIsDeath();
    // Inventory
    bool addItem(Item* item);
    Item addItem(string name, int value);
    bool clearInventory();
    int modGold(int gold);

    // Physique
    string getName();
    Gender getGender();
    string getRace();
    string getClass();
    int getAge();
    double getHeight();
    double getWeight();
    string getHairColor();
    // Stats
    int getHP(bool max = false);
    int getMP(bool max = false);
    int getSTM(bool max = false);
    double getDMG();
    float getCritC();
    float getCritM();
    double* getGlyphs();
    // Growth
    int getLevel();
    int getXP(bool max = false);
    int getAttPts();
    int getGlyphPts();
    // States
    bool isResting();
    bool isDead();
    // Inventory
    vector<Item> getInventory();
    int getItemCount();
    int getGold();

    // Actions
    bool fight();                       // Get:items, gold, xp ; Cost: HP, Mana, STM ; Can die
    bool rest(int gCost, int xpCost);   // Restore stats, lose gold and xp, gain resting bonus
    int sellItems();    // Sell items for gold
    int buyattPts();    // Upgrade hp,mp,stm,dmg
    int buyGlyphPts();  // Upgrade glyphs

private:

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
    int age;
    double height;
    double weight;
    string hairColor;

    // Stats
    int maxHP, HP;      // Health
    int maxMP, MP;      // Mana
    int maxSTM, STM;    // Stamina
    double DMG;         // Damage
    float critC;        // Crit Chance
    float critM;        // Dmg multipler if character crits
    double glyphs[6];   // Multipliers for hp, mp, stm, dmg, critC, critM

    // Growth
    int level;          // Level
    double maxXP, XP;   // Experience needed to gain level
    int attributePts;   // Increase max hp, mp, stm, dmg
    int glyphPts;       // Modify glyph multipliers

    // States
    bool resting;       // Whether the character is rested and gains a bonus
    bool dead;          // Whether the character is dead or not

    // Inventory
    vector<Item> inventory;
    int gold;
};

#endif // !CHARACTERINFO_H