#include "note.hh"

#include "constants.hh"

Note::Note()
{
	rectangle_shape.setSize(
			sf::Vector2f(Constants.rectangle.size, Constants.rectangle.size));
	rectangle_shape.setOutlineThickness(Constants.rectangle.outline);
	rectangle_shape.setFillColor(Constants.rectangle.fillColor);
	rectangle_shape.setOutlineColor(Constants.rectangle.outlineColor);
	rectangle_shape.setPosition(10, 10);
}

void Note::Draw(sf::RenderWindow *window)
{
	window->draw(rectangle_shape);
}

