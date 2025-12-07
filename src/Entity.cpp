#include "Entity.h"

void Entity::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Entity::setPosition(float x, float y) {
    shape.setPosition(x, y);
}

sf::Vector2f Entity::getPosition() const {
    return shape.getPosition();
}

void Entity::move(const sf::Vector2f& offset) {
    shape.move(offset);
}

sf::FloatRect Entity::getGlobalBounds() const {
    return shape.getGlobalBounds();
}