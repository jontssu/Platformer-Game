#pragma once

#include "Player.h"
#include "Tilemap.h"

enum class GameState {
	MENU,
	CHOOSING_LEVEL,
	INSTRUCTIONS,
	PLAYING,
	PAUSED,
	FINISHED
};

class Game {
	public:
		Game();
		virtual ~Game();

		const sf::RenderWindow& getWindow() const;

		void updateInput();
		void updatePlayer();
		void updateCollision();
		void updateTileMap();
		void updateMenu();
		void update();

		void renderPlayer();
		void renderTileMap();
		void renderTime();
		void renderAmountOfPlaceableBlocks();
		void renderMenu();
		void renderFinishedScreen();
		void render();

	private:
		sf::RenderWindow window;
		sf::Texture tileSheet;
		sf::Font font;
		int currentLevel;
		GameState currentState;
		float time;
		float bestTime;
		bool newHighScore;

		Player* player;
		Tilemap* tileMap;
		std::map<std::string, sf::Keyboard::Key> keyboardMappings;
		std::map<std::string, sf::Mouse::Button> mouseMappings;;

		void initWindow();
		void initInput();
		void initTileSheet();
		void initPlayer();
		void initTileMap();
		void initFont();
};
