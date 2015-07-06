#include "note.hh"

#include "constants.hh"
#include "conv.hh"

Note::Note()
{
	rectangleShape.setSize(sf::Vector2f(
				Constants.rectangle.size, Constants.rectangle.size));
	rectangleShape.setOutlineThickness(Constants.rectangle.outline);

	lineShape.setSize(sf::Vector2f(
				Constants.line.thickness,
				Globals.windowHeight - Constants.padding*2));
	lineShape.setFillColor(Constants.line.color);
	keyPressed = false;
}

void Note::SetPosition(int x, int y)
{
	rectangleShape.setPosition(x, y);
	lineShape.setPosition(x + Constants.rectangle.size/2, Constants.padding);
}

void Note::SetHue(int h)
{
	int baseSat = Constants.rectangle.baseSaturation,
		baseVal = Constants.rectangle.baseValue,
		outlineVal = Constants.rectangle.outlineValue,
		pressedSat = Constants.rectangle.pressedSaturation;
	baseFillColor = HSVtoRGB(h, baseSat, baseVal);
	baseOutlineColor = HSVtoRGB(h, baseSat, outlineVal);
	pressedFillColor = HSVtoRGB(h, pressedSat, baseVal);
	pressedOutlineColor = HSVtoRGB(h, pressedSat, outlineVal);
}

void Note::SetFrequency(double frequency)
{
	baseFrequency = frequency;

	sf::Int16 playSamples[Constants.samplesPerSecond];
	double x = 0;
	for (int i = 0; i < Constants.samplesPerSecond; i++) {
		playSamples[i] = Globals.volume * sin(M_PI*2*x);
		x += baseFrequency/Constants.samplesPerSecond;
	}

	if (!playSoundBuffer.loadFromSamples(playSamples,
				Constants.samplesPerSecond,
				Constants.channels,
				Constants.samplesPerSecond))
		puts("failed to copy sound buffer");

	playSound.setBuffer(playSoundBuffer);
	playSound.setLoop(true);

}

void Note::Draw(sf::RenderWindow *window)
{
	window->draw(lineShape);

	if (keyPressed) {
		rectangleShape.setFillColor(pressedFillColor);
		rectangleShape.setOutlineColor(pressedOutlineColor);
	} else {
		rectangleShape.setFillColor(baseFillColor);
		rectangleShape.setOutlineColor(baseOutlineColor);
	}
	window->draw(rectangleShape);
}

void Note::Update()
{
}

void Note::KeyPressed()
{
	playSound.play();
}

void Note::KeyReleased()
{
	playSound.stop();
}

