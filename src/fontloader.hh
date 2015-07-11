#ifndef FONTLOADER_HH
#define FONTLOADER_HH

#include <SFML/Graphics.hpp>

bool loadEmbeddedFont(sf::Font *font, const char *data, unsigned int size);

#endif

