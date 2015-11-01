#ifndef KEY_HH
#define KEY_HH

#include "conv.hh"
#include "note.hh"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Key
{
	sf::RectangleShape rectangleShape;
	// sf::RectangleShape lineShape;
	sf::Color
		color, outlineColor,
		pressedColor, pressedOutlineColor;
	sf::RectangleShape textSprite;

public:
	sf::Keyboard::Key key;
	bool keyPressed;

	Note note;

	Key();
	void CreateSprites();
	void SetPosition(int x, int y);
	void SetHue(int h);
	void SetTexture(sf::Texture *texture);
	void Draw(sf::RenderWindow *window);

	void KeyPressed();
	void KeyReleased();
};

#endif

