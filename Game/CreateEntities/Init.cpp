#include "Init.hpp"

void initPlayer(EntityManager &t_entity_manager) {
  EntityID player = t_entity_manager.createNewEntity();
  SpriteECS player_sprite = SpriteECS("../Client/sprites/starship.png");

  float *player_speed = t_entity_manager.Assign<float>(player, float(10));
  Pos *player_pos = t_entity_manager.Assign<Pos>(
    player, Pos{sf::Vector2f(0, 0), sf::Vector2f(300, 300)});

  sf::RectangleShape body;
  body.setSize({200, 200});
  body.setPosition(player_pos->position);
  body.setTexture(player_sprite.getTexture());
  body.setRotation(90.0);
  body.setOutlineColor(sf::Color::Red);
  //body.setOutlineThickness(5);
  sf::RectangleShape *player_body =
    t_entity_manager.Assign<sf::RectangleShape>(player, body);
}

void initBackground(EntityManager &t_entity_manager) {
  // background layer 1
  EntityID background1 = t_entity_manager.createNewEntity();
  t_entity_manager.Assign<int>(background1, (-272 * 5));
  t_entity_manager.Assign<float>(background1, 1);
  t_entity_manager.Assign<sf::Vector2f>(background1, sf::Vector2f(0, 0));
  t_entity_manager.Assign<SpriteECS>(
    background1, SpriteECS("../Client/sprites/background/bg1.png", {5, 5}));

  // background layer 2
  EntityID background2 = t_entity_manager.createNewEntity();
  t_entity_manager.Assign<int>(background2, (-272 * 10));
  t_entity_manager.Assign<float>(background2, 2);
  t_entity_manager.Assign<sf::Vector2f>(background2, sf::Vector2f(0, 160));
  t_entity_manager.Assign<SpriteECS>(
    background2, SpriteECS("../Client/sprites/background/bg2.png", {4, 4}));

  // background layer 3
  EntityID background3 = t_entity_manager.createNewEntity();
  t_entity_manager.Assign<int>(background3, (-272 * 5));
  t_entity_manager.Assign<float>(background3, 3);
  t_entity_manager.Assign<sf::Vector2f>(background3, sf::Vector2f(0, 0));
  t_entity_manager.Assign<SpriteECS>(
    background3, SpriteECS("../Client/sprites/background/bg3.png", {5, 5}));
}

//void initEnemy()