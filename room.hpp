#pragma once

#include <iostream>


class Room {
public:
	int highPriorityItem;
	int cntItems;
	std::string name;
	Room();

	Room(const Room& copy);

	virtual void countPriorityItem(unsigned& ptr);
	virtual void countItems(unsigned& ptr);
	virtual std::string Name();

};

class Kitchenroom : public Room {
public:
	std::string name;

	Kitchenroom();

	Kitchenroom(const Kitchenroom& copy);

	void countPriorityItem(unsigned& ptr) override;

	void countItems(unsigned& ptr) override;

	std::string Name() override;

};

class Livingroom : public Room {
public:
	std::string name;

	Livingroom();

	Livingroom(const Livingroom& copy);


	void countPriorityItem(unsigned& ptr) override;

	void countItems(unsigned& ptr) override;

	std::string Name() override;

};

class Bathroom : public Room {
public:
	std::string name;

	Bathroom();


	Bathroom(const Bathroom& copy);


	void countPriorityItem(unsigned& ptr) override;

	void countItems(unsigned& ptr) override;

	std::string Name() override;
};