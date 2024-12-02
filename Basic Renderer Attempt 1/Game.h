#ifndef GAME_H
#define GAME_H

// STL includes
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
#include <cmath>

// SFML includes
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>

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
		sf::Vector2f mousePosView;

		// Window FPS displaying
		std::chrono::steady_clock::time_point start, end, nextUpdate;
		std::chrono::milliseconds fpsUpdateInterval;
		bool displayFpsTitle;

		// Private functions
		void initVariables();
		void initWindow();
		void initEnemys();
		void initResources();
		void initText();

		// Pivate update functions
		void PollEvents();
		void UpdateMousePos();
		void UpdateEnemies();
		void UpdateText();

		// Rendering
		void preRender();
		void postRender();
		void renderEnemys();
		void renderText();

		// Resources
		sf::Font font;
		sf::Image windowIcon;
		sf::SoundBuffer popSoundBuf;
		sf::Sound popSound;

		// Text
		bool debugText;
		sf::Text uiText;

		//Game Logic
		float difficulty; // scalar for time difficulty
		unsigned int points;
		bool endGame;
		bool mouseHeld;
		int health;

		// enemy logic
		std::chrono::steady_clock::time_point enemyNextSpawn;
		std::chrono::milliseconds enemySpawnInterval;
		std::chrono::steady_clock::time_point enemyNextMove;
		std::chrono::milliseconds enemyMoveInterval;
		unsigned int maxEnemys;

		// Game Objects
		std::vector<sf::CircleShape> enemys;
		sf::CircleShape enemy;

	public:
		// contstructors & deconstructors
		Game();
		~Game();

		// Accesors
		const bool getIsRunning()const;
		const bool getEndGame()const;

		void setDebugText(bool boolPar);
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