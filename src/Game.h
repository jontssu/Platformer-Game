#pragma once

#include "Player.h"
#include "Tilemap.h"

class Game {
	public:
		Game();
		virtual ~Game();

		const sf::RenderWindow& getWindow() const;

		void updateInput();
		void updatePlayer();
		void updateCollision();
		void updateTileMap();
		void update();

		void renderPlayer();
		void renderTileMap();
		void render();

	private:
		sf::RenderWindow window;
		sf::Texture tileSheet;

		Player* player;
		Tilemap* tileMap;
		std::map<std::string, sf::Keyboard::Key> keyboardMappings;
		std::map<std::string, sf::Mouse::Button> mouseMappings;;

		void initWindow();
		void initInput();
		void initTileSheet();
		void initPlayer();
		void initTileMap();

};
