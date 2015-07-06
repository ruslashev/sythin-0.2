#include "note.hh"

#include "constants.hh"

sf::Color HSVtoRGB(int h_abs, int s_abs, int v_abs)
{
	double h = h_abs / 360.0;
	double s = s_abs / 100.0;
	double v = v_abs / 100.0;
	int i = int(h * 6);
	double f = h * 6 - i;
	double p = v * (1 - s);
	double q = v * (1 - f * s);
	double t = v * (1 - (1 - f) * s);

	double r, g, b;
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}

	sf::Color out;
	out.r = r * 255;
	out.g = g * 255;
	out.b = b * 255;
	return out;
}

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

