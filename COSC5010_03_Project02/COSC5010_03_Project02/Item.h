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

class Item {
    // === Functions ===
public:
    // Constructors
    Item(int value);

    // Getters & Seters
    bool setValue(int value);
    int getValue() { return value; }

    // === Values ===
private:
    int value;
};

#endif // !ITEM_H
