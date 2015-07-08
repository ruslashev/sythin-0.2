#ifndef TEXTURES_HH
#define TEXTURES_HH

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

namespace textures {

struct Rect {
	int x, w, h;
};
static std::vector<Rect> positions;

std::unique_ptr<sf::Texture> CreateNoteTexture(sf::Font &font);

// Rect LookupNotePosition(char letter, char accidental, int octave);

}

#endif

