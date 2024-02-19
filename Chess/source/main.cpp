
#include "Global.hpp"
#include "Board.hpp"

//Window default variables
sf::RenderWindow window;
sf::Color backgroundColor = sf::Color::Black;

//Board variables
std::unique_ptr<Board> board;

//Start project function
void Create()
{
	//Create the board
	board = std::make_unique<Board>(&window);
	board->LoadFEN(BOARD::STARTING_FEN);
}

//Game loop function
void Update(const float& deltaTime)
{
	board->Update(deltaTime);
}

//Window event function
void Event(const float& deltaTime, const sf::Event& event)
{

}

//Draw sprites on the window
void Draw(const float& deltaTime)
{
	board->Draw();
}

//Update the deltaTime, window events and rendering
void UpdateWindow(sf::Clock& deltaTimeClock)
{
	//Create update variables
	const float deltaTime = deltaTimeClock.restart().asSeconds();
	sf::Event windowEvent;

	//If visual studio is on debug mode, it shows the amount of fps of the game
#ifdef _DEBUG
	const std::string fps = std::to_string(static_cast<unsigned int>(round(1.f / deltaTime)));
	window.setTitle(WINDOW::TITLE + " | FPS: " + fps);
#endif

	//Update events and function events
	while (window.pollEvent(windowEvent))
	{
		//Close window button event
		switch (windowEvent.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		}

		//Update the main event function
		Event(deltaTime, windowEvent);
	}

	//Update the main update function
	Update(deltaTime);

	//Render window and update the main draw function
	window.clear(backgroundColor);
	Draw(deltaTime);
	window.display();
}

//Create the window and delta time clock
void CreateWindow()
{
	//Create the window settings
	sf::ContextSettings settings;

	//Create window
	window.create(sf::VideoMode(WINDOW::SIZE.x, WINDOW::SIZE.y), WINDOW::TITLE, WINDOW::STYLE);
	Create();

	//Update window
	sf::Clock deltaTimeClock;

	//Update the window function
	while (window.isOpen())
	{
		UpdateWindow(deltaTimeClock);
	}
}

int main()
{
	//Start the game and update the window
	CreateWindow();
	return 0;
}