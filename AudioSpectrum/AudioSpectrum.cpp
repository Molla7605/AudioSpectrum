#include "AudioSpectrum.h"

AudioSpectrum::AudioSpectrum(int bar_count) :
	sf::Transformable(), 
	sf::Drawable(), 
	m_va(sf::LineStrip, bar_count * 4), 
	m_complex(new Complex[buffer_size]),
	m_result(new Complex[buffer_size]),
	m_window(new float[buffer_size])
{
	config = kiss_fft_alloc(buffer_size, 0, NULL, NULL);
	for (int count = 0; count < bar_count; count++) {
		std::size_t index = count * 4;

		m_va[index].position = sf::Vector2f(5.0f * count, 0.0f);
		m_va[index].color = sf::Color(75, 196, 93);

		m_va[index + 1].position = sf::Vector2f(5.0f * count, 0.0f);
		m_va[index + 1].color = sf::Color(115, 239, 255);

		m_va[index + 2].position = sf::Vector2f(5.0f * count + 3.0f, 0.0f);
		m_va[index + 2].color = sf::Color(115, 239, 255);

		m_va[index + 3].position = sf::Vector2f(5.0f * count + 3.0f, 0.0f);
		m_va[index + 3].color = sf::Color(75, 196, 93);
	}

	for (int count = 0; count < buffer_size; count++) {
		m_window[count] = 0.54f - 0.46f * cos((2 * 3.14159265359f * count) / buffer_size);
	}
}

AudioSpectrum::~AudioSpectrum() {
	kiss_fft_free(config);
	delete[] m_complex;
	delete[] m_result;
	delete[] m_window;
}

float AudioSpectrum::update(const sf::Time& song_pos, const sf::SoundBuffer& buffer, const sf::Time& delta_time) {
	int offset = static_cast<int>(song_pos.asSeconds() * buffer.getSampleRate() * buffer.getChannelCount());

	const sf::Int16* samples = buffer.getSamples();
	for (int count = 0; count < buffer_size && offset + count < buffer.getSampleCount(); count++) {
		m_complex[count] = samples[count + offset] * m_window[count];
	}

	kiss_fft(config, (kiss_fft_cpx*)m_complex, (kiss_fft_cpx*)m_result);

	float avg = 0.0f;
	int av_count = 0;
	for (int count = 0; count < m_va.getVertexCount() / 4; count++) {
		float db = std::abs(m_result[count]) / (90000.0f / scale);

		if (count >= 0 && count <= 20) {
			avg += db;
			av_count++;
		}

		std::size_t index = count * 4;
		float va_y_pos = m_va[index + 1].position.y;

		float m = -va_y_pos * delta_time.asSeconds() * 5.0f;

		if (va_y_pos >= -db) m_va[index + 1].position.y = m - db;
		else if (va_y_pos > 0.0f) m_va[index + 1].position.y = 0.0f;
		else m_va[index + 1].position.y += m;

		m_va[index + 2].position.y = m_va[index + 1].position.y;
	}

	return avg / av_count;
}

sf::FloatRect AudioSpectrum::getLocalBounds() {
	return m_va.getBounds();
}

sf::FloatRect AudioSpectrum::getGlobalBounds() {
	return getTransform().transformRect(m_va.getBounds());
}

void AudioSpectrum::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	target.draw(m_va, states);
}
