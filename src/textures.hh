#ifndef TEXTURES_HH
#define TEXTURES_HH

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <map>

namespace textures {

static std::map<std::string, sf::IntRect> positions;

std::unique_ptr<sf::Texture> CreateNoteTexture(sf::Font &font);

sf::IntRect LookupNotePosition(char letter, char accidental, int octave);

}

#endif

