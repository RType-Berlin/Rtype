#ifndef R_TYPE_CLIENT_POWERUPSYSTEM_HPP
#define R_TYPE_CLIENT_POWERUPSYSTEM_HPP

#include <memory>

#include "../ECS/ISystem.hpp"
#include "./SoundSystem.hpp"

class PowerUpSystem : public ISystem {
 public:
  PowerUpSystem(std::shared_ptr<EntityManager> t_em,
                std::vector<SoundSystem::SoundType> &t_sounds,
                rtype::IGraphicLoader *t_graphic_loader);
  ~PowerUpSystem() = default;

  void update() override;
  void updateData(SystemData &t_data) override;

 private:
  EventQueue m_event_queue;
  std::vector<SoundSystem::SoundType> &m_play_sounds;
  rtype::IGraphicLoader *m_graphic_loader;

  void increaseHealth(std::shared_ptr<Action> action, Player *t_player);
};

#endif  //R_TYPE_CLIENT_POWERUPSYSTEM_HPP
