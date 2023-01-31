/*
** EPITECH PROJECT, 2023
** Rtype
** File description:
** SettingsState
*/

#include "../inc/SettingsState.hpp"

#include "../inc/MainState.hpp"
#include "../inc/PlayState.hpp"

SettingsState::SettingsState(StateMachine &machine, sf::RenderWindow &window,
                             const bool replace)
    : State(machine, window, replace),
      m_start_btn(Button("./assets/startBtn.png", sf::Vector2f(600, 350),
                         sf::Vector2f(270, 130))) {
  if (!m_bg_t.loadFromFile("./assets/menubg.jpg")) {
    throw std::runtime_error("Unable to load image.");
  }
  float scale_x = 1280.0 / m_bg_t.getSize().x;
  float scale_y = 720.0 / m_bg_t.getSize().y;
  m_bg_s.setTexture(m_bg_t, true);
  m_bg_s.setScale(scale_x, scale_y);
}

void SettingsState::pause() { std::cout << "MenuState Pause\n"; }

void SettingsState::resume() { std::cout << "MenuState resume\n"; }

void SettingsState::update() {
  for (auto event = sf::Event{}; m_window.pollEvent(event);) {
    sf::Vector2i mouse_pos = sf::Mouse::getPosition(m_window);
    sf::Vector2f mouse_pos_f(static_cast<float>(mouse_pos.x),
                             static_cast<float>(mouse_pos.y));
    if (event.type == sf::Event::MouseMoved) {
      m_start_btn.is_hovered(mouse_pos_f);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
      if (m_start_btn.is_pressed(mouse_pos_f)) {
        std::cout << "startbtn pressed" << std::endl;
        m_next =
            StateMachine::build<MainState>(m_state_machine, m_window, true);
      }
    }
    switch (event.type) {
      case sf::Event::Closed:
        m_state_machine.quit();
        break;
      case sf::Event::KeyPressed:
        switch (event.key.code) {
          case sf::Keyboard::Space:
            m_next =
                StateMachine::build<MainState>(m_state_machine, m_window, true);
            break;
          case sf::Keyboard::Escape:
            m_state_machine.quit();
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}

void SettingsState::draw() {
  m_window.clear();
  m_window.draw(m_bg_s);
  m_window.display();
}