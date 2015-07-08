#include "conv.hh"
#include "constants.hh"

namespace conv {

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

double NoteNameToFreq(conv::Name name, int octave)
{
	// here "difference" is "measured" from A4
	int octaveDifference = octave - 4;
	int semitoneDifference = name - conv::A;

	double frequency = Constants.stdTuning *
		pow(2, octaveDifference) *
		pow(2, semitoneDifference / 12.0);

	return frequency;
}

}

