#ifndef ASTAR_HH
#define ASTAR_HH
#include "mazeCreator.hpp"
#include <vector>
#include <algorithm>
#include "Graphics.hpp"
#include <unistd.h>

#define SLEEP //usleep(5000);
#define SLEEP_MORE usleep(1000000);
#define SLEEP_MORE_MORE usleep(4000000);

struct Vertex {
	int _cost;
	Point _position;
	Point _prevposition;

	Vertex(Point pos): _cost(0), _position(pos), _prevposition(22) { }
	Vertex(Point pos, const Vertex& parent): _position(pos) {
		_cost = parent._cost + 1;
		_prevposition = parent._position; 
	}
};

std::ostream& operator<<(std::ostream& s, 
						const std::vector<Point>& v) {
	s << "\n";
	for(const Point& x : v)
		s << x << " ";
	return s << "\n";
}

std::ostream& operator<<(std::ostream& s, 
						const std::vector<Vertex>& v) {
	s << "\n";
	for(const Vertex& x : v)
		s << x._position << " ";
	return s << "\n";
}

class AStar {
	static int moves;
	static std::vector<Vertex> closedlist;
	static Point start;
	static Point end;
	static Graphics *graphic;
	AStar() { }

	public:
	static int goThroughBoard(Maze& maze) {
		graphic = new Graphics(int(maze.getHeight()), maze);
		start = maze.enter();
   		end = maze.exit();
		std::vector<Vertex> openlist;	
		Vertex activePoint(start);
		addAndColor(openlist, activePoint, sf::Color::Green);
		while(!openlist.empty()) {
			removeElement(openlist, activePoint._position);
			addAndColor(closedlist, activePoint, sf::Color::Red);
			addAllMoves(openlist, maze, activePoint);
			if(openlist.empty()) {
				SLEEP_MORE
				delete graphic;
				return -1;
			}
			activePoint = getLowest(openlist);
			if(activePoint._position == end) {
				addAndColor(closedlist, activePoint, sf::Color::Red);
				SLEEP_MORE
				findPath(activePoint);
				SLEEP_MORE_MORE
				std::cout << "\nMove by: " << moves << "points\n";
				delete graphic;
				return 0;
			}
			SLEEP
		}	
		SLEEP_MORE
		delete graphic;
		return -1;
	}
	
	private:
	static void findPath(const Vertex& v) {	
		SLEEP
		graphic->setBlue(v._position);
		if(v._position == start) {
			std::cout << v._position;
			++moves;
			return;
		} else {
			findPath(getVertex(closedlist, v._prevposition));
			std::cout << "->" << v._position;
			++moves;
		}
	}	

	static void addAndColor(std::vector<Vertex>& v, 
							const Vertex& vertex,
							sf::Color color) {
		v.push_back(vertex);
		if(color == sf::Color::Red)
			graphic->setRed(vertex._position);
		else
			graphic->setGreen(vertex._position);
	}

	static void removeElement(std::vector<Vertex>& set, 
							  Point element) {
		auto it = std::find_if(set.begin(), set.end(), 
						[&element](Vertex v){
							return(v._position == element ? true : false);
		});
		if(it != set.end())
			set.erase(it);
	}
	
	static bool contain(const std::vector<Vertex>& stack, 
						Point element) {
		auto it = std::find_if(stack.begin(), stack.end(), 
						[&element](Vertex v){
							return(v._position == element ? true : false);
		});
		if(it != stack.end())
			return true;
		return false;
	}	
	
	static Vertex getLowest(const std::vector<Vertex>& openset) {
		int min = 100000000;
		Point x;
		Vertex setpos(x);
		for(const Vertex& v : openset) {
			if(f(v) < min) {
				min = f(v);
				setpos = v;
			}
		}
		return setpos;
	}
	
	static void addAllMoves(std::vector<Vertex>& openset, 
							const Maze& maze,   
							Vertex fromPosition) {
		Point pos = fromPosition._position;
		std::vector<Point> positionsToCheck;
		for(int i=pos.x-1; i<pos.x+2; i++)
			for(int j=pos.y-1; j<pos.y+2; j++) 
				if((abs(pos.x-i)+abs(pos.y-j))%2 && maze.isSpace(i,j))
					positionsToCheck.push_back(Point(i,j));
		for(const Point& p : positionsToCheck) {
			if(!contain(closedlist, p)) {
				Vertex newVertex(p, fromPosition);
				if(contain(openset, p))
					if(!swapIfBetter(openset, newVertex)) {
						continue;
					}
				//openset.push_back(newVertex);
					addAndColor(openset, newVertex, sf::Color::Green);
			}
		}	
	}

	static Vertex getVertex(const std::vector<Vertex>& stack,
				 			Point pos) {	
		auto it = std::find_if(stack.begin(), stack.end(), 
						[&pos](Vertex v){
							return(v._position == pos ?	true : false);
		});
		if(it != stack.end())
			return *it;
		return Point(-1,-1);
	}	

	static bool swapIfBetter(std::vector<Vertex>& stack, 
							Vertex v) {
		auto it = std::find_if(stack.begin(), stack.end(), 
								[&v](Vertex V){
			return (V._position == v._position ? true : false);			  
		});
		if(it != stack.end()) 
			if(it->_cost > v._cost) {
				it->_cost = v._cost;
				it->_prevposition = v._prevposition;
				return true;
			}
		return false;
	}

	static int f(const Vertex& v) {
		return g(v)+h(v);	
	}

	static int g(const Vertex& v) {
		return v._cost;
	}

	static int h(const Vertex& v) {
		return manhattanDistance(v._position, end);
	}
	
	static int manhattanDistance(const Point& pos, const Point& pos2) {
		return Point::distance(pos, pos2);
	}


};

int AStar::moves = 0;
std::vector<Vertex> AStar::closedlist;
Point AStar::end;
Point AStar::start;
Graphics* AStar::graphic;


#endif
