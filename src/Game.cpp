#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	initWindow();
	initTileSheet();
	initPlayer();
	initTileMap();
}

Game::~Game()
{
	delete player;
	delete tileMap;
}

const sf::RenderWindow &Game::getWindow() const
{
	return window;
}

void Game::initWindow()
{
	window.create(sf::VideoMode({800, 600}), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	window.setFramerateLimit(60);
}

void Game::initInput()
{
	
}

void Game::initTileSheet()
{
	if (!tileSheet.loadFromFile("textures/texturesheet.png"))
	{
		std::cout << "ERROR::GAME::INITTILESHEET::FAILED TO LOAD TILESHEET" << "\n";
	}
}

void Game::initPlayer()
{
	player = new Player();
}
void Game::initTileMap()
{
	tileMap = new Tilemap(20, 20, &tileSheet, 64);
	tileMap->addTile(0, 0);
}

void Game::update()
{
	//Polling window events
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			window.close();
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->code == sf::Keyboard::Key::Escape)
				window.close();
		}
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
		{
			if (keyReleased->code == sf::Keyboard::Key::Up ||
				keyReleased->code == sf::Keyboard::Key::Down ||
				keyReleased->code == sf::Keyboard::Key::Left ||
				keyReleased->code == sf::Keyboard::Key::Right)
			{
				player->resetAnimationTimer();
			}
		}
	}
	updatePlayer();

	updateCollision();

	updateTileMap();
}

void Game::renderPlayer()
{
	player->render(window);
}

void Game::renderTileMap()
{
	tileMap->render(window);
}

void Game::updateInput()
{
}

void Game::updatePlayer()
{
	player->update();
}

void Game::updateCollision()
{
	//Collision bottom of screen
	if (player->getPosition().y + player->getGlobalBounds().size.y >= window.getSize().y)
	{
		player->setCanJump(true);
		player->resetVelocityY();
		player->setPosition(
			player->getPosition().x,
			window.getSize().y - player->getGlobalBounds().size.y);
	}
}

void Game::updateTileMap()
{
	tileMap->update();
}

void Game::render()
{
	window.clear();

	//Render game
	renderTileMap();
	renderPlayer();

	window.display();
}
