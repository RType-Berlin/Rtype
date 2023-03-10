# ShootingSystem

This system will detect if you pressed the shooting key (SPACE) and the server will send to the client the command to create a BULLET.

ShootingSystem.hpp:

```
#ifndef CLIENT_SRC_SYSTEMS_SHOOTINGSYSTEM_HPP_
#define CLIENT_SRC_SYSTEMS_SHOOTINGSYSTEM_HPP_

#include <memory>

#include "../CreateEntities/Init.hpp"
#include "../ECS/ISystem.hpp"
#include "../../Client/src/Action/Action.hpp"
#include "../../Server/Protocol/UdpServer.hpp"

class ShootingSystem : public ISystem {
 public:
  ShootingSystem(std::shared_ptr<EntityManager> t_em, UdpServer *t_serverCom,
                 rtype::IGraphicLoader *t_graphic_loader);
  ~ShootingSystem() = default;

  void update() override;
  void updateData(SystemData &t_data) override;

 private:
  EventQueue m_event_queue;
  UdpServer *m_serverCom;
  rtype::IGraphicLoader *m_graphic_loader;

  void shootMoney(std::shared_ptr<Action> t_action);
  void shootFire(std::shared_ptr<Action> t_action);
  void shootBomb(std::shared_ptr<Action> t_action);
};

#endif  // CLIENT_SRC_SYSTEMS_SHOOTINGSYSTEM_HPP_
```

ShootingSystem.cpp:

```
#include "ShootingSystem.hpp"

ShootingSystem::ShootingSystem(std::shared_ptr<EntityManager> t_em,
                               UdpServer *t_serverCom,
                               rtype::IGraphicLoader *t_graphic_loader) {
  m_em = t_em;
  m_serverCom = t_serverCom;
  m_graphic_loader = t_graphic_loader;
}

void ShootingSystem::updateData(SystemData &t_data) {
  m_event_queue = t_data.event_queue;
}

void ShootingSystem::update() {
  for (std::shared_ptr<Action> action :
       m_event_queue.getAllOfType(Action::ActionType::SHOOT)) {
    for (EntityID ent : EntityViewer<Player>(*m_em)) {
      if (ent != action->getId()) continue;

      switch (action->getShootType()) {
        case Action::ShootingType::NORMAL:
          initShoot(action, m_em, m_graphic_loader, m_serverCom);
          break;
        case Action::ShootingType::COIN:
          shootMoney(action);
          break;
        case Action::ShootingType::FIRE:
          shootFire(action);
          break;
        case Action::ShootingType::BOMB:
          shootBomb(action);
          break;
        default:
          break;
      }
    }
  }
}

void ShootingSystem::shootMoney(std::shared_ptr<Action> t_action) {
  Player *player = (*m_em).Get<Player>(t_action->getId());

  if (player->coin_shot == 0) return;
  player->coin_shot -= 1;
  if (player->coin_shot < 0) player->coin_shot = 0;

  initCoinShoot(t_action, m_em, m_graphic_loader, m_serverCom);
}

void ShootingSystem::shootFire(std::shared_ptr<Action> t_action) {
  Player *player = (*m_em).Get<Player>(t_action->getId());

  if (player->fire_shot == 0) return;
  player->fire_shot -= 1;
  if (player->fire_shot < 0) player->fire_shot = 0;

  initFireShoot(t_action, m_em, m_graphic_loader, m_serverCom);
}

void ShootingSystem::shootBomb(std::shared_ptr<Action> t_action) {
  Player *player = (*m_em).Get<Player>(t_action->getId());

  if (player->bomb_shot == 0) return;
  player->bomb_shot -= 1;
  if (player->bomb_shot < 0) player->bomb_shot = 0;

  initBombShoot(t_action, m_em, m_graphic_loader, m_serverCom);
}
```

[Back](../game-engine/systems.md)
