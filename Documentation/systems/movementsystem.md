# MovementSystem

The MovementSystem handles the movement. In our implementation it works like this: If you press a movement key (W, A, S, D) the player will continue to move in this direction till it arrives at the screen border or a new direction is given.

MovementSystem.hpp:

```
#ifndef CLIENT_SRC_SYSTEMS_MOVEMENTSYSTEM_HPP_
#define CLIENT_SRC_SYSTEMS_MOVEMENTSYSTEM_HPP_

#include <string>

#include "../ECS/DataTypesECS.hpp"
#include "../ECS/ISystem.hpp"
#include "../../Client/src/Action/Action.hpp"
#include "../../Server/Protocol/UdpServer.hpp"
#include "../../Client/Protocol/UdpClient.hpp"

class MovementSystem : public ISystem {
 public:
  MovementSystem(std::shared_ptr<EntityManager> t_em, UdpServer *t_serverCom);
  MovementSystem(std::shared_ptr<EntityManager> t_em, UdpServer *t_serverCom,
                 UdpClient *t_clientCom);
  ~MovementSystem() = default;

  void update() override;
  void updateData(SystemData &t_data) override;

 private:
  EventQueue m_event_queue;
  UdpServer *m_serverCom;
  UdpClient *m_clientCom;

  void playerObstacleInteraction(Pos &t_position, const rtype::Vector2f &t_size,
                                 float t_speed);
  void keepPlayerInsideScreen(rtype::Vector2f &t_position,
                              const rtype::Vector2f &t_size);
  void updatePlayer(EntityID t_ent);
  void updateBackground(EntityID t_ent);
  void updateBullets(EntityID t_ent);
};

#endif  // CLIENT_SRC_SYSTEMS_MOVEMENTSYSTEM_HPP_
```

MovementSystem.cpp:

```
#include "MovementSystem.hpp"

MovementSystem::MovementSystem(std::shared_ptr<EntityManager> t_em,
                               UdpServer *t_serverCom) {
  m_em = t_em;
  m_serverCom = t_serverCom;
  m_clientCom = nullptr;
}

MovementSystem::MovementSystem(std::shared_ptr<EntityManager> t_em,
                               UdpServer *t_serverCom, UdpClient *t_clientCom) {
  m_em = t_em;
  m_clientCom = t_clientCom;
  m_serverCom = t_serverCom;
}

void MovementSystem::updateData(SystemData &t_data) {
  m_event_queue = t_data.event_queue;
}

void MovementSystem::update() {
  for (EntityID ent : EntityViewer<Player>(*m_em)) {
    updatePlayer(ent);
  }
  for (EntityID ent : EntityViewer<BackgroundLayer>(*m_em)) {
    updateBackground(ent);
  }
  for (EntityID ent : EntityViewer<Obstacle>(*m_em)) {
    Obstacle *obstacle = (*m_em).Get<Obstacle>(ent);
    obstacle->position.position.x += obstacle->position.velocity.x;
    if (obstacle->position.position.x <= obstacle->limit) {
      obstacle->position.position.x = obstacle->original_x;
    }
    if (int(obstacle->position.position.x) % 50 == 0 && m_serverCom) {
      m_serverCom->addEvent(
        std::make_shared<Action>(PosAction(ent, obstacle->position.position)));
    }
    if (!m_serverCom && m_event_queue.hasLatestPos(ent)) {
      obstacle->position.position = m_event_queue.getLatestPos(ent);
    }
    obstacle->body->setPosition(obstacle->position.position);
  }
  for (EntityID ent : EntityViewer<Bullet>(*m_em)) {
    updateBullets(ent);
  }
  for (EntityID ent : EntityViewer<AnimationObj>(*m_em)) {
    AnimationObj *anim = (*m_em).Get<AnimationObj>(ent);
    anim->position.position += anim->position.velocity;
    anim->body->setPosition(
      {anim->position.position.x, anim->position.position.y});
    if (anim->position.position.x < 0) { m_em->destroyEntity(ent); }
  }
  for (EntityID ent : EntityViewer<Enemy>(*m_em)) {
    Enemy *enem = (*m_em).Get<Enemy>(ent);
    enem->obj->position.position += enem->obj->position.velocity;
    enem->obj->body->setPosition(
      {enem->obj->position.position.x, enem->obj->position.position.y});
    enem->health.left_health->setPosition(
      {enem->obj->position.position.x + enem->health.offset.x,
       enem->obj->position.position.y + enem->health.offset.y});
    enem->health.missing_health->setPosition(
      {enem->obj->position.position.x + enem->health.offset.x,
       enem->obj->position.position.y + enem->health.offset.y});
    if (enem->obj->position.position.x < 0) { m_em->destroyEntity(ent); }
  }
  for (EntityID ent : EntityViewer<SpinningItem>(*m_em)) {
    SpinningItem *anim = (*m_em).Get<SpinningItem>(ent);
    anim->position.position += anim->position.velocity;
    anim->body->setPosition(
      {anim->position.position.x, anim->position.position.y});
    if (anim->position.position.x < 0) { m_em->destroyEntity(ent); }
  }
}

void MovementSystem::playerObstacleInteraction(Pos &t_position,
                                               const rtype::Vector2f &t_size,
                                               float t_speed) {
  std::string type;
  for (EntityID ent : EntityViewer<Obstacle>(*m_em)) {
    Obstacle *obstacle = (*m_em).Get<Obstacle>(ent);
    rtype::IRectangleShape::SIDE side = obstacle->body->intersectsSide({
      t_position.position.x,
      t_position.position.y,
      t_size.x,
      t_size.y,
    });
    if (side == rtype::IRectangleShape::SIDE::NONE) continue;
    t_speed /= 2;
    if (side == rtype::IRectangleShape::SIDE::LEFT) {
      type = "LEFT";
      t_position.position.x += t_speed;
    } else if (side == rtype::IRectangleShape::SIDE::RIGHT) {
      type = "RIGHT";
      t_position.position.x -= t_speed;
    } else if (side == rtype::IRectangleShape::SIDE::TOP) {
      type = "TOP";
      t_position.position.y += t_speed;
    } else if (side == rtype::IRectangleShape::SIDE::BOTTOM) {
      type = "BOTTOM";
      t_position.position.y -= t_speed;
    }
    t_position.velocity = {0, 0};
    std::cout << "obstacle collision detected: " << type << std::endl;
  }
}

void MovementSystem::keepPlayerInsideScreen(rtype::Vector2f &t_position,
                                            const rtype::Vector2f &t_size) {
  int screen_width = 800;
  int screen_height = 800;

  for (EntityID ent : EntityViewer<Enemy>(*m_em)) {
    Enemy *enem = (*m_em).Get<Enemy>(ent);
    if (enem->obj->type == "paywall") {
      screen_width = enem->obj->position.position.x + 30;
      if (screen_width > 800) screen_width = 800;
    }
  }
  if (t_position.x - t_size.x <= 0) {
    t_position.x = t_size.x;
  } else if (t_position.x >= screen_width) {
    t_position.x = screen_width;
  }
  if (t_position.y < 0) {
    t_position.y = 0;
  } else if (t_position.y + t_size.y >= screen_height) {
    t_position.y = screen_height - t_size.y;
  }
}

void MovementSystem::updatePlayer(EntityID t_ent) {
  Player *player = (*m_em).Get<Player>(t_ent);
  int left = 0;
  int right = 0;
  int up = 0;
  int down = 0;
  int pos = 0;
  rtype::Vector2f orig_pos = player->position.position;

  for (int i = 0; i < m_event_queue.getEventQueue().size(); i++) {
    Action::ActionType action =
      m_event_queue.getEventQueue()[i].get()->getType();
    if (m_clientCom != nullptr && player->player_id != m_clientCom->m_id) {
      if (m_event_queue.getEventQueue()[i].get()->getId() == t_ent) {
        if (action == Action::ActionType::LEFT) {
          left = 1;
        } else if (action == Action::ActionType::UP) {
          up = 1;
        } else if (action == Action::ActionType::DOWN) {
          down = 1;
        } else if (action == Action::ActionType::RIGHT) {
          right = 1;
        } else if (action == Action::ActionType::POS) {
          pos = 1;
        }
      }
    } else if (m_event_queue.getEventQueue()[i].get()->getId() == t_ent) {
      if (action == Action::ActionType::LEFT) {
        left = 1;
      } else if (action == Action::ActionType::UP) {
        up = 1;
      } else if (action == Action::ActionType::DOWN) {
        down = 1;
      } else if (action == Action::ActionType::RIGHT) {
        right = 1;
      } else if (action == Action::ActionType::POS) {
        pos = 1;
      }
    }
  }
  rtype::Vector2f direction = {0, 0};

  if (left) { direction.x = -1; }
  if (right) { direction.x = 1; }
  if (up) { direction.y = -1; }
  if (down) { direction.y = 1; }
  if (direction.x != 0 || direction.y != 0) {
    player->position.velocity = direction * player->speed;
  }
  player->position.position += player->position.velocity;
  keepPlayerInsideScreen(
    player->position.position,
    {player->body->getSize().x, player->body->getSize().y});
  playerObstacleInteraction(player->position,
                            {
                              player->body->getSize().x,
                              player->body->getSize().y,
                            },
                            player->speed);
  if (pos) { player->position.position = m_event_queue.getLatestPos(t_ent); }
  player->body->setPosition(
    {player->position.position.x, player->position.position.y});
  player->health.body->setPosition(
    {player->position.position.x - 180, player->position.position.y - 70});

  if (player->position.velocity.x != 0 || player->position.velocity.y != 0)
    player->position.velocity *= 0.99f;
  if (m_serverCom != nullptr) {
    if (orig_pos != player->position.position) {
      m_serverCom->addEvent(
        std::make_shared<Action>(PosAction(t_ent, player->position.position)));
    }
  }
}

void MovementSystem::updateBackground(EntityID t_ent) {
  BackgroundLayer *layer = (*m_em).Get<BackgroundLayer>(t_ent);
  layer->position.x -= layer->speed;
  if (layer->position.x <= layer->limit) { layer->position.x = 0; }
  layer->sprite.setPosition(layer->position);
}

void MovementSystem::updateBullets(EntityID t_ent) {
  Bullet *bullet = (*m_em).Get<Bullet>(t_ent);

  bullet->pos.x += bullet->speed;
  bullet->body->setPosition({bullet->pos.x, bullet->pos.y});
  if (bullet->pos.x >= 800) { m_em->destroyEntity(t_ent); }
}
```

[Back](../game-engine/systems.md)
