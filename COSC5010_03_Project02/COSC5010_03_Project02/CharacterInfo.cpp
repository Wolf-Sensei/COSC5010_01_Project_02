/*
@File: CharacterInfo.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Implimentation for the CharacterInfo class object
*/

#include <math.h>   // pow
#include "CharacterInfo.h"

// === Constructors ===
CharacterInfo::CharacterInfo() {
    // Attempting to load info if any
    // If not creates a new character with default values
    if (!loadInfo()) { newCharacter(); }
}

// === Save & Load ===
bool CharacterInfo::saveInfo() { return false; }
bool CharacterInfo::loadInfo() { return false; }

// === Character ===
// Creates a new character and sets default values
void CharacterInfo::newCharacter() {
    // Physique
    setName("Mioriya Wolf");
    setGender(Gender::female);
    setRace("Elf");
    setClass("Mage");
    setAge(112);    // (~22 Human years)
    setHeight(180); // cm (~6ft)
    setWeight(45);  // kg (100 lb)
    setHairColor("Light Blue");
    // Stats
    setHP(85, true); setHP(85);
    setMP(215, true); setMP(215);
    setSTM(100, true); setSTM(100);
    setDMG(130);
    setCritC(0.1f); // 10%
    setCritM(2);    // 200% (x2 dmg)
    for (int i = 0; i < 6; i++) setGlyph(i, 1); // 100% (no change)
    // Growth
    setLevel(1);
    setXP(100, true); setXP(0); // Need 100 xp to level
    setAttPts(0);
    setGlyphPts(0);
    // States
    setIsReseting(false);
    setIsDead(false);
    // Inventory
    clearInventory();
    setGold(10);
}

// Levels up the character
// level+1, attPts+5, glyphPts+1, xp->0, maxXP^
// Returns current level
int CharacterInfo::levelUp() {
    // Gain pts
    modLevel(1);
    modAttPts(5);
    modGlyphPts(1);

    // Reset XP
    setXP(0);
    setXP(pow(2,level-1)*100, true);    // starting at 100, double needed xp based on level

    return level;
}

// Character info string
string CharacterInfo::toString() {
    return "";
}

// === Getters & Setters & Modifs ===
// Setters
// Physique
void CharacterInfo::setName(string name) { this->name = name; }
void CharacterInfo::setGender(Gender gender) { this->gender = gender; }
void CharacterInfo::setRace(string race) { this->race = race; }
void CharacterInfo::setClass(string _class) { this->_class = _class; }
bool CharacterInfo::setAge(int age) {
    // Checking if age is negative
    if (age < 0) return false;

    // Setting age
    this->age = age;
    return true;
}
bool CharacterInfo::setHeight(double height) {
    // Checking if height is negative
    if (height < 0) return false;

    // Setting height
    this->height = height;
    return true;
}
bool CharacterInfo::setWeight(double weight) {
    // Checking if weight is negative
    if (weight < 0) return false;

    // Setting weight
    this->weight = weight;
    return true;
}
void CharacterInfo::setHairColor(string color) { hairColor = color; }
// Stats
bool CharacterInfo::setHP(int hp, bool max) {
    // Checking if hp is negative
    if (hp < 0) return false;

    // Setting HP
    if (max) maxHP = HP;
    else HP = hp;

    // Checking if current HP is over max
    if (HP > maxHP) HP = maxHP;

    // Checking if character died
    if (HP == 0) setIsDead(true);
    return true;
}
bool CharacterInfo::setMP(int mp, bool max) {
    // Checking if mp is negative
    if (mp < 0) return false;

    // Setting MP
    if (max) maxMP = mp;
    else MP = mp;

    // Checking if current MP is over max
    if (MP > maxMP) MP = maxMP;
    return true;
}
bool CharacterInfo::setSTM(int stm, bool max) {
    // Checking if stm in negative
    if (stm < 0) return false;

    // Setting stm
    if (max) maxSTM = stm;
    else STM = stm;

    // Checking if current STM is over max
    if (STM > maxSTM) STM = maxSTM;
    return true;
}
bool CharacterInfo::setDMG(double dmg) {
    // Checking if dmg is negative
    if (dmg < 0) return false;

    // Setting dmg
    DMG = dmg;
    return true;
}
bool CharacterInfo::setCritC(float critC) {
    // Checking if crit chance is negative
    if (critC < 0) return false;

    // Setting crit chance
    this->critC = critC;
    return true;
}
bool CharacterInfo::setCritM(float critM) {
    // Checking if crit multiplier is negative
    if (critM < 0) return false;

    // Setting crit multipler
    this->critM = critM;
    return true;
}
bool CharacterInfo::setGlyph(int glyph, double multipler) {
    // Checking if glyph is out of rance, and if multiplier is negative
    if (glyph < 0 || glyph > 5 || multipler < 0) return false;

    // Setting glyph multipler
    glyphs[glyph] = multipler;
    return true;
}
// Growth
bool CharacterInfo::setLevel(int level) {
    // Checking if level is less than 1
    if (level < 1) return false;

    // Setting level
    this->level = level;
    return true;
}
bool CharacterInfo::setXP(double xp, bool max) {
    // Checking if xp is negative
    if (xp < 0) return false;

    // Setting xp
    if (max) maxXP = xp;
    else XP = xp;

    // Checking if XP is over/at max
    if (XP >= maxXP) levelUp();
    return true;
}
bool CharacterInfo::setAttPts(int pts) {
    // Checking if attribute pts is negative
    if (pts < 0) return false;

    // Setting attribute pts
    attributePts = pts;
    return true;
}
bool CharacterInfo::setGlyphPts(int pts) {
    // Checking if glyph pts is negative
    if (pts < 0) return false;

    // Setting attribute pts
    glyphPts = pts;
    return true;
}
// States
void CharacterInfo::setIsReseting(bool resting) { this->resting = resting; }
void CharacterInfo::setIsDead(bool dead) { this->dead = dead; }
// Invetory
bool CharacterInfo::setGold(int gold) {
    // Checking if gold is negative
    if (gold < 0) return false;

    // Setting gold
    this->gold = gold;
    return true;
}

// Modifs
// Stats
int CharacterInfo::modHP(int hp, bool max) {
    // Modify max hp, ensures that it isn't negative
    if (max) {
        maxHP += hp;
        if (maxHP < 0) maxHP = 0;

        // Checking if current hp is over max
        if (HP > maxHP) HP = maxHP;
    }
    // Modify current hp, ensures that it isn't negative
    else {
        HP += hp;
        if (HP < 0) HP = 0;
    }

    // Checking if character died
    if (HP == 0) setIsDead(true);
    return HP;
}
int CharacterInfo::modMP(int mp, bool max) {
    // Modify max mp, ensures that it isn't negative
    if (max) {
        maxMP += mp;
        if (maxMP < 0) maxMP = 0;

        // Checking if current mp is over max
        if (MP > maxMP) MP = maxMP;
    }
    // Modify current mp, ensures that it isn't negative
    else {
        MP += mp;
        if (MP < 0) MP = 0;
    }
    return MP;
}
int CharacterInfo::modSTM(int stm, bool max) {
    // Modify max stm, ensures that it isn't negative
    if (max) {
        maxSTM += stm;
        if (maxSTM < 0) maxSTM = 0;

        // Checking if current stm is over max
        if (STM > maxSTM) STM = maxSTM;
    }
    // Modify current stm, ensures that it isn't negative
    else {
        STM += stm;
        if (STM < 0) STM = 0;
    }
    return STM;
}
double CharacterInfo::modDMG(int dmg) {
    // Modifying dmg
    DMG += dmg;

    // Checking if dmg is negative
    if (DMG < 0) DMG = 0;
    return DMG;
}
float CharacterInfo::modCritC(float critC) {
    // Modifying crit chance
    this->critC += critC;

    // Checking if crit chance is negative
    if (this->critC < 0) this->critC = 0;
    return this->critC;
}
float CharacterInfo::modCritM(float critM) {
    // Modifying crit multiiplier
    this->critM += critM;

    // Checking if crit multipler is negative
    if (this->critM < 0) this->critM = 0;
    return this->critM;
}
double CharacterInfo::modGlyph(int glyph, double multipler) {
    // Checking if glyph is out of range
    if (glyph < 0 || glyph > 5) return -1;

    // Modifying glyph
    glyphs[glyph] += multipler;

    // Checking if glyph is negative
    if (glyphs[glyph] < 0) glyphs[glyph] = 0;
    return glyphs[glyph];
}
// Growth
int CharacterInfo::modLevel(int level) {
    // Modifying level
    this->level += level;

    // Checking if level is less than 1
    if (this->level < 1) level = 1;
    return this->level;
}
double CharacterInfo::modXP(double xp, bool max) {
    // Modify max xp, ensures that it isn't negative
    if (max) {
        maxXP += xp;
        if (maxXP < 0) maxXP = 0;

        // Checking if current xp is over max
        if (XP > maxXP) XP = maxXP;
    }
    // Modify current xp, ensures that it isn't negative
    else {
        XP += xp;
        if (XP < 0) XP = 0;
    }

    // Checking if character is at maxXP
    if (XP = maxXP) levelUp();
    return XP;
}
int CharacterInfo::modAttPts(int pts) {
    // Modifying attribute pts, ensures that it isn't negative
    attributePts += pts;
    if (attributePts < 0) attributePts = 0;
    return attributePts;
}
int CharacterInfo::modGlyphPts(int pts) {
    // Modfying glyph pts, ensures that it isn't negative
    glyphPts += pts;
    if (glyphPts < 0) glyphPts = 0;
    return glyphPts;
}
// States
bool CharacterInfo::toggleIsResting() {
    resting = !resting;
    return resting;
}
bool CharacterInfo::toggleIsDeath() {
    dead = !dead;
    return dead;
}
// Inventory
Item* CharacterInfo::addItem(string name, int value) {
    Item item(name, value);
    inventory.push_back(item);
    return &item;
}
int CharacterInfo::clearInventory() {
    int size = inventory.size();
    inventory.clear();
    return size;
}
int CharacterInfo::modGold(int gold) {
    // Modifying gold, ensures that it isn't negative
    this->gold += gold;
    if (this->gold < 0) this->gold = 0;
    return this->gold;
}

// Getters
// Physique
string CharacterInfo::getName() { return name; }
Gender CharacterInfo::getGender() { return gender; }
string CharacterInfo::getRace() { return race; }
string CharacterInfo::getClass() { return _class; }
int CharacterInfo::getAge() { return age; }
double CharacterInfo::getHeight() { return height; }
double  CharacterInfo::getWeight() { return weight; }
string CharacterInfo::getHairColor() { return hairColor; }
// Stats
int CharacterInfo::getHP(bool max) {
    if (max) return maxHP;
    else return HP;
}
int CharacterInfo::getMP(bool max) {
    if (max) return maxMP;
    else return MP;
}
int CharacterInfo::getSTM(bool max) {
    if (max) return maxSTM;
    else return STM;
}
double CharacterInfo::getDMG() { return DMG; }
float CharacterInfo::getCritC() { return critC; }
float CharacterInfo::getCritM() { return critM; }
double* CharacterInfo::getGlyphs() { return glyphs; }
// Growth
int CharacterInfo::getLevel() { return level; }
int CharacterInfo::getXP(bool max) {
    if (max) return maxXP;
    else return XP;
}
int CharacterInfo::getAttPts() { return attributePts; }
int CharacterInfo::getGlyphPts() { return glyphPts; }
// States
bool CharacterInfo::isResting() { return resting; }
bool CharacterInfo::isDead() { return dead; }
// Inventory
vector<Item> CharacterInfo::getInventory() { return inventory; }
int CharacterInfo::getItemCount() { return inventory.size(); }
int CharacterInfo::getGold() { return gold; }

// === Actions ===
bool CharacterInfo::fight() {
    return true;
}

bool CharacterInfo::rest(int gCost, int xpCost) {
    return true;
}

// Sells all items in the characters inventory.
// The value of all the items is put into the characters gold supply
// Returns the amount of gold gained
int CharacterInfo::sellItems() {
    // Init
    int profit = 0;

    // Selling items
    for (Item item : inventory) {
        profit += item.getValue();
    }
    inventory.clear();
    modGold(profit);

    return profit;
}

// Buys an attribute pt for the character
// Attribute pts cost more for every successive pt
// Returns the number of attribute pts bought
// If gold was insufficient, returns -1
int CharacterInfo::buyAttPts(int count) {
    return 0;
}

// Buys a glyph pt for the character
// Glyph pts cost more for every successive pt
// Returns the number of glyph pts bought
// If gold was insufficient, returns -1
int  CharacterInfo::buyGlyphPts(int count) {
    return 0;
}