#ifndef NOTE_HH
#define NOTE_HH

#include <SFML/Graphics.hpp>

class Note
{
	sf::RectangleShape rectangle_shape;
	sf::RectangleShape line_shape;
public:
	Note();
	void SetPosition(int x, int y);
	void Draw(sf::RenderWindow *window);
};

#endif

