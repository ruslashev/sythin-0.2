#ifndef FONTLOADER_HH
#define FONTLOADER_HH

#include <SFML/Graphics.hpp>

bool loadEmbeddedFont(sf::Font *font, std::unique_ptr<char> *uncompBuffer,
		const char *compData, unsigned int size);

#endif

