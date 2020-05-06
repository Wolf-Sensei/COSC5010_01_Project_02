/*
@File: Item.cpp
@Author: Robert Randolph
@Class: COSC 5010-03
@Assignment: Project 02 | Option 1
@Due: May 12, 2020
@Description: Implimentation for the Item object class
Holds the item name and the item value.
Can Return a string of the item
*/

#include "Item.h"

// === Functions ===
// Constructors
// Default constructor, gives the item an id, and a value of 1
Item::Item(int value = 1) {
    if (value < 0) value = 1;
    setValue(value);
}

// Getters & Seters
// Sets the value of the item. Only accepts non-negative values.
bool Item::setValue(int value) {
    if (value < 0) return false;
    this->value = value;
    return true;
}
