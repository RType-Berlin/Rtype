#ifndef CLIENT_SRC_SYSTEMS_RANDOMENEMYGENERATORSYSTEM_HPP_
#define CLIENT_SRC_SYSTEMS_RANDOMENEMYGENERATORSYSTEM_HPP_

#include <memory>
#include <string>

#include "../CreateEntities/Init.hpp"
#include "../ECS/ISystem.hpp"
#include "../ECS/DataTypesECS.hpp"
#include "../../Server/Protocol/UdpServer.hpp"

class RandomEnemyGeneratorSystem : public ISystem {
 public:
  RandomEnemyGeneratorSystem(std::shared_ptr<EntityManager> t_em,
                             UdpServer *t_serverCom,
                             rtype::IGraphicLoader *t_graphic_loader);
  ~RandomEnemyGeneratorSystem();

  void update();
  void updateData(SystemData &t_data);

 private:
  std::shared_ptr<EntityManager> m_em;
  UdpServer *m_serverCom;
  rtype::IGraphicLoader *m_graphic_loader;
};

#endif  // CLIENT_SRC_SYSTEMS_RANDOMENEMYGENERATORSYSTEM_HPP_
