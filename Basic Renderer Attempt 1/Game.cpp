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

	// Game logic
	this->points = 0;
	this->enemySpawnInterval = std::chrono::seconds(4); // time in seconds
	this->enemyNextSpawn = steady_clock::now() + this->enemySpawnInterval;
	this->maxEnemys = 5;

	this->enemyMoveInterval = std::chrono::milliseconds(5);
	this->enemyNextMove = steady_clock::now() + this->enemyMoveInterval;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new RenderWindow(VideoMode(800, 600), "Game Window", Style::Default);
	//this->window->setFramerateLimit(144);
}

void Game::initEnemys()
{
	this->enemy.setPosition(10.f, 10.f);

	this->enemy.setSize(Vector2f(50.f, 50.f));
	this->enemy.setFillColor(Color::Red);
	this->enemy.setOutlineColor(Color::Yellow);
	this->enemy.setOutlineThickness(1.f);
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

void Game::UpdateMousePos()
{
	// updates position of the mouse

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
}

void Game::UpdateEnemies()
{
	// updates enemies by spawning and incrementing timer for next spawn if less than max enemys
	if (this->enemys.size() < this->maxEnemys)
	{
		if (this->enemyNextSpawn <= steady_clock::now())
		{
			this->spawnEnemy();
			this->enemyNextSpawn = steady_clock::now() + this->enemySpawnInterval;
		}
	}

	// moves enemys
	if (this->enemyNextMove <= steady_clock::now())
	{
		for (int i = 0; i < this->enemys.size(); i++)
		{
			this->enemys[i].move(0.f, 1.f);
		}
		
		this->enemyNextMove = steady_clock::now() + this->enemyMoveInterval;
	}

	// delete enemys if they reach the bottom of the screen

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

void Game::renderEnemys()
{
	for (int i = 0; i < this->enemys.size(); i++)
	{
		this->window->draw(this->enemys[i]);
	}
	//this->window->draw(this->enemy);
}

/*
* Constructors
*/
Game::Game()
{
	this->initVariables(); // initialize all variables
	this->initWindow(); // initialize window
	this->initEnemys();
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
void Game::spawnEnemy()
{
	// spawns enemies into the game and sets their pos/color

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	this->enemy.setFillColor(Color::Red);

	// spawn enemy
	this->enemys.push_back(this->enemy);
}

void Game::update() // game logic and functionality
{
	this->PollEvents();
	this->UpdateMousePos();
	this->UpdateEnemies();
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

	this->renderEnemys();

	this->window->display();
	this->postRender();
}
