#include "./SettingsState.hpp"

void SettingsState::initSprites() {
  float size_x = m_window->getSize().x;
  float size_y = m_window->getSize().y;
  m_bg_t = m_graphic_loader->loadTexture();
  m_bg_s = m_graphic_loader->loadSprite();
  if (!m_bg_t->loadFromFile("./assets/menubg.jpg")) {
    throw std::runtime_error("Unable to load image.");
  }
  float scale_x = size_x / m_bg_t->getSize().x;
  float scale_y = size_y / m_bg_t->getSize().y;
  m_bg_s->setTexture(m_bg_t, true);
  m_bg_s->setScale({scale_x, scale_y});
}

void SettingsState::initText() {
  float size_x = m_window->getSize().x;
  float size_y = m_window->getSize().y;

  m_font = m_graphic_loader->loadFont();
  if (!m_font->loadFromFile("./assets/font/nasalization-rg.ttf")) {
    throw std::runtime_error("Unable to load font.");
  }
  m_title = m_graphic_loader->loadText();
  m_title->setFont(m_font);
  m_title->setString("SETTINGS");
  m_title->setCharacterSize(50);
  m_title->setPosition(
    {(size_x / 2) - (m_title->getLocalBounds().width / 2), 100});
  m_vol_txt = m_graphic_loader->loadText();
  m_vol_txt->setFont(m_font);
  m_vol_txt->setString("VOLUME");
  m_vol_txt->setCharacterSize(35);
  m_vol_txt->setPosition(
    {(size_x / 2) - (m_vol_txt->getLocalBounds().width / 2),
     (size_y / 2) - 50});
  m_vol_digit = m_graphic_loader->loadText();
  m_vol_digit->setFont(m_font);
  m_vol_digit->setString(
    std::to_string(static_cast<int>(m_music_player.getVolume())));
  m_vol_digit->setCharacterSize(35);
  m_vol_digit->setPosition(
    {(size_x / 2) - (m_vol_digit->getLocalBounds().width / 2),
     static_cast<float>((size_y / 2) + 14.5)});
  m_vol_txt_se = m_graphic_loader->loadText();
  m_vol_txt_se->setFont(m_font);
  m_vol_txt_se->setString("VOLUME SOUNDEFFECTS");
  m_vol_txt_se->setCharacterSize(35);
  m_vol_txt_se->setPosition(
    {(size_x / 2) - (m_vol_txt_se->getLocalBounds().width / 2),
     (size_y / 2) + 74});
  m_vol_digit_se = m_graphic_loader->loadText();
  m_vol_digit_se->setFont(m_font);
  m_vol_digit_se->setString(
    std::to_string(static_cast<int>(m_music_player.getSEVol())));
  m_vol_digit_se->setCharacterSize(35);
  m_vol_digit_se->setPosition(
    {(size_x / 2) - (m_vol_digit_se->getLocalBounds().width / 2),
     static_cast<float>((size_y / 2) + 138.5)});
}

SettingsState::SettingsState(StateMachine &t_machine,
                             rtype::IRenderWindow *t_window,
                             MusicPlayer &t_music_player, std::size_t t_flag,
                             rtype::IGraphicLoader *t_graphic_loader,
                             int *t_level, const std::string &t_path_to_sprite,
                             const bool t_replace, std::string t_ip,
                             UdpClient *t_clientCom)
    : State(t_machine, t_window, t_music_player, t_graphic_loader, t_level,
            t_path_to_sprite, t_replace, t_ip, t_clientCom),
      m_start_btn(Button(
        "./assets/icons/white/home.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 - 32),
                        static_cast<float>(m_window->getSize().y - 100)},
        rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_vol_up(Button(
        "./assets/icons/white/plus.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 - 91.5),
                        static_cast<float>(m_window->getSize().y / 2)},
        rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_vol_down(Button(
        "./assets/icons/white/minus.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 + 27.5),
                        static_cast<float>(m_window->getSize().y / 2)},
        rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_vol_up_se(Button(
        "./assets/icons/white/plus.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 - 91.5),
                        static_cast<float>(m_window->getSize().y / 2 + 124)},
        rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_vol_down_se(Button(
        "./assets/icons/white/minus.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 + 27.5),
                        static_cast<float>(m_window->getSize().y / 2 + 124)},
        rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_flag(t_flag) {
  initSprites();
  initText();
}

SettingsState::~SettingsState() {
  delete m_bg_t;
  delete m_bg_s;
}

void SettingsState::update() {
  for (auto event = rtype::Event{}; m_window->pollEvent(event);) {
    rtype::Vector2i mouse_pos = m_mouse->getMousePosition(m_window);
    rtype::Vector2f mouse_pos_f{static_cast<float>(mouse_pos.x),
                                static_cast<float>(mouse_pos.y)};
    if (event.type == rtype::EventType::MouseMoved) {
      m_start_btn.is_hovered(mouse_pos_f);
      m_vol_down.is_hovered(mouse_pos_f);
      m_vol_up.is_hovered(mouse_pos_f);
      m_vol_up_se.is_hovered(mouse_pos_f);
      m_vol_down_se.is_hovered(mouse_pos_f);
    }
    if (m_mouse->isLeftMouseButtonPressed()) {
      if (m_start_btn.is_pressed(mouse_pos_f)) {
        std::cout << "startbtn pressed" << std::endl;
        m_next = StateMachine::build<MainState>(
          m_state_machine, m_window, m_music_player, m_flag, m_graphic_loader,
          m_level, m_path_to_sprite, true, "", m_clientCom);
      }
      if (m_vol_down.is_pressed(mouse_pos_f)) {
        if (m_music_player.getVolume() > 0) {
          float tmp_vol = m_music_player.getVolume();
          m_music_player.setVolume(tmp_vol - 5);
          m_vol_digit->setString(std::to_string(static_cast<int>(tmp_vol - 5)));
          m_vol_digit->setPosition(
            {(m_window->getSize().x / 2) -
               (m_vol_digit->getLocalBounds().width / 2),
             static_cast<float>(
               (static_cast<float>(m_window->getSize().y) / 2) + 14.5)});
          std::ofstream file_set("./assets/files/settings.txt");
          file_set << "vol:" + std::to_string(static_cast<int>(tmp_vol - 5))
                   << ":SE:"
                   << std::to_string(
                        static_cast<int>(m_music_player.getSEVol()));
          file_set.close();
        }
      }
      if (m_vol_up.is_pressed(mouse_pos_f)) {
        if (m_music_player.getVolume() < 100) {
          float tmp_vol = m_music_player.getVolume();
          m_music_player.setVolume(tmp_vol + 5);
          m_vol_digit->setString(std::to_string(static_cast<int>(tmp_vol + 5)));
          m_vol_digit->setPosition(
            {(m_window->getSize().x / 2) -
               (m_vol_digit->getLocalBounds().width / 2),
             static_cast<float>(
               (static_cast<float>(m_window->getSize().y) / 2) + 14.5)});
          std::ofstream file_set("./assets/files/settings.txt");
          file_set << "vol:" + std::to_string(static_cast<int>(tmp_vol + 5))
                   << ":SE:"
                   << std::to_string(
                        static_cast<int>(m_music_player.getSEVol()));
          file_set.close();
        }
      }
      if (m_vol_down_se.is_pressed(mouse_pos_f)) {
        if (m_music_player.getSEVol() > 0) {
          float tmp_vol_se = m_music_player.getSEVol();
          m_music_player.setSEVol(tmp_vol_se - 5);
          m_vol_digit_se->setString(
            std::to_string(static_cast<int>(tmp_vol_se - 5)));
          m_vol_digit_se->setPosition(
            {(m_window->getSize().x / 2) -
               (m_vol_digit_se->getLocalBounds().width / 2),
             static_cast<float>(
               (static_cast<float>(m_window->getSize().y) / 2) + 138.5)});
          std::ofstream file_set("./assets/files/settings.txt");
          file_set << "vol:" + std::to_string(
                                 static_cast<int>(m_music_player.getVolume()))
                   << ":SE:"
                   << std::to_string(static_cast<int>(tmp_vol_se - 5));
          file_set.close();
        }
      }
      if (m_vol_up_se.is_pressed(mouse_pos_f)) {
        if (m_music_player.getSEVol() < 100) {
          float tmp_vol_se = m_music_player.getSEVol();
          m_music_player.setSEVol(tmp_vol_se + 5);
          m_vol_digit_se->setString(
            std::to_string(static_cast<int>(tmp_vol_se + 5)));
          m_vol_digit_se->setPosition(
            {(m_window->getSize().x / 2) -
               (m_vol_digit_se->getLocalBounds().width / 2),
             static_cast<float>(
               (static_cast<float>(m_window->getSize().y) / 2) + 138.5)});
          std::ofstream file_set("./assets/files/settings.txt");
          file_set << "vol:" + std::to_string(
                                 static_cast<int>(m_music_player.getVolume()))
                   << ":SE:"
                   << std::to_string(static_cast<int>(tmp_vol_se + 5));
          file_set.close();
        }
      }
    }
    switch (event.type) {
      case rtype::EventType::Closed:
        m_state_machine.quit();
        break;
      case rtype::EventType::KeyPressed:
        switch (event.key) {
          case rtype::EventKey::Space:
            m_next = StateMachine::build<MainState>(
              m_state_machine, m_window, m_music_player, m_flag,
              m_graphic_loader, m_level, m_path_to_sprite, true, "",
              m_clientCom);
            break;
          case rtype::EventKey::Escape:
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
  m_window->clear();
  m_window->draw(m_bg_s);
  m_window->draw(m_title);
  m_window->draw(m_vol_txt);
  m_window->draw(m_vol_digit);
  m_window->draw(m_vol_txt_se);
  m_window->draw(m_vol_digit_se);
  m_window->draw(m_start_btn.getSprite());
  m_window->draw(m_vol_down.getSprite());
  m_window->draw(m_vol_up.getSprite());
  m_window->draw(m_vol_down_se.getSprite());
  m_window->draw(m_vol_up_se.getSprite());
  m_window->display();
}