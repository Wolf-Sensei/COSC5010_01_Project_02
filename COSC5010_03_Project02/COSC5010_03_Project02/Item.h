/*
@File: Item.h
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: header file for declaring the Item class object
*/

#ifndef ITEM_H
#define ITEM_H

#include <string>

using std::string;

class Item {
    // === Functions ===
public:
    // Constructors
    Item();
    Item(string name, int value);

    // Getters & Seters
    bool setName(string name);
    bool setValue(int value);
    string getName();
    int getValue();

    // Item String
    string toString();

    // === Values ===
private:
    // Const
    static unsigned int ID;

    // Item
    string name;
    int value;
};

#endif // !ITEM_H
