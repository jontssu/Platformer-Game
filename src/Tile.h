#pragma once
#include "Collider.h"

enum TILE_TYPES {
	TILE_NORMAL = 1,
	TILE_POWERUP_EXTRA_BLOCK = 2,
	TILE_POWERUP_JUMP_BOOST = 3,
	TILE_PLACEABLE = 4,
	TILE_DEADLY = 5,
	TILE_WIN = 6
};

class Tile {
	public:
		Tile(const unsigned& grid_pos_x, const unsigned& grid_pos_y, const unsigned& tile_size, sf::Texture* texture_sheet, sf::IntRect texture_rect, bool damaging = false, int tile_type = TILE_NORMAL);
		virtual ~Tile() {}

		const sf::FloatRect getGlobalBounds() const;
		inline const sf::Sprite& getSprite() const { return sprite; };
		inline const int getType() const { return tileType; }; 
		inline const bool getIsSpawned() const { return isSpawned; };
		Collider getCollider() { return Collider(body); }

		void setDespawn();

		void update();
		void render(sf::RenderTarget& target);

	private:
	sf::RectangleShape body;
	sf::Sprite sprite;
	bool isSpawned;
	int timer;
	int tileType;
	const bool damaging;

};