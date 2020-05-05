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

#include <sstream>
#include "Item.h"

using std::stringstream;

// === Values ===

unsigned int Item::ID = 0;

// === Functions ===
// Constructors
// Default constructor, gives the item an id, and a value of 1
Item::Item() {
    Item(std::to_string(ID++), 1);
}
// Init constructor, uses the given name and value.
Item::Item(string name, int value) {
    setName(name);
    setValue(value);
}

// Getters & Seters
bool Item::setName(string name) {
    this->name = name;
    return true;
}
// Sets the value of the item. Only accepts non-negative values.
bool Item::setValue(int value) {
    if (value < 0) return value;
    this->value = value;
    return true;
}
string Item::getName() {
    return name;
}
int Item::getValue() {
    return value;
}

// Returns a string representation of the item in a readable format
string Item::toString() {
    stringstream ss;
    ss << "Item name: " << name << " | Value: " << value;
    return ss.str();
}
