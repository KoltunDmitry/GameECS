#pragma once
#include "SFML/Graphics.hpp"
extern sf::RenderWindow window(sf::VideoMode(1366,768),"Game", sf::Style::Resize);
extern sf::View view = window.getDefaultView();
extern int count = 0;