#pragma once
#include "room.hpp"
#include <iostream>

Room::Room() {
	highPriorityItem = 0;
	cntItems = 0;
	name = "room";
}
Room::Room(const Room& copy) {
	this->highPriorityItem = copy.highPriorityItem;
	this->cntItems = copy.cntItems;
	this->name = copy.name;
}

void Room::countPriorityItem(unsigned& ptr) {};
void Room::countItems(unsigned& ptr) {};
std::string Room::Name() { return this->name; };


Kitchenroom::Kitchenroom() {
	highPriorityItem = 0;
	cntItems = 0;
	name = "Kitchenroom";
}
Kitchenroom::Kitchenroom(const Kitchenroom& copy) {
	this->highPriorityItem = copy.highPriorityItem;
	this->cntItems = copy.cntItems;
	this->name = copy.name;
}

void Kitchenroom::countPriorityItem(unsigned& ptr) {
	if (ptr == 79 || ptr == 82) this->highPriorityItem++;
}
void Kitchenroom::countItems(unsigned& ptr) {
	if (ptr == 79 || ptr == 78 || ptr == 67 || ptr == 80) this->cntItems++;
}

std::string Kitchenroom::Name() { return this->name; }


Livingroom::Livingroom() {
	highPriorityItem = 0;
	cntItems = 0;
	name = "Livingroom";
}
Livingroom::Livingroom(const Livingroom& copy) {
	this->highPriorityItem = copy.highPriorityItem;
	this->cntItems = copy.cntItems;
	this->name = copy.name;
}

void Livingroom::countPriorityItem(unsigned& ptr) {
	if (ptr == 63) this->highPriorityItem++;
}
void Livingroom::countItems(unsigned& ptr) {
	if (ptr == 69 || ptr == 62 || ptr == 72 || ptr == 64) this->cntItems++;
}

std::string Livingroom::Name() { return this->name; }



Bathroom::Bathroom() {
	highPriorityItem = 0;
	cntItems = 0;
	name = "Bathroom";
}
Bathroom::Bathroom(const Bathroom& copy) {
	this->highPriorityItem = copy.highPriorityItem;
	this->cntItems = copy.cntItems;
	this->name = copy.name;
}

void Bathroom::countPriorityItem(unsigned& ptr) {
	if (ptr == 70) this->highPriorityItem++;
}
void Bathroom::countItems(unsigned& ptr) {
	if (ptr == 81 || ptr == 90) this->cntItems++;
}

std::string Bathroom::Name() { return this->name; }