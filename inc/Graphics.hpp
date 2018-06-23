#ifndef GRAPH_HH
#define GRAPH_HH
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "mazeCreator.hpp"

#define MAX_SIZE 1000.0

class Graphics {
	sf::RenderWindow* window;
	sf::RectangleShape* rectangles;
	int size;
	

	public:
		Graphics(float s, const Maze& maze): size(int(s)) {

			window = new sf::RenderWindow(
							sf::VideoMode(MAX_SIZE,MAX_SIZE,32),
							"Maze solving!");
			window->clear();
			rectangles = new sf::RectangleShape[size*size];
			float w = MAX_SIZE/size;
			for(int i=0; i<size*size; i++) {
				rectangles[i] = sf::RectangleShape(sf::Vector2f(w,w));
				
				int x = i%size;
				int y = i/size;	
				if(maze.isSpace(x,y))
					rectangles[i].setFillColor(sf::Color::White);
				else
					rectangles[i].setFillColor(sf::Color::Black);
				rectangles[i].setPosition(w*float(x), w*float(y));
				window->draw(rectangles[i]);
			}
			window->display();
		}

		~Graphics() {
			window->close();
			delete [] rectangles;
			delete window;	
		}

		void setGreen(int x, int y) {
			setGreen(Point(x,y));
		}

		void setRed(int x, int y) {
			setRed(Point(x,y));	
		}
		
		void setGreen(const Point& p) {
			int i = p.x+p.y*size;
			rectangles[i].setFillColor(sf::Color::Green);
			drawall();
			show();
		}
		
		void setBlue(const Point& p) {
			int i = p.x+p.y*size;
			rectangles[i].setFillColor(sf::Color::Blue);
			drawall();
			show();
		}

		void setRed(const Point& p) {
			int i = p.x+p.y*size;
			rectangles[i].setFillColor(sf::Color::Red);
			drawall();
			show();
		}
		
		void show() {
			sf::Event event;
			window->display();
			while(window->pollEvent(event)) {
				switch(event.type) {
					case sf::Event::Closed:
						window->close();
					break;
					default:
					break;
				}
			}
		}

		void drawall() {
			for(int i=0; i<size*size; i++)
				window->draw(rectangles[i]);
		}
};

#endif
