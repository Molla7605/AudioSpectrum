#ifndef AUDIO_SPECTRUM_H
#define AUDIO_SPECTRUM_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <kiss_fft.h>
#include <complex>

class AudioSpectrum : public sf::Transformable, public sf::Drawable {
public:
	AudioSpectrum(int bar_count);
	~AudioSpectrum();

	void update(const sf::Time& song_pos, const sf::SoundBuffer& buffer, const sf::Time& delta_time);

	sf::FloatRect getLocalBounds();
	sf::FloatRect getGlobalBounds();

private:
	static const int scale = 2;
	static const int buffer_size = 16384 / scale;

	using Complex = std::complex<float>;

	kiss_fft_cfg config;
	Complex* m_complex;
	Complex* m_result;
	sf::VertexArray m_va;

	float* m_window;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // !AUDIO_SPECTRUM_H

