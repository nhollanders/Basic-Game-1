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
	this->difficulty = 1;
	this->endGame = false;
	this->health = 10;
	this->points = 0;
	this->mouseHeld = false;
	this->enemySpawnInterval = std::chrono::milliseconds(500); // time in seconds
	this->enemyNextSpawn = steady_clock::now() + this->enemySpawnInterval;
	this->maxEnemys = 20;

	this->enemyMoveInterval = std::chrono::milliseconds(5);
	this->enemyNextMove = steady_clock::now() + this->enemyMoveInterval;

	// Extra
	this->debugText = false;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new RenderWindow(VideoMode(800, 600), "Balls", Style::Titlebar | Style::Close);
	this->window->setIcon(this->windowIcon.getSize().x, this->windowIcon.getSize().y, this->windowIcon.getPixelsPtr());
	//this->window->setFramerateLimit(144);
}

void Game::initEnemys()
{
	this->enemy.setPosition(10.f, 10.f);

	this->enemy.setRadius(25.f);
	this->enemy.setFillColor(Color(50, 50, 50, 255));
	this->enemy.setOutlineColor(Color(25, 25, 25, 255));
	this->enemy.setOutlineThickness(1.f);
}

void Game::initResources()
{
	bool noFontLoadIssues = true;
	bool noResourceLoadIssues = true;

	noFontLoadIssues = this->font.loadFromFile("Fonts/PolygonParty.ttf");
	noResourceLoadIssues = this->windowIcon.loadFromFile("Resources/balls_icon.png");

	if (!noFontLoadIssues)
		std::cout << "Error loading fonts!" << "\n";
	if (!noResourceLoadIssues)
		std::cout << "Error loading resources!" << "\n";
}

void Game::initText()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(24);
	this->uiText.setFillColor(Color::White);
	this->uiText.setString("NONE");
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
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow); // convert from pixel to coord on the window
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

	// moves enemys and deletes if they are outside bottom bounds
	if (this->enemyNextMove <= steady_clock::now())
	{
		for (int i = 0; i < this->enemys.size(); i++)
		{
			if (this->enemys[i].getFillColor() == Color::Red)
			{
				this->enemys[i].move(0.f, (2.f) * this->difficulty);
			}
			else
			{
				this->enemys[i].move(0.f, (1.f) * this->difficulty);
			}

			// if enemy went past the screen bottom
			if (this->enemys[i].getPosition().y > this->window->getSize().y)
			{
				this->enemys.erase(this->enemys.begin() + i);
				this->health -= 1;
			}
		}
		

		this->enemyNextMove = steady_clock::now() + this->enemyMoveInterval;
	}

	// check if the enemy gets clicked
	if (Mouse::isButtonPressed(Mouse::Left))
	{
		if (this->mouseHeld == false)
		{
			bool deleted = false;
			this->mouseHeld = true;

			for (int i = 0; i < this->enemys.size() && deleted == false; i++)
			{
				if (this->enemys[i].getGlobalBounds().contains(this->mousePosView))
				{
					if (this->enemys[i].getFillColor() == Color::Red)
					{
						this->points += 4;
						this->health += 1; // increase hp for hitting red balls

						this->difficulty = std::max(log(static_cast<float>(points) / 20.f), 1.f);
					}
					else
					{
						this->points += 1;

						this->difficulty = std::max(log(static_cast<float>(points) / 20.f), 1.f);
					}
				
					deleted = true;
					this->enemys.erase(this->enemys.begin() + i);
				}
			}
		}
	}
	else
	{
		this->mouseHeld = false;
	}
}

void Game::UpdateText()
{
	std::stringstream ss;

	switch (this->debugText)
	{
	case 0: // debug text disabled
		ss << "Points: " << this->points << "\n"
			<< "Health: " << this->health << "\n";
		break;
	case 1: // debug text enabled
		ss << "Points: " << this->points << "\n"
			<< "Health: " << this->health << "\n"
			<< "Difficulty: " << this->difficulty << "\n";
		break;
	}

	this->uiText.setString(ss.str());
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

void Game::renderText()
{
	this->window->draw(this->uiText);
}

/*
* Constructors
*/
Game::Game()
{
	this->initVariables(); // initialize all variables
	this->initResources(); // initializes resources like textures
	this->initText();
	this->initWindow(); // initialize window
	this->initEnemys(); // initializes the enemy template
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

const bool Game::getEndGame() const
{
	return this->endGame;
}

void Game::setDebugText(bool boolPar)
{
	this->debugText = boolPar;
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
	float randScale = static_cast<float>((rand() % 2) + 1);
	this->enemy.setFillColor(Color(50, 50, 50, 255));

	if (static_cast<int>(rand() % 8) == 2) // extra points harder to catch
	{
		randScale = 0.5;
		this->enemy.setFillColor(Color::Red);
	}

	this->enemy.scale(Vector2f(randScale, randScale));

	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - ((this->enemy.getRadius()*this->enemy.getScale().x) * 2))),
		-((this->enemy.getRadius() * this->enemy.getScale().y) * 2)
	);

	// spawn enemy
	this->enemys.push_back(this->enemy);

	this->enemy.scale(1.f / randScale, 1.f / randScale);

}

void Game::update() // game logic and functionality
{
	this->PollEvents();

	if (this->health <= 0) // ends the game
	{
		this->endGame = true;
	}

	if (!this->endGame)
	{
		this->UpdateText();
		this->UpdateMousePos();
		this->UpdateEnemies(); // update position, spawning, and detection
	}
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
	this->renderText();

	this->window->display();
	this->postRender();
}
