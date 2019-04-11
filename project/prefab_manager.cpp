#include "preabs_manager.h"
#include "levelsystem.h"
#include "components/cmp_sprite.h"
#include "components/cmp_player_movement.h"
#include "components/cmp_enemy_ai.h"
#include "components/cmp_physics.h"
#include "levelsystem.h"
#include "components//cmp_physics.h"
#include "components/cmp_tile.h"
#include "components/cmp_sprite.h"
#include "system_renderer.h"
#include "components/cmp_ghost.h"
#include "components/cmp_pathfind.h"
#include "pathfinder.h"
#include "system_resources.h"
#include "single_player_scene.h"

using namespace std;
using namespace sf;

// Create Player
std::shared_ptr<Entity> make_player()
{
	auto player = Engine::GetActiveScene()->makeEntity();
	player->setPosition(ls::getTilePosition(ls::findTiles(ls::START)[0]));
	player->addTag("player");
	auto s = player->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("tex.png");
	s->setTexture(tex);
	s->getSprite().setTextureRect(sf::IntRect(35, 0, 32, 32));
	s->getSprite().setScale(1.5f, 1.5f);
	s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);
	player->addComponent<PlayerMovementComponent>();
	auto p = player->addComponent<PhysicsComponent>(true, Vector2f(s->getSprite().getLocalBounds().width + 10.f, s->getSprite().getLocalBounds().height + 10.f));
	p->getBody()->SetSleepingAllowed(false);
	p->getBody()->SetFixedRotation(true);

	Engine::GetActiveScene()->ents.list.push_back(player);
	return player;
}

//Create Ghost
std::shared_ptr<Entity> make_ghost()
{
	auto ghost = Engine::GetActiveScene()->makeEntity();
	ghost->setPosition(ls::getTilePosition(ls::findTiles(ls::ENEMY)[0]));
	ghost->addTag("ghost");
	auto s = ghost->addComponent<SpriteComponent>();
	auto tex = Resources::get<Texture>("tex.png");
	s->setTexture(tex);
	s->getSprite().setTextureRect(sf::IntRect(165, 0, 32, 32));
	s->getSprite().setScale(1.5f, 1.5f);
	s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);
	auto g = ghost->addComponent<PhysicsComponent>(false, Vector2f(s->getSprite().getLocalBounds().width,
		s->getSprite().getLocalBounds().height));
	g->getBody()->SetSleepingAllowed(false);
	g->getBody()->SetFixedRotation(true);
	ghost->addComponent<EnemyAIComponent>();
	Engine::GetActiveScene()->ents.list.push_back(ghost);
	return ghost;
}

// Add physics to walls
void make_walls() 
{
	auto walls = ls::findTiles(ls::WALL);

	for (auto w : walls) 
	{
		auto e = Engine::GetActiveScene()->makeEntity();
		e->addTag("wall");
		e->setPosition(ls::getTilePosition(w) + Vector2f(ls::getTileSize() / 2, ls::getTileSize() / 2));
		e->addComponent<PhysicsComponent>(false, Vector2f(ls::getTileSize(), ls::getTileSize()));

		auto s = e->addComponent<SpriteComponent>();
		auto tex = Resources::get<Texture>("tex.png");
		s->setTexture(tex);
		s->getSprite().setTextureRect(sf::IntRect(32 * 9 + 5, 0, 32, 32));
		s->getSprite().setScale(1.875f, 1.875f);
		s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);

		Engine::GetActiveScene()->ents.list.push_back(e);
	}
}

// Add physics and breakable logic to breakable walls
void make_breakable_walls()
{
	auto blocks = ls::findTiles(ls::BREAKABLE);

	for (auto w : blocks)
	{
		auto e = Engine::GetActiveScene()->makeEntity();
		e->addTag("breakable");
		e->setPosition(ls::getTilePosition(w) + Vector2f(ls::getTileSize() / 2, ls::getTileSize() / 2));
		e->addComponent<PhysicsComponent>(false, Vector2f(ls::getTileSize(), ls::getTileSize()));

		auto s = e->addComponent<SpriteComponent>();
		auto tex = Resources::get<Texture>("tex.png");
		s->setTexture(tex);
		s->getSprite().setTextureRect(sf::IntRect(0, 32, 32, 32));
		s->getSprite().setScale(1.875f, 1.875f);
		s->getSprite().setOrigin(s->getSprite().getLocalBounds().width / 2, s->getSprite().getLocalBounds().height / 2);
		e->addComponent<TileComponent>();
		Engine::GetActiveScene()->ents.list.push_back(e);
	}
}