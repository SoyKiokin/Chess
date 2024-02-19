
#pragma once

#include "Global.hpp"
#include <unordered_map>

class MouseManager
{
private:
	//Store when the mouse is pressed or not pressed
	std::unordered_map<sf::Mouse::Button, bool> pressedPreviously;
	std::unordered_map<sf::Mouse::Button, bool> notPressedPreviously;
	
public:
	//Constructor/Destructor
	MouseManager();
	~MouseManager();

	//Check press button functions
	const bool& Pressed(const sf::Mouse::Button button) const;
	const bool& JustPressed(const sf::Mouse::Button button);
	const bool& JustReleased(const sf::Mouse::Button button);
};

