#ifndef NOTE_HH
#define NOTE_HH

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Note
{
	sf::RectangleShape rectangleShape;
	sf::RectangleShape lineShape;
	sf::Color baseFillColor, baseOutlineColor,
		pressedFillColor, pressedOutlineColor;

	sf::SoundBuffer playSoundBuffer;
	sf::Sound playSound;
public:
	sf::Keyboard::Key key;
	double baseFrequency;
	bool keyPressed;

	Note();
	void SetPosition(int x, int y);
	void SetHue(int h);
	void SetFrequency(double frequency);
	void Draw(sf::RenderWindow *window);
	void Update();

	void KeyPressed();
	void KeyReleased();
};

#endif

