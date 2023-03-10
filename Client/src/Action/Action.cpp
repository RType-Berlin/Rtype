#include "./Action.hpp"
#include <iostream>

Action::Action(ActionType type, EntityID id) : m_type(type), m_id(id) {
  m_action_id = action_counter++;
}

Action::Action(ActionType type, EntityID id, int t_action_id)
    : m_type(type), m_id(id) {
  m_action_id = t_action_id;
}

Action::~Action() {}

std::string Action::getTypeAsString() const {
  switch (m_type) {
    case ActionType::START:
      return "START";
    case ActionType::RESTART:
      return "RESTART";
    case ActionType::UP:
      return "UP";
    case ActionType::DOWN:
      return "DOWN";
    case ActionType::LEFT:
      return "LEFT";
    case ActionType::RIGHT:
      return "RIGHT";
    case ActionType::POS:
      return "POS";
    case ActionType::SHOOT:
      return "SHOOT";
    case ActionType::CREATE:
      return "CREATE";
    case ActionType::INCREASE:
      return "INCREASE";
    case ActionType::COLLISION:
      return "COLLISION";
    case ActionType::DEAD:
      return "DEAD";
    case ActionType::END:
      return "END";
    case ActionType::DESTROY:
      return "DESTROY";
    case ActionType::DAMAGE:
      return "DAMAGE";
    case ActionType::CREATELOBBY:
      return "CREATELOBBY";
    case ActionType::CREATESUCCESSFULL:
      return "CREATESUCCESSFULL";
    case ActionType::JOINLOBBY:
      return "JOINLOBBY";
    case ActionType::JOINSUCCESSFULL:
      return "JOINSUCCESSFULL";
    case ActionType::ASKLEADERBOARD:
      return "ASKLEADERBOARD";
    case ActionType::SENDLEADERBOARD:
      return "SENDLEADERBOARD";
    case ActionType::ERROR:
      return "ERROR";
  }
  return "ERROR";
}

std::string Action::getCommand() const {
  std::string type_string = getTypeAsString();
  std::string data = "";
  std::string names = "";
  std::string leaderboardline = "";
  std::string lobby_code = "";

  switch (m_type) {
    case ActionType::START:
      if (m_lobby_ip != "") { lobby_code = m_lobby_ip; }
      return std::to_string(m_action_id) + ";" + type_string + ";" +
             std::to_string(m_id) + ";" + lobby_code + ";" +
             std::to_string(m_client_id) + ";" + m_player_name + ";";
    case ActionType::DEAD:
    case ActionType::END:
      return std::to_string(m_action_id) + ";" + type_string + ";" +
             std::to_string(m_id) + ";";
    case ActionType::UP:
    case ActionType::DOWN:
    case ActionType::LEFT:
    case ActionType::RIGHT:
    case ActionType::RESTART:
      return std::to_string(m_action_id) + ";" + type_string + ";" +
             std::to_string(m_id) + ";" + std::to_string(m_triggered_by_user) +
             ";";
    case ActionType::POS:
      return std::to_string(m_action_id) + ";" + type_string + ";" +
             std::to_string(m_id) + ";" + std::to_string(m_position.x) + ";" +
             std::to_string(m_position.y) + ";";
    case ActionType::SHOOT:
      return std::to_string(m_action_id) + ";SHOOT;" + std::to_string(m_id) +
             ";" + std::to_string(m_triggered_by_user) + ";" +
             std::to_string(m_damage) + ";" + std::to_string(m_shoot_type) +
             ";";
    case ActionType::INCREASE:
      return std::to_string(m_action_id) + ";INCREASE;" + std::to_string(m_id) +
             ";" + std::to_string(m_increase_type) + ";" +
             std::to_string(m_value) + ";";
    case ActionType::DAMAGE:
      return std::to_string(m_action_id) + ";DAMAGE;" + std::to_string(m_id) +
             ";" + std::to_string(m_damage) + ";" +
             std::to_string(m_collision_partner_id) + ";";
    case ActionType::CREATE:
      data =
        std::to_string(m_position.x) + ";" + std::to_string(m_position.y) + ";";
      if (m_sprite_path.length() > 0) { data += m_sprite_path + ";"; }
      if (m_velocity >= -1) { data += std::to_string(m_velocity) + ";"; }
      if (m_item_type != 0) {
        data += std::to_string(float(m_item_type)) + ";";
      }
      if (m_client_id != 0) { data += std::to_string(m_client_id) + ";"; }
      if (m_collision_partner_id != 0) {
        data += std::to_string(m_collision_partner_id) + ";";
      }
      if (m_damage != 0) { data += std::to_string(m_damage) + ";"; }
      if (m_total_obstacle_width != 0) {
        data += std::to_string(m_total_obstacle_width) + ";";
      }
      std::cout << std::to_string(m_velocity) << std::endl;

      return std::to_string(m_action_id) + ";CREATE;" + std::to_string(m_id) +
             ";" + std::to_string(m_object_type) + ";" + data;
    case ActionType::COLLISION:
      return std::to_string(m_action_id) + ";COLLISION;" +
             std::to_string(m_id) + ";" +
             std::to_string(m_collision_partner_id) + ";";
    case ActionType::DESTROY:
      return std::to_string(m_action_id) + ";DESTROY;" + std::to_string(m_id) +
             ";";
    case ActionType::CREATELOBBY:
      return std::to_string(m_action_id) + ";CREATELOBBY;" +
             std::to_string(m_id) + ";" + m_lobby_ip + ";" + m_player_name +
             ";";
    case ActionType::LEAVELOBBY:
      return std::to_string(m_action_id) + ";LEAVELOBBY;" +
             std::to_string(m_id) + ";" + m_lobby_ip + ";" + m_player_name +
             ";";
    case ActionType::JOINLOBBY:
      return std::to_string(m_action_id) + ";JOINLOBBY;" +
             std::to_string(m_id) + ";" + m_lobby_ip + ";" + m_player_name +
             ";";
    case ActionType::JOINSUCCESSFULL:
      for (int i = 0; i < m_lobby_player_names.size(); i++) {
        names += m_lobby_player_names[i];
        names += ";";
      }
      return std::to_string(m_action_id) + ";JOINSUCCESSFULL;" +
             std::to_string(m_id) + ";" + names;
    case ActionType::CREATESUCCESSFULL:
      return std::to_string(m_action_id) + ";CREATESUCCESSFULL;" +
             std::to_string(m_id) + ";";
    case ActionType::CHAD:
      return std::to_string(m_action_id) + ";CHAD;" + std::to_string(m_id) +
             ";" + m_chad_msg + ";" + m_lobby_ip + ";";
    case ActionType::ASKLEADERBOARD:
      return std::to_string(m_action_id) + ";ASKLEADERBOARD;" +
             std::to_string(m_id) + ";";
    case ActionType::SENDLEADERBOARD:
      for (int i = 0; i < m_leaderboard.size(); i++) {
        leaderboardline += m_leaderboard[i];
        leaderboardline += ";";
      }
      return std::to_string(m_action_id) + ";SENDLEADERBOARD;" +
             std::to_string(m_id) + ";" + leaderboardline;
  }
  return std::to_string(m_action_id) + ";" + type_string + ";" +
         std::to_string(m_id) + ";";
}

int Action::getActionId() const { return m_action_id; }
Action::ActionType Action::getType() const { return m_type; }
EntityID Action::getId() const { return m_id; }

bool Action::isTriggeredByUser() const { return m_triggered_by_user; }

EntityID Action::getCollisionPartnerId() const {
  return m_collision_partner_id;
}

Action::ObjectType Action::getCreateType() const { return m_object_type; }

rtype::Vector2f Action::getCreatePosition() const { return m_position; }

std::string Action::getCreateSpritePath() const { return m_sprite_path; }

Action::IncreaseType Action::getIncreaseType() const { return m_increase_type; }

int Action::getIncreaseValue() const { return m_value; }

int Action::getShootDamage() const { return m_damage; }

Action::ShootingType Action::getShootType() const { return m_shoot_type; }

float Action::getVelocity() const { return m_velocity; }

int Action::getItemType() const { return m_item_type; }

int Action::getTotalObstacleWidth() const { return m_total_obstacle_width; }

void Action::setPlayerId(EntityID t_id) { m_id = t_id; }

int Action::getClientId() const { return m_client_id; }

void Action::setClientId(int t_client_id) { m_client_id = t_client_id; }

void Action::setLobbyId(int t_lobby_id) { m_lobby_id = t_lobby_id; }

int Action::getLobbyId() const { return m_lobby_id; }

void Action::setPlayerName(std::string t_player_name) {
  m_player_name = t_player_name;
}

std::string Action::getPlayerName() const { return m_player_name; }

void Action::setLobbyIp(std::string t_lobby_ip) { m_lobby_ip = t_lobby_ip; }

std::string Action::getLobbyIp() const { return m_lobby_ip; }

void Action::setLobbyPlayerNames(
  std::vector<std::string> t_lobby_player_names) {
  m_lobby_player_names = t_lobby_player_names;
}

std::vector<std::string> Action::getLobbyPlayerNames() const {
  return m_lobby_player_names;
}

std::string Action::getChadMsg() const { return m_chad_msg; }

void Action::setChadMsg(std::string t_chad_msg) { m_chad_msg = t_chad_msg; }

std::vector<std::string> Action::getLeaderboard() const {
  return m_leaderboard;
}
