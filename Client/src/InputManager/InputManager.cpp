#include "InputManager.hpp"

#include <iostream>

InputManager::InputManager() {
  //SetExitKey(KEY_CAPS_LOCK);
}

// -4 = UP ; -3 = LEFT ; -2 = DOWN ; -1 = RIGHT
void InputManager::recordInputs(const sf::Event &t_event) {
  // TODO: we need to link the events with the entity id from corresponding
  //  player
  switch (t_event.key.code) {
    case sf::Keyboard::W:
      this->m_input_queue.push_back(sf::Keyboard::W);
      printf("W");
      break;
    case sf::Keyboard::S:
      this->m_input_queue.push_back(sf::Keyboard::S);
      printf("S");
      break;
    case sf::Keyboard::D:
      this->m_input_queue.push_back(sf::Keyboard::D);
      printf("D");
      break;
    case sf::Keyboard::A:
      this->m_input_queue.push_back(sf::Keyboard::A);
      printf("a");
      break;
    case sf::Keyboard::Space:
      this->m_input_queue.push_back(sf::Keyboard::Space);
      break;
  }

  //std::this_thread::sleep_for(std::chrono::milliseconds(100)); // This should change
}

void InputManager::popInputs() {
  if (!this->m_input_queue.empty()) this->m_input_queue.clear();
}

EventQueue InputManager::getInputs() {
  std::vector<int> inputs = this->m_input_queue;
  EventQueue eventQueue(inputs);
  this->popInputs();
  return eventQueue;
}

sf::Vector2i InputManager::getMousePosition() {
  return sf::Mouse::getPosition();
}

bool InputManager::isMouseLeftClicked() {
  return sf::Mouse::isButtonPressed(sf::Mouse::Left);
  // Needs click position
}

bool InputManager::isMouseRightClicked() {
  return sf::Mouse::isButtonPressed(sf::Mouse::Right);
  // Needs click position
}