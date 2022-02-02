#include "Vertex.h"
#include "Model.h"
#include "Polygon.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "Matrix.h"

#include <SFML/Graphics.hpp>


void clear_image(sf::Image& image) {
	auto px0 = const_cast<sf::Uint8*>(image.getPixelsPtr());
	std::fill(px0, px0 + image.getSize().x * image.getSize().y * 4, 0x00u);
}

int main() {
	const int WINDOW_HEIGHT = 720;
	const int WINDOW_WIDTH = 1280;
	const float AUTOROTATE_SPEED = 0.05f;
	bool AUTOROTATE = false;
	bool SMOOTH = false;
	bool WIREFRAME = false;

	sf::ContextSettings window_settings;
	window_settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Viewer", sf::Style::Default, window_settings);
	sf::Image canvas;
	sf::Texture texture;
	sf::Sprite sprite;

	canvas.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black);
	texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(0.f, 0.f));

	bool move = false;
	bool rotate = false;
	bool modified = true;
	sf::Vector2i last_mouse_pos;
	sf::Vector2i mouse_delta; 
	int wheel_delta;

	sf::Event e;

	Model model;
	if (!model.load("./models/mill.obj")) throw std::exception("Can not load model!");

	model.fit(window);
	// model.draw_wireframe(canvas);

	while (window.isOpen()) {
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) window.close();
			else if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Escape) window.close();
				else if (e.key.code == sf::Keyboard::R) {
					model.reset();
					model.fit(window);
					modified = true;
				}
				else if (e.key.code == sf::Keyboard::A) {
					AUTOROTATE = !AUTOROTATE;
					std::cout << "autorotate " << ((AUTOROTATE) ? "on" : "off") << std::endl;
				}
				else if (e.key.code == sf::Keyboard::S) {
					SMOOTH = !SMOOTH;
					std::cout << "smooth " << ((SMOOTH) ? "on" : "off") << std::endl;
				}
				else if (e.key.code == sf::Keyboard::M) {
					WIREFRAME = !WIREFRAME;
					std::cout << ((WIREFRAME) ? "wireframe" : "polygone") << " mode" << std::endl;
				}
			}
			else if (e.type == sf::Event::MouseButtonPressed) {
				if (e.mouseButton.button == sf::Mouse::Left) {
					last_mouse_pos = sf::Mouse::getPosition();
					std::cout << "move: on" << std::endl;
					move = true;
				}
				else if (e.mouseButton.button == sf::Mouse::Right) {
					last_mouse_pos = sf::Mouse::getPosition();
					std::cout << "rotate: on" << std::endl;
					rotate = true;
				}
			}
			else if (e.type == sf::Event::MouseButtonReleased) {
				if (e.mouseButton.button == sf::Mouse::Left){
					std::cout << "move: off" << std::endl;
					move = false;
				}
				else if (e.mouseButton.button == sf::Mouse::Right) {
					std::cout << "rotate: off" << std::endl;
					rotate = false;
				}
			}
			else if (e.type == sf::Event::MouseWheelMoved) {
				wheel_delta = e.mouseWheel.delta;
				if (wheel_delta) {
					model.scale(1 + 0.07 * wheel_delta);
					modified = true;
				}
			}
		}

		if (move || rotate) {
			mouse_delta = last_mouse_pos - sf::Mouse::getPosition();
			last_mouse_pos = sf::Mouse::getPosition();
			if (mouse_delta != sf::Vector2i(0, 0)) {
				modified = true;
				//std::cout << mouse_delta.x << " " << mouse_delta.y << std::endl;
				if (move)
					model.move(-mouse_delta.x / 10.f, mouse_delta.y / 10.f, 0.f);
				if (rotate)
					model.rotate(0.f, mouse_delta.x, 0.f);
					//model.rotate(mouse_delta.y, mouse_delta.x, 0.f);
			}
		}

		if (WIREFRAME) {
			clear_image(canvas);
			model.draw_wireframe(canvas);
			modified = false;
			
			modified = false;
		}

		if (AUTOROTATE)
			model.rotate(0.f, AUTOROTATE_SPEED, 0.f);

		window.clear();

		if (WIREFRAME) {
			texture.update(canvas);
			window.draw(sprite);
		}
		else
			model.draw_poly(window, SMOOTH);
		
		window.display();
	}
}