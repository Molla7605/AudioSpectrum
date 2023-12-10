#include "ParticleEffect.h"

ParticleEffect::ParticleEffect(std::size_t particle_count, int range, int max_size, int min_size, sf::Color color) :
	sf::Transformable(),
	sf::Drawable(), 
	m_rd(), 
	m_rgen(m_rd()),
	m_particle_count(particle_count),
	m_p(m_particle_count),
	m_va(sf::Quads, m_particle_count * 4),
	m_size_dis(min_size, max_size),
	m_speed_dis(10000, 100000),
	m_angle_dis(-5 - 90, 5 - 90),
	m_pos_dis(0, range * 1000),
	m_time_dis(2000, 5000) 
{
	for (std::size_t index = 0; index < m_particle_count; index++) {
		m_p[index].index = index * 4;

		m_va[m_p[index].index].color = color;
		m_va[m_p[index].index + 1].color = color;
		m_va[m_p[index].index + 2].color = color;
		m_va[m_p[index].index + 3].color = color;

		resetParticle(m_p[index]);
	}
}
ParticleEffect::~ParticleEffect() {}

void ParticleEffect::update(const sf::Time& delta_time, float speed_scale){
	for (auto& it : m_p) {
		it.time -= delta_time;
		if (it.time <= sf::Time::Zero) resetParticle(it);

		for (int count = 0; count < 4; count++) {
			m_va[it.index + count].position += it.velocity * delta_time.asSeconds() * (speed_scale >= 1.0f ? speed_scale * 2.0f : 1.0f);

			float a = (it.time.asSeconds() / it.life_time.asSeconds()) * 255;
			m_va[it.index + count].color.a = static_cast<sf::Uint8>(a);
		}
	}
}

void ParticleEffect::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	states.transform *= getTransform();

	target.draw(m_va, states);
}

void ParticleEffect::resetParticle(Particle& particle){
	static const float pi = 3.14159265359f;

	particle.life_time = sf::seconds(static_cast<float>(m_time_dis(m_rgen)) / 1000.0f);
	particle.time = particle.life_time;
	float angle = static_cast<float>(m_angle_dis(m_rgen));
	float r = static_cast<float>(m_speed_dis(m_rgen)) / 1000.0f;

	particle.velocity.x = cos(pi / 180.0f * angle) * r;
	particle.velocity.y = (sin(pi / 180.0f * angle) * r);

	sf::Vector2f pos(static_cast<float>(m_pos_dis(m_rgen)) / 1000.0f, 0.0f);
	float size = static_cast<float>(m_size_dis(m_rgen));

	m_va[particle.index].position = pos;
	m_va[particle.index + 1].position = sf::Vector2f(pos.x + size, 0.0f);
	m_va[particle.index + 2].position = sf::Vector2f(pos.x + size, size);
	m_va[particle.index + 3].position = sf::Vector2f(pos.x, size);
}
