#ifndef GAME_H
#define GAME_H

// STL includes
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

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

		// Window FPS displaying
		std::chrono::steady_clock::time_point start, end, nextUpdate;
		std::chrono::milliseconds fpsUpdateInterval;
		bool displayFpsTitle;

		// Private functions
		void initVariables();
		void initWindow();

		// Polling
		void PollEvents();

		// Rendering
		void preRender();
		void postRender();

	public:
		// contstructors & deconstructors
		Game();
		~Game();

		// Accesors
		const bool getIsRunning()const;

		void setDisplayTitleFps(bool boolPar);
		void setFpsUpdateInterval(std::chrono::milliseconds msPar);

		// function
		void update();

		// Rendering
		void render();
};

#endif // !GAME_H