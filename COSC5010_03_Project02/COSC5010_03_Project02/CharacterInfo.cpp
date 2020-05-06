/*
@File: CharacterInfo.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Implimentation for the CharacterInfo class object
*/

#include <cmath>   // pow
#include <cstdlib> // srand, rand
#include <ctime>   // time
#include <string>
#include <sstream>
#include "CharacterInfo.h"

using std::pow;
using std::srand;
using std::rand;
using std::time;
using std::stringstream;

// =========================
// === Constructors ===
// =========================

CharacterInfo::CharacterInfo(string filename, bool load) {
    // Init
    this->filename = filename;

    // Checking if loading info
    if (load) {
        // Attempting to load info if any
        // If not creates a new character with default values
        if (!loadInfo()) { newCharacter(); }
    }
    else {
        // Creating new character
        newCharacter();
    }
}

// =========================
// === Save & Load ===
// =========================
bool CharacterInfo::saveInfo() { return false; }
bool CharacterInfo::loadInfo() { return false; }

// === Character Events ===
// Creates a new character and sets default values
void CharacterInfo::newCharacter() {
    newCharacter("Mioriya Wolf", Gender::FEMALE, "Elf", "Mage", DMGType::MAGIC,
        112,    // (~22 Human years)
        165,    // cm (~5.4ft)
        45,     // kg (100 lb)
        "Light Blue",
        8,  // 80 max HP
        20, // 200 max MP
        10, // 100 max STM
        4,  // 40 DMG
        1,  // 10% crit chance (.1 per pt)
        10, // 200% crit multipler (.1 per point)
        1, 10);
}
void CharacterInfo::newCharacter(string name, Gender gender, string race, string _class, DMGType dmgType,
    int age, double height, double weight, string hairColor,
    int attHP, int attMP, int attSTM, double attDMG, double glyCC, double glyCM,
    int level, int gold) {
    // Physique
    setName(name);
    setGender(gender);
    setRace(race);
    setClass(_class);
    setDmgType(dmgType);
    setAge(age);
    setHeight(height);
    setWeight(weight);
    setHairColor(hairColor);
    // Stats
    setAttribute(0, attHP); 
    setAttribute(1, attMP); 
    setAttribute(2, attSTM); 
    setAttribute(3, attDMG);
    updateStats();
    setHP(attHP * 10);
    setMP(attMP * 10);
    setSTM(attSTM * 10);
    // Growth
    setLevel(level);
    setXP(0);
    setAttPts(0);
    // States
    setIsDead(false);
    // Inventory
    clearInventory();
    setGold(10);
}

// Levels up the character
// level+1, attPts+5, glyphPts+1, xp->0, maxXP^
// Returns current level
int CharacterInfo::levelUp() {
    // Reset XP
    modXP(-maxXP);
    setXP(pow(2, level) * 100, true);    // starting at 100, double needed xp based on level

    // Gain pts
    modLevel(1);
    modAttPts(5);

    return level;
}

// =========================
// === Character Actions ===
// =========================

// Reduce stats and gain rewards based on current level
// Returns true if the fight was successful, and false otherwise
bool CharacterInfo::fight(CharacterInfo enemy, DMGType type) {
    // Init
    srand(time(NULL));
    int dmg;
    int drop = rand() % 100 + 1; // 1-100

    // Checking if enemy or character is dead
    if (enemy.isDead() || isDead()) return false;

    // === Attacking enemy ===
    // Calculating dmg to enemy
    // Comparing damage types
    if (dmgType == type) dmg = DMG;
    else dmg = DMG / 2;
    // Checking stats - cost 10 of respective type
    if (type == DMGType::PHYSICAL) {
        if (STM < 10) dmg /= 4;
        modSTM(-10);
    }
    else {  // Magic
        if (MP < 10) dmg /= 4;
        modMP(-10);
    }
    // Attacking
    enemy.modHP(-dmg);

    // Checking if enemy is dead
    if (enemy.isDead()) {
        // Looting enemy
        modGold(enemy.getGold());
        // Chance of item 10%
        if (drop <= 10) {
            drop = rand() % 10 + 1;  // 1-10 value
            addItem(drop);
        }

        // XP
        modXP(pow(2, enemy.getLevel() - 1) * 10);   // 2^level * 10 (10 enemies @ current level)
        return true;
    }

    // === Getting attacked ===
    // Calclating dmg
    // Checking stats (enemy will use it's dmg type, not being optimal)
    dmg = enemy.getDMG();
    if (enemy.getDMGType() == DMGType::PHYSICAL) {
        if (enemy.getSTM() < 10) dmg /= 4;
        enemy.modSTM(-10);
    }
    else {  // Magic
        if (enemy.getMP() < 10) dmg /= 4;
        enemy.modMP(-10);
    }
    // Attacking
    modHP(-dmg);
    return true;
}

// The character rests at an Inn.
// The inn costs gold to rest at.
// Character HP, MP, and STM are fully restored
// Returns true if resting was successful and false otherwise
bool CharacterInfo::rest(int gCost) {
    // Checking if character has enough gold or is dead
    if (gCost > gold || isDead()) return false;

    // Resting character
    modGold(-gCost);        // Removing gold
    setHP(maxHP);           // Restoring HP
    setMP(maxMP);           // Restoring MP
    setSTM(maxSTM);         // Restoring STM
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

// =========================
// === Character Utility ===
// =========================

// Buys an attribute pt for the character
// Returns true if the purchase was successful and false otherwise
bool CharacterInfo::buyAttPts(int gCost) {
    // Checking if character has enough gold
    if (gCost > gold) return false;

    // Purhcasing point
    modGold(-gCost);    // Removing gold
    modAttPts(1);       // Adding attribute point
    return true;
}

// Increase a character attribute and increase
// max HP, max MP, Max STM, or DMG
// Returns true if point allocation was successful and false otherwise
bool CharacterInfo::spendAttPt(int att) {
    // Checking if character has attribute points to spend or is dead
    if (attributePts == 0) return false;

    // Adding attribute point
    if (!modAttribute(att, 1)) return false;
    updateStats();
    modAttPts(-1);

    // Updating stats
    return true;
}

// =========================
// === Utility ===
// =========================

// Updates stats
void CharacterInfo::updateStats() {
    setHP(attributes[0] * 10, true);
    setMP(attributes[1] * 10, true);
    setSTM(attributes[2] * 10, true);
    setDMG(attributes[3] * 10);
}

// Character info string
string CharacterInfo::toString() {
    // Init
    stringstream ss;

    // Creating string
    ss << "=== Character Info ===\n"
        << "Name: " << name << '\n'
        << ((gender == Gender::MALE) ? "Male" : "Female") << " " << race << " " << _class << '\n'
        << "Age: " << age << " years | Height: " << height << "cm | Weight: " << weight << "kg | Hair Color: " << hairColor << '\n'
        << "=== Stats ===\n"
        << "Status: " << ((dead) ? "Dead" : "Alive") << '\n'
        << "Level: " << level << " | XP: " << XP << "/" << maxXP << '\n'
        << "HP: " << HP << "/" << maxHP << " | MP: " << MP << "/" << maxMP << " | STM: " << STM << "/" << maxSTM << " | Damage: " << DMG << '\n'
        << "Attributes HP: " << attributes[0] << " | MP: " << attributes[1] << " | STM: " << attributes[2] << " | DMG: " << attributes[3] << '\n'
        << "Gold: " << gold << " | Items: " << inventory.size() << " | Attribute Points: " << attributePts;

    return ss.str();
}

// =========================
// === Getters & Setters & Modifs ===
// =========================

// Setters
// Physique
void CharacterInfo::setName(string name) { this->name = name; }
void CharacterInfo::setGender(Gender gender) { this->gender = gender; }
void CharacterInfo::setRace(string race) { this->race = race; }
void CharacterInfo::setClass(string _class) { this->_class = _class; }
void CharacterInfo::setDmgType(DMGType type) { dmgType = type; }
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
    if (max) maxHP = hp;
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
bool CharacterInfo::setDMG(int dmg) {
    // Checking if dmg is negative
    if (dmg < 0) return false;

    // Setting dmg
    DMG = dmg;
    return true;
}
bool CharacterInfo::setAttribute(int att, int amount) {
    // Checking if attribute is out of range, and if amount is negative
    if (att < 0 || att > 3 || amount < 0) return false;

    // Setting attribute
    attributes[att] = amount;
    return true;
}
// Growth
bool CharacterInfo::setLevel(int level) {
    // Checking if level is less than 1
    if (level < 1) return false;

    // Setting level
    this->level = level;
    setXP(pow(2, level) * 100, true);
    return true;
}
bool CharacterInfo::setXP(int xp, bool max) {
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
// States
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
int CharacterInfo::modDMG(int dmg) {
    // Modifying dmg
    DMG += dmg;

    // Checking if dmg is negative
    if (DMG < 0) DMG = 0;
    return DMG;
}
int CharacterInfo::modAttribute(int att, int amount) {
    // Checking if attribute is out of range
    if (att < 0 || att > 3) return false;

    // Modifying attribute
    attributes[att] += amount;

    // Checking if attribute is negative
    if (attributes[att] < 0) attributes[att] = 0;
    return attributes[att];
}
// Growth
int CharacterInfo::modLevel(int level) {
    // Modifying level
    this->level += level;

    // Checking if level is less than 1
    if (this->level < 1) level = 1;
    return this->level;
}
int CharacterInfo::modXP(int xp, bool max) {
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
    if (XP >= maxXP) levelUp();
    return XP;
}
int CharacterInfo::modAttPts(int pts) {
    // Modifying attribute pts, ensures that it isn't negative
    attributePts += pts;
    if (attributePts < 0) attributePts = 0;
    return attributePts;
}
// Inventory
void CharacterInfo::addItem(int value) {
    Item item(value);
    inventory.push_back(item);
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
// Growth
int CharacterInfo::getXP(bool max) {
    if (max) return maxXP;
    else return XP;
}
