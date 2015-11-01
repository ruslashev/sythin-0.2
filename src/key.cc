#include "key.hh"

#include "constants.hh"
#include "conv.hh"
#include "note_atlas.hh"

Key::Key()
{
	rectangleShape.setSize(sf::Vector2f(
				Constants.rectangle.size, Constants.rectangle.size));
	rectangleShape.setOutlineThickness(Constants.rectangle.outline);

	// lineShape.setSize(sf::Vector2f(
	// 			Constants.line.thickness,
	// 			Globals.windowHeight - Constants.padding*2));
	// lineShape.setFillColor(Constants.line.color);
	keyPressed = false;
}

void Key::CreateSprites()
{
	sf::IntRect rect = note_atlas::LookupNotePosition(note.letter,
				note.accidental, note.octave);
	textSprite.setSize(sf::Vector2f(rect.width, rect.height));
	textSprite.setTextureRect(rect);
	textSprite.setOutlineThickness(Constants.text.outline);
	textSprite.setOutlineColor(conv::HSVtoRGB(0, 0, Constants.text.backgroundValue));
	textSprite.setPosition(
			rectangleShape.getPosition().x +
			rectangleShape.getLocalBounds().width/2 -
			textSprite.getLocalBounds().width/2 +
			Constants.text.outline,
			rectangleShape.getPosition().y +
			rectangleShape.getLocalBounds().height/2 -
			textSprite.getLocalBounds().height/2 +
			Constants.text.outline);
}

void Key::SetPosition(int x, int y)
{
	rectangleShape.setPosition(x, y);
	// lineShape.setPosition(x + Constants.rectangle.size/2, Constants.padding);
}

void Key::SetHue(int h)
{
	int saturation = Constants.rectangle.saturation;
	int pressedSaturation = Constants.rectangle.pressedSaturation;
	int value = Constants.rectangle.value;
	int outlineValue = Constants.rectangle.outlineValue;

	color               = conv::HSVtoRGB(h, saturation,        value);
	outlineColor        = conv::HSVtoRGB(h, saturation,        outlineValue);
	pressedColor        = conv::HSVtoRGB(h, pressedSaturation, value);
	pressedOutlineColor = conv::HSVtoRGB(h, pressedSaturation, outlineValue);
}

void Key::SetTexture(sf::Texture *texture)
{
	textSprite.setTexture(texture);
}

void Key::Draw(sf::RenderWindow *window)
{
	// window->draw(lineShape);

	if (keyPressed) {
		rectangleShape.setFillColor(pressedColor);
		rectangleShape.setOutlineColor(pressedOutlineColor);
	} else {
		rectangleShape.setFillColor(color);
		rectangleShape.setOutlineColor(outlineColor);
	}

	window->draw(rectangleShape);

	window->draw(textSprite);
}

void Key::KeyPressed()
{
	note.sound.setLoop(true);
	note.sound.play();
}

void Key::KeyReleased()
{
	note.sound.setLoop(false);
}

