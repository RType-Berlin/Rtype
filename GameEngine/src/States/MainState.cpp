#include "./MainState.hpp"

std::vector<std::string> initLeaderboard() {
  std::vector<std::string> lines;
  std::string filename = ".leaderboard.txt";
  std::ifstream infile(filename);

  if (infile) {
    std::string line;
    while (std::getline(infile, line)) {
      lines.push_back(line);
    }
    infile.close();
  } else {
    lines = {"No Score", "No Score", "No Score", "No Score", "No Score",
             "No Score", "No Score", "No Score", "No Score", "No Score"};
  }
  return lines;
}

void MainState::initSprites() {
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

void MainState::initText() {
  float size_x = m_window->getSize().x;
  float size_y = m_window->getSize().y;

  m_font = m_graphic_loader->loadFont();
  if (!m_font->loadFromFile("./assets/font/nasalization-rg.ttf")) {
    throw std::runtime_error("Unable to load font.");
  }
  m_title = m_graphic_loader->loadText();
  m_title->setFont(m_font);
  m_title->setString("R-TYPE");
  m_title->setCharacterSize(50);
  m_title->setPosition(
    {(size_x / 2) - (m_title->getLocalBounds().width / 2), 100});
}

MainState::MainState(StateMachine &t_machine, rtype::IRenderWindow *t_window,
                     MusicPlayer &t_music_player, std::size_t t_flag,
                     rtype::IGraphicLoader *t_graphic_loader, int *t_level,
                     const std::string &t_path_to_sprite, const bool t_replace,
                     std::string t_ip, UdpClient *t_clientCom)
    : State(t_machine, t_window, t_music_player, t_graphic_loader, t_level,
            t_path_to_sprite, t_replace, t_ip, t_clientCom),
      m_start_btn(Button(
        "./assets/scoreBtn.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 - 65),
                        static_cast<float>(m_window->getSize().y / 2 + 150)},
        rtype::Vector2f{130, 50}, t_graphic_loader, false)),
      m_settings_btn(
        Button("./assets/icons/white/gear.png",
               rtype::Vector2f{static_cast<float>(m_window->getSize().x - 100),
                               static_cast<float>(m_window->getSize().y - 100)},
               rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_exit_btn(
        Button("./assets/icons/white/exit.png",
               rtype::Vector2f{static_cast<float>(m_window->getSize().x - 200),
                               static_cast<float>(m_window->getSize().y - 100)},
               rtype::Vector2f{64, 64}, t_graphic_loader, true)),
      m_create_btn(Button(
        "./assets/createlobbybtn.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 - 65),
                        static_cast<float>(m_window->getSize().y / 2 - 100)},
        rtype::Vector2f{130, 50}, t_graphic_loader, false)),
      m_join_btn(Button(
        "./assets/joinlobbybtn.png",
        rtype::Vector2f{static_cast<float>(m_window->getSize().x / 2 - 65),
                        static_cast<float>(m_window->getSize().y / 2)},
        rtype::Vector2f{130, 50}, t_graphic_loader, false)),

      m_flag(t_flag) {
  initSprites();
  initText();
  m_start_pressed = false;
  m_join_pressed = false;
  m_clientCom->m_lobby_names.clear();
}

MainState::~MainState() {
  delete m_bg_t;
  delete m_bg_s;
}

void MainState::update() {
  for (auto event = rtype::Event{}; m_window->pollEvent(event);) {
    rtype::Vector2i mouse_pos = m_mouse->getMousePosition(m_window);
    rtype::Vector2f mouse_pos_f{static_cast<float>(mouse_pos.x),
                                static_cast<float>(mouse_pos.y)};
    if (event.type == rtype::EventType::MouseMoved) {
      m_start_btn.is_hovered(mouse_pos_f);
      m_settings_btn.is_hovered(mouse_pos_f);
      m_exit_btn.is_hovered(mouse_pos_f);
      m_create_btn.is_hovered(mouse_pos_f);
      m_join_btn.is_hovered(mouse_pos_f);
    }
    if (m_mouse->isLeftMouseButtonPressed()) {
      if (m_start_btn.is_pressed(mouse_pos_f) && m_start_pressed == false) {
        std::cout << "scorebtn pressed" << std::endl;
        m_clientCom->setLeaderboard(initLeaderboard());
        m_next = StateMachine::build<LeaderboardState>(
          m_state_machine, m_window, m_music_player, m_flag, m_graphic_loader,
          m_level, m_path_to_sprite, false, m_ip, m_clientCom);
      }
      if (m_settings_btn.is_pressed(mouse_pos_f)) {
        std::cout << "settingsbtn pressed" << std::endl;
        m_next = StateMachine::build<SettingsState>(
          m_state_machine, m_window, m_music_player, m_flag, m_graphic_loader,
          m_level, m_path_to_sprite, true, "", m_clientCom);
      }
      if (m_exit_btn.is_pressed(mouse_pos_f)) {
        std::cout << "exitbtn pressed" << std::endl;
        m_state_machine.quit();
      }
      if (m_create_btn.is_pressed(mouse_pos_f)) {
        std::string lobby_code = createLobbyCode();
        CreateLobbyAction create_lobby_action =
          CreateLobbyAction(Action::ActionType::CREATELOBBY, lobby_code,
                            m_clientCom->getPlayerName());
        m_clientCom->sendMessage(create_lobby_action.getCommand());
        m_clientCom->m_lobby_code = lobby_code;
        m_clientCom->m_lobby_names.push_back(m_clientCom->getPlayerName());
        std::cout << "create lobby" << std::endl;
        m_next = StateMachine::build<CreateLobbyState>(
          m_state_machine, m_window, m_music_player, m_flag, m_graphic_loader,
          m_level, m_path_to_sprite, true, "", m_clientCom);
      }
      if (m_join_btn.is_pressed(mouse_pos_f) && !m_join_pressed) {
        std::cout << "join lobby" << std::endl;
        m_next = StateMachine::build<JoinLobbyState>(
          m_state_machine, m_window, m_music_player, m_flag, m_graphic_loader,
          m_level, m_path_to_sprite, true, "", m_clientCom);
      }
    }
    switch (event.type) {
      case rtype::EventType::Closed:
        m_state_machine.quit();
        break;
      default:
        break;
    }
  }
}

void MainState::draw() {
  m_window->clear();
  m_window->draw(m_bg_s);
  m_window->draw(m_title);
  m_window->draw(m_create_btn.getSprite());
  m_window->draw(m_join_btn.getSprite());
  m_window->draw(m_start_btn.getSprite());
  m_window->draw(m_settings_btn.getSprite());
  m_window->draw(m_exit_btn.getSprite());
  m_window->display();
}

std::string MainState::createLobbyCode() {
  auto randchar = []() -> char {
    const char charset[] = "0123456789"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
  };
  std::string str(10, 0);
  std::generate_n(str.begin(), 10, randchar);
  std::cout << "Lobby Code: " << str << std::endl;
  return str;
}