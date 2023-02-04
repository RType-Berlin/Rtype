#ifndef PLAYSTATE_HPP_
#define PLAYSTATE_HPP_

#include <iostream>
#include <memory>
#include <stdexcept>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>

#include "../../Game/ECS/EntityManager.hpp"
#include "../../Game/ECS/ISystem.hpp"
#include "../../Game/Systems/AnimationSystem.hpp"
#include "../../Game/Systems/CollisionSystem.hpp"
#include "../../Game/Systems/DisplaySystem.hpp"
#include "../../Game/Systems/MovementSystem.hpp"
#include "../../Game/Systems/RandomEnemyGeneratorSystem.hpp"
#include "../../Game/Systems/ShootingSystem.hpp"
#include "../../GameEngine/inc/MusicPlayer.hpp"
#include "../../GameEngine/inc/State.hpp"
#include "../../GameEngine/inc/StateMachine.hpp"
#include "../inc/MainState.hpp"
#include "../src/InputManager/InputManager.hpp"

class StateMachine;

class PlayState final : public State {
 public:
  PlayState(StateMachine &t_machine, sf::RenderWindow &t_window,
            MusicPlayer &t_music_player, bool t_replace = true);

  void pause() override;
  void resume() override;

  void update() override;
  void draw() override;

 private:
  EntityManager m_entity_manager;
  std::shared_ptr<EntityManager> m_entity_manager_ptr;
  std::vector<std::shared_ptr<ISystem>> m_systems;
  InputManager m_input_manager;
};

#endif  // !PLAYSTATE_HPP_