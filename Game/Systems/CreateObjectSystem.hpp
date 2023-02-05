#ifndef R_TYPE_CLIENT_CREATEOBJECTSYSTEM_HPP
#define R_TYPE_CLIENT_CREATEOBJECTSYSTEM_HPP

#include <memory>
#include <string>

#include "../ECS/ISystem.hpp"
#include "./AnimationSystem.hpp"

class CreateObjectSystem : public ISystem {
 public:
  CreateObjectSystem(std::shared_ptr<EntityManager> t_em);
  ~CreateObjectSystem();

  void update();
  void updateData(SystemData &t_data);

 private:
  std::shared_ptr<EntityManager> m_em;
  EventQueue m_event_queue;
  void createPlayer(EntityID t_id, std::string t_sprite_path,
                    sf::Vector2f t_pos);
  void createBullet(EntityID t_id, sf::Vector2f t_pos);
  void createEnemy(EntityID t_id, sf::Vector2f t_pos, float velocity);
};

#endif  //R_TYPE_CLIENT_CREATEOBJECTSYSTEM_HPP
