#include "note.hh"

#include "constants.hh"

Note::Note()
{
	rectangle_shape.setSize(
			sf::Vector2f(Constants.rectangle.size, Constants.rectangle.size));
	rectangle_shape.setOutlineThickness(Constants.rectangle.outline);

	line_shape.setSize(sf::Vector2f(
				Constants.line.thickness,
				Globals.windowHeight - Constants.padding*2));
	line_shape.setFillColor(Constants.line.color);

	keyPressed = false;
}

void Note::SetPosition(int x, int y)
{
	rectangle_shape.setPosition(x, y);
	line_shape.setPosition(x + Constants.rectangle.size/2, Constants.padding);
}

void Note::Draw(sf::RenderWindow *window)
{
	window->draw(line_shape);

	if (keyPressed) {
		rectangle_shape.setFillColor(Constants.rectangle.pressedFillColor);
		rectangle_shape.setOutlineColor(Constants.rectangle.pressedOutlineColor);
	} else {
		rectangle_shape.setFillColor(Constants.rectangle.fillColor);
		rectangle_shape.setOutlineColor(Constants.rectangle.outlineColor);
	}
	window->draw(rectangle_shape);
}

void Note::KeyReleased()
{

}

