#include <iostream>
#include "AudioSpectrum.h"

int main() {
	sf::SoundBuffer buffer;
	while (true) {
		std::string path;
		std::cout << "재생할 파일 :";
		std::cin >> path;

		if (buffer.loadFromFile(path))
			break;
		else {
			std::cout << "Error :파일이 존재하지 않거나 열 수 없습니다." << std::endl;
			system("PAUSE");
		}
		
		system("cls");
	}

	system("cls");
	std::cout << "스페이스바를 눌러 재생하세요." << std::endl;

	sf::RenderWindow window(sf::VideoMode(1280U, 720U), "AudioSpectrum", sf::Style::Default);
	window.setVerticalSyncEnabled(true);

	sf::Sound sound(buffer);
	sound.setVolume(50.0f);
	sound.setLoop(true);

	AudioSpectrum st(256);
	st.setPosition(0.0f, 700.0f);

	st.setScale(1280.0f / st.getLocalBounds().getSize().x, 1.0f);

	sf::Clock tick_clock;
	while (window.isOpen()) {
		sf::Time delta_time = tick_clock.restart();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scancode::Space)
				sound.play();
		}

		st.update(sound.getPlayingOffset(), buffer, delta_time);

		window.clear();
		window.draw(st);
		window.display();
	}

	return 0;
}