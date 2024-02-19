
#include "MouseManager.hpp"

//Constructor
MouseManager::MouseManager()
{

}

//Destructor
MouseManager::~MouseManager()
{
}

//Check if the mouse button is pressed
const bool& MouseManager::Pressed(const sf::Mouse::Button button) const
{
	return sf::Mouse::isButtonPressed(button);
}

const bool& MouseManager::JustPressed(const sf::Mouse::Button button)
{
	//Check if the mouse button is pressed
	if (sf::Mouse::isButtonPressed(button))
	{
		//Check if the mouse button was pressed in the previous frame
		if (!pressedPreviously[button])
		{
			//The mouse button wasn't pressed in the previous frame, return true
			pressedPreviously[button] = true;
			return true;
		}
	}
	else
	{
		pressedPreviously[button] = false;
	}

	return false;
}

const bool& MouseManager::JustReleased(const sf::Mouse::Button button)
{
	//Check if the mouse button is not pressed
	if (!sf::Mouse::isButtonPressed(button))
	{
		//Check if the mouse button wasn't pressed in the previous frame
		if (!notPressedPreviously[button])
		{
			//The mouse button was pressed in the previous frame, return true
			notPressedPreviously[button] = true;
			return true;
		}
	}
	else
	{
		notPressedPreviously[button] = false;
	}

	return false;
}
