#include "Game.h"

using namespace sf;
using namespace std::chrono;

/*
* Private Functions
*/
void Game::initVariables()
{
	this->window = nullptr;

	// FPS title displaying
	this->displayFpsTitle = false;
	this->fpsUpdateInterval = std::chrono::milliseconds(100);
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new RenderWindow(VideoMode(800, 600), "Game Window", Style::Default);
}

void Game::PollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case Event::Closed:
			this->window->close();
			break;

		case Event::KeyPressed:
			if (this->ev.key.code == Keyboard::Escape)
			{
				this->window->close();
				break;
			}
		}
	}
}

void Game::preRender() // ran before rendering
{
	this->start = steady_clock::now();
}

void Game::postRender() // ran right after rendering
{

	// FPS window title displaying
	this->end = steady_clock::now();
	if (this->displayFpsTitle)
	{
		if (this->nextUpdate <= steady_clock::now()) // only updates every nextUpdate
		{
			auto frameDuration = duration_cast<std::chrono::microseconds>(this->end - this->start).count();
			float fps = 1.0f / (frameDuration / 1e6); // from micro seconds to seconds (micro sec * 1,000,000 or 1e6) 

			std::stringstream stream;
			stream << std::fixed << std::setprecision(3) << fps; // set precision to 3 decimal places
			std::string fpsString = stream.str();

			this->window->setTitle("FPS: " + fpsString);

			this->nextUpdate = steady_clock::now() + this->fpsUpdateInterval; // set the next time it will update
		}
	}
}

/*
* Constructors
*/
Game::Game()
{
	this->initVariables(); // initialize all variables
	this->initWindow(); // initialize window
}

Game::~Game()
{
	delete this->window;
}

/*
* Accessors
*/
const bool Game::getIsRunning() const
{
	return this->window->isOpen();
}

void Game::setDisplayTitleFps(bool boolPar) { // lets you set if the fps is displayed in the title or not
	this->displayFpsTitle = boolPar;
}

void Game::setFpsUpdateInterval(std::chrono::milliseconds msPar) { // lets you set how often the fps changes in the title in milliseconds
	this->fpsUpdateInterval = msPar;
}

/*
* Public functions
*/

void Game::update() // game logic and functionality
{
	this->PollEvents();

}

void Game::render() // rendering pixels on screen
{
	/*
	* Renders the game in order:
	* - clear window with color
	* - draw objects
	* - display window
	*/

	this->preRender();
	this->window->clear();

	// display objects here

	this->window->display();
	this->postRender();
}
