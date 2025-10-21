#pragma once

class Tile {
	public:
		Tile(const unsigned& grid_pos_x, const unsigned& grid_pos_y, const unsigned& tile_size, sf::Texture* texture_sheet, sf::IntRect texture_rect, bool damaging = false);
		virtual ~Tile() {}

		const sf::FloatRect getGlobalBounds() const;

		void update();
		void render(sf::RenderTarget& target);

	private:
	sf::Sprite sprite;
	const bool damaging;

};