/*
@File: CharacterInfo.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Implimentation for the CharacterInfo class object
*/

#include "CharacterInfo.h"

CharacterInfo::CharacterInfo() {
    // Attempting to load info if any
    // If not creates a new character with default values
    if (!loadInfo) { newCharacter(); }
}

// Creates a new character and sets default values
bool CharacterInfo::newCharacter() {
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