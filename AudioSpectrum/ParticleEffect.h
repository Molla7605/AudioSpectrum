#ifndef PARTICLE_EFFECT_H
#define PARTICLE_EFFECT_H

#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <cmath>

class ParticleEffect : public sf::Transformable, public sf::Drawable {
public:
	ParticleEffect(std::size_t particle_count, int range, int max_size, int min_size, sf::Color color);
	virtual ~ParticleEffect();

	void update(const sf::Time& delta_time, float speed_scale);

private:
	struct Particle {
		std::size_t index = 0;
		sf::Vector2f velocity = sf::Vector2f();
		sf::Time life_time = sf::Time::Zero;
		sf::Time time = sf::Time::Zero;
	};

	std::size_t m_particle_count;
	std::vector<Particle> m_p;
	sf::VertexArray m_va;

	std::uniform_int_distribution<int> m_pos_dis;
	std::uniform_int_distribution<int> m_speed_dis;
	std::uniform_int_distribution<int> m_size_dis;
	std::uniform_int_distribution<int> m_angle_dis;
	std::uniform_int_distribution<int> m_time_dis;

	std::mt19937 m_rgen;
	std::random_device m_rd;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void resetParticle(Particle& particle);
};

#endif // !PARTICLE_EFFECT_H

