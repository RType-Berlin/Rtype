#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP

#include "./ECS/EntityManager.hpp"
#include "./ECS/DataTypesECS.hpp"
#include "./ECS/ISystem.hpp"
#include "./Systems/AnimationSystem.hpp"
#include "./Systems/CollisionSystem.hpp"
#include "./Systems/DisplaySystem.hpp"
#include "./Systems/MovementSystem.hpp"
#include "./Systems/RandomEnemyGeneratorSystem.hpp"
#include "./Systems/ShootingSystem.hpp"
#include "../Client/src/InputManager/InputManager.hpp"
#include "../Client/Protocol/UdpClient.hpp"
#include "../Server/Protocol/UdpServer.hpp"
#include "../Server/Protocol/UdpSession.hpp"

class Game {
 public:
  Game(std::size_t t_flag);
  ~Game();

  sf::RenderWindow &getWindow();
  InputManager &getInputManager();
  void run();

 private:
  enum CommunicationFlag { server, client };
  sf::RenderWindow m_window;
  InputManager m_input_manager;
  CommunicationFlag m_flag;
  UdpClient m_clientCom;
  UdpServer m_serverCom;
  boost::thread m_thread;

  EntityManager initEntityManager();
  std::vector<std::shared_ptr<ISystem>>
    initSystems(std::shared_ptr<EntityManager>);
};

#endif  //GAME_GAME_HPP
