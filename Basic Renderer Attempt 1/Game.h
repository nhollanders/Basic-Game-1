#ifndef GAME_H
#define GAME_H

// STL includes
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>

// SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
* Primary Game Engine Wrapper Class
*/

class Game
{
	private:
		// Variables

		// Windows
		sf::RenderWindow* window;
		sf::VideoMode videoMode;
		sf::Event ev;

		//Mouse positions
		sf::Vector2i mousePosWindow;

		// Window FPS displaying
		std::chrono::steady_clock::time_point start, end, nextUpdate;
		std::chrono::milliseconds fpsUpdateInterval;
		bool displayFpsTitle;

		// Private functions
		void initVariables();
		void initWindow();
		void initEnemys();

		// Pivate update functions
		void PollEvents();
		void UpdateMousePos();
		void UpdateEnemies();

		// Rendering
		void preRender();
		void postRender();
		void renderEnemys();

		//GameLogic
		int points;

		std::chrono::steady_clock::time_point enemyNextSpawn;
		std::chrono::seconds enemySpawnInterval;
		std::chrono::steady_clock::time_point enemyNextMove;
		std::chrono::milliseconds enemyMoveInterval;
		int maxEnemys;

		// Game Objects
		std::vector<sf::RectangleShape> enemys;
		sf::RectangleShape enemy;

	public:
		// contstructors & deconstructors
		Game();
		~Game();

		// Accesors
		const bool getIsRunning()const;

		void setDisplayTitleFps(bool boolPar);
		void setFpsUpdateInterval(std::chrono::milliseconds msPar);

		// functions
		void spawnEnemy();

		// updates
		void update();

		// Rendering
		void render();
};

#endif // !GAME_H