#include "./RectangleShape.hpp"

void rtype::RectangleShape::setFillColor(const rtype::Color &color) {
  m_shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
}

void rtype::RectangleShape::setOutlineColor(const rtype::Color &color) {
  m_shape.setOutlineColor(sf::Color(color.r, color.g, color.b, color.a));
}

void rtype::RectangleShape::setOutlineThickness(float thickness) {
  m_shape.setOutlineThickness(thickness);
}

void rtype::RectangleShape::setSize(const rtype::Vector2f &size) {
  m_shape.setSize(sf::Vector2f(size.x, size.y));
}

void rtype::RectangleShape::setPosition(const rtype::Vector2f &position) {
  m_shape.setPosition(sf::Vector2f(position.x, position.y));
}

void rtype::RectangleShape::setTexture(const rtype::ITexture *texture) {
  rtype::ITexture *texture_cast = const_cast<rtype::ITexture *>(texture);
  m_shape.setTexture(
    dynamic_cast<rtype::Texture *>(texture_cast)->getTexture());
}

void rtype::RectangleShape::setTextureRect(const rtype::IntRect &rect) {
  m_shape.setTextureRect(
    sf::IntRect(rect.left, rect.top, rect.width, rect.height));
}

void rtype::RectangleShape::setRotation(float angle) {
  m_shape.setRotation(angle);
}

void rtype::RectangleShape::setScale(const rtype::Vector2f &factors) {
  m_shape.setScale(sf::Vector2f(factors.x, factors.y));
}

void rtype::RectangleShape::setOrigin(const rtype::Vector2f &origin) {
  m_shape.setOrigin(sf::Vector2f(origin.x, origin.y));
}

void rtype::RectangleShape::rotate(float angle) { m_shape.rotate(angle); }

void rtype::RectangleShape::scale(const rtype::Vector2f &factors) {
  m_shape.scale(sf::Vector2f(factors.x, factors.y));
}

bool rtype::RectangleShape::intersects(const rtype::FloatRect &rect) {
  return m_shape.getGlobalBounds().intersects(
    sf::FloatRect(rect.left, rect.top, rect.width, rect.height));
}

rtype::IRectangleShape::SIDE
rtype::RectangleShape::intersectsSide(const rtype::FloatRect &rect) {
  sf::FloatRect shape_rect = m_shape.getGlobalBounds();
  shape_rect.width += shape_rect.left;
  shape_rect.height += shape_rect.top;
  sf::FloatRect obstacle_rect = sf::FloatRect(
    rect.left, rect.top, rect.width + rect.left, rect.height + rect.top);
  rtype::IRectangleShape::SIDE side = rtype::IRectangleShape::SIDE::NONE;

  if (m_shape.getGlobalBounds().intersects(
        sf::FloatRect(rect.left, rect.top, rect.width, rect.height))) {
    // left, top, right, bottom
    sf::FloatRect intersection_rect = {
      std::min(shape_rect.left, obstacle_rect.left),
      std::min(shape_rect.top, obstacle_rect.top),
      std::max(shape_rect.width, obstacle_rect.width),
      std::max(shape_rect.height, obstacle_rect.height)};
    float width = intersection_rect.width - intersection_rect.left;
    float height = intersection_rect.height - intersection_rect.top;

    if (intersection_rect.left == shape_rect.left && height > width) {
      side = rtype::IRectangleShape::SIDE::LEFT;
    } else if (intersection_rect.top == shape_rect.top && width > height) {
      side = rtype::IRectangleShape::SIDE::TOP;
    } else if (intersection_rect.width == shape_rect.width && height > width) {
      side = rtype::IRectangleShape::SIDE::RIGHT;
    } else if (intersection_rect.height == shape_rect.height &&
               width > height) {
      side = rtype::IRectangleShape::SIDE::BOTTOM;
    }
  };
  return side;
}

const rtype::Vector2f &rtype::RectangleShape::getSize() {
  sf::Vector2f sfml_vector = m_shape.getSize();
  m_size = {sfml_vector.x, sfml_vector.y};
  return m_size;
}

const rtype::Vector2f &rtype::RectangleShape::getPosition() {
  sf::Vector2f sfml_vector = m_shape.getPosition();
  m_position = {sfml_vector.x, sfml_vector.y};
  return m_position;
}

const rtype::FloatRect &rtype::RectangleShape::getGlobalBounds() {
  sf::FloatRect sfml_rect = m_shape.getGlobalBounds();
  m_globalBounds = {sfml_rect.left, sfml_rect.top, sfml_rect.width,
                    sfml_rect.height};
  return m_globalBounds;
}

const rtype::IntRect &rtype::RectangleShape::getTextureRect() {
  sf::IntRect sfml_rect = m_shape.getTextureRect();
  rtype::IntRect *m_textureRect = new rtype::IntRect{
    sfml_rect.left, sfml_rect.top, sfml_rect.width, sfml_rect.height};
  return *m_textureRect;
}

float rtype::RectangleShape::getRotation() const {
  return m_shape.getRotation();
}

const rtype::Vector2f &rtype::RectangleShape::getScale() {
  sf::Vector2f sfml_vector = m_shape.getScale();
  m_scale = {sfml_vector.x, sfml_vector.y};
  return m_scale;
}

const rtype::Vector2f &rtype::RectangleShape::getOrigin() {
  sf::Vector2f sfml_vector = m_shape.getOrigin();
  m_origin = {sfml_vector.x, sfml_vector.y};
  return m_origin;
}

sf::RectangleShape &rtype::RectangleShape::getRectangleShape() {
  return m_shape;
}
