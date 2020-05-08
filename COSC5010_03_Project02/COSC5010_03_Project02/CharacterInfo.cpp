/*
@File: CharacterInfo.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Implimentation for the CharacterInfo class object
Impliments setters and modifs for character stats
Modifs modify the value by a set amount.
Is able to initalize a new character
Impliments varius character actions such as fighting and resting.
Impliments save and load functionallity
Impliments signature and encryption/decrytipon for character data
*/

#include <cmath>        // pow, rand, srand
#include <ctime>        // time
#include <exception>    // exception, hash
#include <fstream>      // endl, ifstream, ofstream
#include <sstream>      // getline, stringstream
#include "CharacterInfo.h"

using std::pow;
using std::rand;
using std::srand;
using std::time;
using std::endl;
using std::getline;
using std::exception;
using std::hash;
using std::ifstream;
using std::ofstream;
using std::stringstream;

// =========================
// === Constructors ===
// =========================

// Constructor takes in a filename for where character information is saved and loaded from
// Also initalizes the character
CharacterInfo::CharacterInfo(string filename) {
    // Init
    this->filename = filename;
    newCharacter();
}

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
bool CharacterInfo::saveInfo() {
    // Init
    ofstream fileOut;   // Output stream used to output character information and signature to file
    string info;        // Info the signature and encryption will take place on

    // Opening file
    fileOut.open(filename);
    if (fileOut.is_open()) {
        // Encrypting character information and producing signature
        info = getInfo();
        fileOut << getSig(info) << endl;
        fileOut << XOR(info);

        // Closing file
        fileOut.close();
    }
    // File was unable to open
    else return false;

    // Save was successful
    return true;
}

// Load character information from a file
// Reads in a signature line and encrypted data
// After decrypting the data, a signature is generated from it
// If the read and calculated signatures match then the data wasn't tampered
// If the data was tampered with then returns false
// Otherwise it sets the character data to what was loaded
// Returns true if the information was loaded successfully and false otherwise
bool CharacterInfo::loadInfo() {
    // Init
    // File
    ifstream fileIn;    // Input stream used to input character information and signature from file
    string line;        // String taken from file

    // Info
    stringstream ss;    // Used to combine all the character information
    string info;        // Unencrypted character information
    string ssig;        // Signature line
    int sig;            // Signature line converted to an int    
    
    // Opening file
    fileIn.open(filename);
    if (fileIn.is_open()) {
        // Getting signature
        getline(fileIn, ssig);

        // Getting encrypted character information
        // If character information has newlines, adds them back in
        bool firstLine = true;
        while (getline(fileIn, line)) {
            if (firstLine) {
                ss << line;             // Getting character info
                firstLine = false;      // First line is done
            }
            else ss << endl << line;    // Charcter info had a newline, adding it back in
        }

        // Closing file
        fileIn.close();
    }
    // File couldn't be opened
    else return false;
    
    // Decrypting character information
    info = XOR(ss.str());
    ss.str("");
    ss << info;

    // Checking if character information was tampered
    // Generates the hash signature of the character information
    // If the signature in the file and the compute signature don't match then the info was tampered
    try {
        sig = stoi(ssig);   // Calculating signature from character info
    }
    catch (exception& e) {
        return false;       // Signature was tampered
    }

    // Checking if Signatures match
    // If not the data was tampered with
    if (sig != getSig(info)) return false;

    // Loading info into character
    // Physique
    getline(ss, name);
    getline(ss, line);
    if (line.compare("MALE") == 0) gender = Gender::MALE;
    else gender = Gender::FEMALE;
    getline(ss, race);
    getline(ss, _class);
    getline(ss, line);
    if (line.compare("PHYSICAL") == 0) dmgType = DMGType::PHYSICAL;
    else dmgType = DMGType::MAGIC;
    getline(ss, line);
    age = stoi(line);
    getline(ss, line);
    height = stod(line);
    getline(ss, line);
    weight = stod(line);
    getline(ss, hairColor);
    // Stats
    getline(ss, line);
    maxHP = stoi(line);
    getline(ss, line);
    HP = stoi(line);
    getline(ss, line);
    maxMP = stoi(line);
    getline(ss, line);
    MP = stoi(line);
    getline(ss, line);
    maxSTM = stoi(line);
    getline(ss, line);
    STM = stoi(line);
    getline(ss, line);
    DMG = stoi(line);
    getline(ss, line);
    attributes[0] = stoi(line);
    getline(ss, line);
    attributes[1] = stoi(line);
    getline(ss, line);
    attributes[2] = stoi(line);
    getline(ss, line);
    attributes[3] = stoi(line);
    // Growth
    getline(ss, line);
    level = stoi(line);
    getline(ss, line);
    maxXP = stoi(line);
    getline(ss, line);
    XP = stoi(line);
    getline(ss, line);
    attributePts = stoi(line);
    // States
    getline(ss, line);
    dead = stoi(line);
    // Inventory
    getline(ss, line);
    sig = stoi(line);   // Number of items
    for (int i = 0; i < sig; i++) {
        getline(ss, line);
        addItem(stoi(line));
    }
    getline(ss, line);
    gold = stoi(line);

    return true;
}

// Returns the character information as a string seperated by newlines
string CharacterInfo::getInfo() {
    stringstream ss;
    ss << name << endl
        << ((gender == Gender::MALE) ? "MALE" : "FEMALE") << endl
        << race << endl << _class << endl
        << ((dmgType == DMGType::PHYSICAL) ? "PHYSICAL" : "MAGIC") << endl
        << age << endl << height << endl << weight << endl << hairColor << endl
        << maxHP << endl << HP << endl << maxMP << endl << MP << endl << maxSTM << endl << STM << endl << DMG << endl
        << attributes[0] << endl << attributes[1] << endl << attributes[2] << endl << attributes[3] << endl
        << level << endl << maxXP << endl << XP << endl << attributePts << endl << dead << endl
        << inventory.size() << endl;
    for (Item item : inventory) ss << item.getValue() << endl;
    ss << gold;

    return ss.str();
}

// Returns a hashed value of the given string (signature)
// Is reproducable between runs
int CharacterInfo::getSig(string str) {
    // Init
    hash<string> sighash;   // Hashing function

    // Returning signature
    return sighash(str);
}

// Returns a encrypted or decrypted string of the given info
string CharacterInfo::XOR(string str) {
    // Init
    char key[3] = { 'K', 'C', 'Q' };    // Key to encrypt/decrypt with
    string output = str;                // Resulting encrypted/decrypted data

    // Encrypting/decrypting
    for (int i = 0; i < str.size(); i++)
        output[i] = str[i] ^ key[i % (sizeof(key) / sizeof(char))];

    return output;
}

// =========================
// === Character Events ===
// =========================

// Creates a new character with default values
void CharacterInfo::newCharacter() {
    newCharacter("Mioriya Wolf",
        Gender::FEMALE,
        "Elf",  // Race
        "Mage", // Class
        DMGType::MAGIC,
        112,    // (~22 Human years)
        165,    // cm (~5.4ft)
        45,     // kg (100 lb)
        "Light Blue",
        8,      // 80 max HP
        20,     // 200 max MP
        10,     // 100 max STM
        4,      // 40 DMG
        1,      // Level
        10);    // Gold
}

// Creates a new character with the given values
void CharacterInfo::newCharacter(string name, Gender gender, string race, string _class, DMGType dmgType,
    int age, double height, double weight, string hairColor,
    int attHP, int attMP, int attSTM, int attDMG, int level, int gold) {
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
    setHP(maxHP);
    setMP(maxMP);
    setSTM(maxSTM);
    // Growth
    XP = 0;
    setLevel(level);
    setXP(0);
    setAttPts(0);
    // States
    setIsDead(false);
    // Inventory
    clearInventory();
    setGold(gold);
}

// Levels up the character
// level+1, attPts+5, glyphPts+1, xp->0, maxXP^
// Restores HP, MP, STM
// Returns current level
int CharacterInfo::levelUp() {
    // Growth
    modXP(-maxXP);
    modLevel(1);
    modAttPts(5);

    // Restore stats
    setHP(maxHP);
    setMP(maxMP);
    setSTM(maxSTM);

    return level;
}

// =========================
// === Character Actions ===
// =========================

// Reduce stats and gain rewards based on current level
// First does damage to enemy, if enemy dies then loots
// Otherwise recieve damage
// Returns true if the fight was successful, and false otherwise
bool CharacterInfo::fight(CharacterInfo& enemy, DMGType type) {
    // Init
    srand(time(NULL));
    int dmg;                    // Damage done
    int cost = 20;              // Cost of doing damage

    // Checking if enemy or character is dead
    if (enemy.isDead() || isDead()) return false;

    // === Attacking enemy ===
    // Calculating dmg to enemy
    // Comparing damage types
    if (dmgType == type) dmg = DMG;
    else dmg = DMG / 2;
    // Checking stats
    if (type == DMGType::PHYSICAL) {
        if (STM < cost) dmg /= 4;
        modSTM(-cost);
    }
    else {  // Magic
        if (MP < cost) dmg /= 4;
        modMP(-cost);
    }
    // Attacking enemy
    enemy.modHP(-dmg);

    // Checking if enemy is dead
    if (enemy.isDead()) {
        // Looting enemy
        modGold(enemy.getGold());
        for (Item item : enemy.getInventory()) {
            addItem(item.getValue());
        }

        // XP
        modXP(pow(2, enemy.getLevel() - 1) * 10);   // 2^level * 10 (10 enemies @ current level)
        return true;    // Fight is over
    }

    // === Getting attacked ===
    // Calclating dmg
    // Checking stats (enemy will use it's dmg type)
    dmg = enemy.getDMG();
    if (enemy.getDMGType() == DMGType::PHYSICAL) {
        if (enemy.getSTM() < cost) dmg /= 4;
        enemy.modSTM(-cost);
    }
    else {  // Magic
        if (enemy.getMP() < cost) dmg /= 4;
        enemy.modMP(-cost);
    }
    // Attacking character
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
    // Checking if character has attribute points to spend
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

// Updates stats for max HP, MP, STM, and DMG
void CharacterInfo::updateStats() {
    setHP(attributes[0] * 10, true);
    setMP(attributes[1] * 10, true);
    setSTM(attributes[2] * 10, true);
    setDMG(attributes[3] * 10);
}

// Converts character info into an easily readable string
string CharacterInfo::toString() {
    // Init
    stringstream ss;

    // Creating string
    ss << "=== Character Info ===\n"
        << "Name: " << name << '\n'
        << ((gender == Gender::MALE) ? "Male" : "Female") << " " << race << " " << _class << '\n'
        << "Damage type: " << ((dmgType == DMGType::PHYSICAL) ? "Physical" : "Magic") << '\n'
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
// - Ensures values aren't negative
// - Returns true if value was valid, and false otherwise
// Physique
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
// Stats
// - Set HP/MP/STM/DMG ignores attributes;
//   if attributes are updated HP/MP/STM/DMG will reset to proper values
// - Set HP can cause the character to die
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
// - Attributes are for HP(0), MP(1), STM(2), and DMG(3)
bool CharacterInfo::setAttribute(int att, int amount) {
    // Checking if attribute is out of range, and if amount is negative
    if (att < 0 || att > 3 || amount < 0) return false;

    // Setting attribute
    attributes[att] = amount;
    updateStats();
    return true;
}
// Growth
// - Minimum level is 1
// - Sets maximum XP
bool CharacterInfo::setLevel(int level) {
    // Checking if level is less than 1
    if (level < 1) return false;

    // Setting level
    this->level = level;
    setXP(pow(2, level-1) * 100, true);
    return true;
}
// - If current XP is over maxXP, a levelup occurs
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
// Invetory
bool CharacterInfo::setGold(int gold) {
    // Checking if gold is negative
    if (gold < 0) return false;

    // Setting gold
    this->gold = gold;
    return true;
}

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
// - Attributes are for HP(0), MP(1), STM(2), and DMG(3)
int CharacterInfo::modAttribute(int att, int amount) {
    // Checking if attribute is out of range
    if (att < 0 || att > 3) return false;

    // Modifying attribute
    attributes[att] += amount;

    // Checking if attribute is negative
    if (attributes[att] < 0) attributes[att] = 0;
    updateStats();
    return attributes[att];
}
// Growth
// - Does not chance max or current XP
// - Sets maximum XP
int CharacterInfo::modLevel(int level) {
    // Modifying level
    this->level += level;

    // Checking if level is less than 1
    if (this->level < 1) this->level = 1;
    setXP(pow(2, this->level - 1) * 100, true);
    return this->level;
}
// - If current XP is over maxXP, a levelup occurs
int CharacterInfo::modXP(int xp, bool max) {
    // Modify max xp, ensures that it isn't negative
    if (max) {
        maxXP += xp;
        if (maxXP < 0) maxXP = 0;
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
// - Adds an item to the character inventory with the given value
void CharacterInfo::addItem(int value) {
    Item item(value);
    inventory.push_back(item);
}
// - Returns number of items that were cleared
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
