#ifndef FONTLOADER_HH
#define FONTLOADER_HH

#include <SFML/Graphics.hpp>
#include "../imgui/imgui.h"

namespace FontLoader
{

bool loadEmbeddedFont(sf::Font &sfFont, ImFont *&imFont,
		std::unique_ptr<char> &uncompBuffer,
		const char *compData, unsigned int compSize);

};

#endif

