#pragma once
#include <iostream>

class Room {
public:
	int highPriorityItem;
	int cntItems;
	std::string name;
	Room() {
		highPriorityItem = 0;
		cntItems = 0;
		name = "room";
	}
	
	Room(const Room& copy) {
		this->highPriorityItem = copy.highPriorityItem;
		this->cntItems = copy.cntItems;
		this->name = copy.name;
	}
	

	virtual void countPriorityItem(unsigned& ptr) {};
	virtual void countItems(unsigned& ptr) {};
	virtual std::string Name() { return this->name; };

};

class Kitchenroom : public Room {
public:
	std::string name;

	Kitchenroom() {
		highPriorityItem = 0;
		cntItems = 0;
		name = "Kitchenroom";
	}
	
	Kitchenroom(const Kitchenroom& copy) {
		this->highPriorityItem = copy.highPriorityItem;
		this->cntItems = copy.cntItems;
		this->name = copy.name;
	}
	
	void countPriorityItem(unsigned& ptr) override {
		if (ptr == 82) this->highPriorityItem++;
	}

	void countItems(unsigned& ptr) override {
		if (ptr == 79 || ptr == 78 || ptr == 67 || ptr == 80) this->cntItems++;
	}

	std::string Name() override { return this->name; }

};

class Livingroom: public Room {
public:
	std::string name;

	Livingroom() {
		highPriorityItem = 0;
		cntItems = 0;
		name = "Livingroom";
	}
	
	Livingroom(const Livingroom& copy) {
		this->highPriorityItem = copy.highPriorityItem;
		this->cntItems = copy.cntItems;
		this->name = copy.name;
	}
	

	void countPriorityItem(unsigned& ptr) override {
		if (ptr == 63) this->highPriorityItem++;
	}

	void countItems(unsigned& ptr) override {
		if (ptr == 69 || ptr == 62 || ptr == 72 || ptr == 64) this->cntItems++;
	}

	std::string Name() override  { return this->name; }

};

class Bathroom: public Room {
public:
	std::string name;

	Bathroom() {
		highPriorityItem = 0;
		cntItems = 0;
		name = "Bathroom";
	}

	
	Bathroom(const Bathroom& copy) {
		this->highPriorityItem = copy.highPriorityItem;
		this->cntItems = copy.cntItems;
		this->name = copy.name;
	}
	

	void countPriorityItem(unsigned& ptr) override {
		if (ptr == 70) this->highPriorityItem++;
	}

	void countItems(unsigned& ptr) override {
		if (ptr == 81 || ptr == 90) this->cntItems++;
	}

	std::string Name() override { return this->name; }

};