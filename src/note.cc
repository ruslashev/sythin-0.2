#include "note.hh"

#include "constants.hh"

Note::Note()
{
	rectangleShape.setSize(sf::Vector2f(
				Constants.rectangle.size, Constants.rectangle.size));
	rectangleShape.setOutlineThickness(Constants.rectangle.outline);

	lineShape.setSize(sf::Vector2f(
				Constants.line.thickness,
				Globals.windowHeight - Constants.padding*2));
	lineShape.setFillColor(Constants.line.color);

	sf::Int16 playSamples[Constants.samplesPerSecond];
	double x = 0;
	for (int i = 0; i < Constants.samplesPerSecond; i++) {
		playSamples[i] = Globals.volume * sin(M_PI*2*x);
		x += 440./Constants.samplesPerSecond;
	}

	if (!playSoundBuffer.loadFromSamples(playSamples,
				Constants.samplesPerSecond,
				Constants.channels,
				Constants.samplesPerSecond))
		puts("failed to copy sound buffer");

	playSound.setBuffer(playSoundBuffer);
	playSound.setLoop(true);

	keyPressed = false;
}

void Note::SetPosition(int x, int y)
{
	rectangleShape.setPosition(x, y);
	lineShape.setPosition(x + Constants.rectangle.size/2, Constants.padding);
}

void Note::Draw(sf::RenderWindow *window)
{
	window->draw(lineShape);

	if (keyPressed) {
		rectangleShape.setFillColor(Constants.rectangle.pressedFillColor);
		rectangleShape.setOutlineColor(Constants.rectangle.pressedOutlineColor);
	} else {
		rectangleShape.setFillColor(Constants.rectangle.fillColor);
		rectangleShape.setOutlineColor(Constants.rectangle.outlineColor);
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

