#ifndef NOTE_HH
#define NOTE_HH

#include <SFML/Graphics.hpp>

class Note
{
	sf::RectangleShape rectangle_shape;
public:
	Note();
	void Draw(sf::RenderWindow *window);
};

#endif

