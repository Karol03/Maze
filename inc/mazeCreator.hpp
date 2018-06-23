#ifndef MAKZEMAZE_HH
#define MAKZEMAZE_HH
#include <iostream>
#include <set>
#include <random>
#include <iterator>
#include <cstdlib>
#include <fstream>
#include <cmath>


typedef unsigned int u_int;

enum class BlockType { BLOCKED, ENTER, SPACE, WALL, EXIT };

enum class Direction { UNSET, N, E, S, W };

struct Point {
	int x;
	int y;
	
	Point(int x = 0, int y = 0): x(x), y(y) {	}

	operator bool() {
		return ( x!=-1 && y!=-1 ? true : false );
	}

	static int distance(const Point& p1, const Point& p2) {
		return (abs(p1.x-p2.x)+abs(p1.y-p2.y));	
	}

	bool operator==(const Point& p) const {
		return (x==p.x && y==p.y ? true : false );
	}
	
	bool operator!=(const Point& p) const {
		return (x!=p.x || y!=p.y ? true : false );
	}
};

std::ostream& operator<<(std::ostream& str, const Point& p) {
	return std::cout << "["<< p.x << "," << p.y <<"]";
}	

struct Maze {
	private:
	BlockType** area;
	const u_int width;
	const u_int height;
	Point start;
   	Point end;

	public:
	Maze(u_int w, u_int h): width(w), height(h) {
		area = new BlockType*[width];
		for(u_int i=0; i<width; i++) {
			area[i] = new BlockType[height];
			for(u_int j=0; j<height; j++)
				if(i==0 || j==0 || i==width-1 || j==height-1)
					area[i][j] = BlockType::BLOCKED;
				else
					area[i][j] = BlockType::WALL;
		}
	}

	~Maze() {
		for(u_int i=0; i<width; i++) {
			delete area[i];
		}
		delete [] area;
	}

	Point enter() const {
		return start;
	}
	
	Point exit() const {
		return end;
	}

	Point getSpace(int i, int j) const {
		get(i,j);
		if(area[i][j] == BlockType::SPACE)
			return Point(i,j);
		return Point(-1, -1);	
	}

	Point get(int i, int j) const {
		if(inRange(Point(i,j)))
			return Point(i,j);
		return Point(-1, -1);
	}

	void saveToFile() {
		std::ofstream fs;
		fs.open("Maze.bmp");
		if(!fs.is_open())
			throw 101;
		fs << "P1\n" << width << " " << height << "\n";	
		for(u_int i=0; i<width; i++) {
			for(u_int j=0; j<height; j++) {
				switch(area[i][j]) {
					case BlockType::ENTER:
						fs << '0';
						break;
					case BlockType::EXIT:
						fs << '0';
						break;
					case BlockType::SPACE:
						fs << '0';
						break;
					default:
						fs << '1';
						break;
				}
				fs << ' ';
			}
			fs << "\n";
		}
		fs.close();
	}
	void print() const {
		std::cout << "\n";
		for(u_int i=0; i<width; i++) {
			for(u_int j=0; j<height; j++) {
				switch(area[i][j]) {
					case BlockType::WALL:
						std::cout << 'w';
						break;
					case BlockType::ENTER:
						std::cout << 'e';
						break;
					case BlockType::EXIT:
						std::cout << 'q';
						break;
					case BlockType::SPACE:
						std::cout << 'o';
						break;
					case BlockType::BLOCKED:
						std::cout << '-';
						break;
				}
			}
			std::cout << "\n";
		}
	}

	u_int getWidth() const {
		return width;
	}
	
	u_int getHeight() const {
		return height;
	}

	bool inRange(const Point& p) const {
		return (p.x>=0 && p.x<int(width) && p.y>=0 && p.y<int(height) ?
				true : false);
	}

	void changeBlock(const Point& p, BlockType block = BlockType::SPACE) {
		if(inRange(p))
			area[p.x][p.y] = block;
	}	

	Point range() const {
		return Point(width, height);
	}

	bool isSpace(const int& x, const int& y) const {
		if(x>=0 && x<int(width) && y>=0 && y<int(height))
			return (area[x][y] == BlockType::SPACE ? true : false);
		return false;
	}
	
	bool isFree(const Point& p) const {
		return isFree(p.x, p.y);
	}

	bool isFree(const int& x, const int& y) const {
		if(x>=0 && x<int(width) && y>=0 && y<int(height))
			return (area[x][y] == BlockType::WALL ? true : false);
		return false;
	}
	
	void makeEnters() {
		std::mt19937 range;
    	range.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> result(0,
															width-1);
		int res = result(range);
		if(!findLowThan(res, start))
			findMoreThan(res, start);
		
		res = result(range);
		if(!findLowThan(res, end, height-2))
			findMoreThan(res, end, height-2);
		
	}

	bool findLowThan(int range, Point& p, int j = 1) {
		int next = (j == 1 ? j-1 : j+1);
		while(range < int(width)-1) {
			if(area[range][j] == BlockType::SPACE) {
				area[range][next] = BlockType::SPACE; 
				area[range][j] = BlockType::SPACE; 
				p.x = range;
				p.y = next;
				return true;
			}
			++range;
		}
		return false;
	}
	
	bool findMoreThan(int range, Point& p, int j = 1) {
		int next = (j == 1 ? j-1 : j+1);
		while(range > 0) {
			if(area[range][j] == BlockType::SPACE) {
				area[range][next] = BlockType::SPACE; 
				area[range][j] = BlockType::SPACE; 
				p.x = range;
				p.y = next;
				return true;
			}
			--range;
		}
		return false;
	}

	void block(const Point& p) {
		int x = p.x;
		int y = p.y;
		for(int i = x-1; i<x+2; i++)
			for(int j = y-1; j<y+2; j++) {
				if(i<0 || i>=int(width) || j<0 || j>=int(height)) 
					continue;
				if(i==p.x && j==p.y)
					continue;		
				if(hasNeighbours(i,j)>=2 && isFree(i,j)) 
					changeBlock(Point(i,j), BlockType::BLOCKED);
			}
	}

	int hasNeighbours(int x, int y, bool trig = false) const {
		bool SouthNorthNeig = false;
		bool EastWestNeig = false;
		int counter = 0;
		Point p(x,y);
		if(isNeighbourExist(p, Direction::N)) { 
				SouthNorthNeig = true; ++counter; }
		if(isNeighbourExist(p, Direction::S)) {
				SouthNorthNeig = true; ++counter; }
		if(isNeighbourExist(p, Direction::E)) {
				EastWestNeig = true; ++counter; }
		if(isNeighbourExist(p, Direction::W)) {
				EastWestNeig = true; ++counter; }
		if(trig && (SouthNorthNeig || EastWestNeig))
			return 1;
		return counter;
	}	

	bool isNeighbourExist(Point p, Direction d) const {
		Point sPoint(p.x, p.y);
		switch(d) {
			case Direction::N:
			++sPoint.y;
			break;
			case Direction::S:
			--sPoint.y;
			break;
			case Direction::E:
			++sPoint.x;
			break;
			case Direction::W:
			--sPoint.x;
			break;
			default:
			break;
		}
		if(!inRange(sPoint))
			return false;	
		if(area[sPoint.x][sPoint.y] == BlockType::SPACE)
			return true;
		return false;
	}

};


class HaK {
	
	static Point LastPoint;	
	HaK() {}
	public:

	static void initialize(const Maze& maze) {
		Point range = maze.range();
		int x = range.x;
		int y = range.y;
		std::mt19937 rng;
    	rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> resultX(0,
															x-1);
		std::uniform_int_distribution<std::mt19937::result_type> resultY(0,
															y-1);
		LastPoint.x = resultX(rng);
		LastPoint.y = resultY(rng);
	}
		
	static void create(Maze& maze, bool print = true) {
		initialize(maze);
		do{
			do{
				if(print) {
					std::system("clear");
					maze.print();
				}
			}while(step(maze));	
		}while(hunt(maze));
		maze.makeEnters();
	}	


	private:
	static bool step(Maze& maze) {
		static Direction dir = Direction::UNSET;	
		if(!changeDirection(dir, maze))
			return false;	
		
		walk(maze, dir);
		maze.block(LastPoint);
		return true;
	}

	static bool changeDirection(Direction& dir, const Maze& maze) {
		std::set<Direction> DirSet;
		getPossibleDirections(DirSet, maze);
		if(DirSet.empty())
			return false;
		else
			randomDir(dir, DirSet);
		return true;
	}

	static void randomDir(Direction& dir, std::set<Direction>& Set) {
		std::mt19937 range;
    	range.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> result(0,
															Set.size()-1);
		auto it = Set.begin();
		int x = result(range);
	   	std::advance(it, x);
		dir = *it;
	}

	static void getPossibleDirections(std::set<Direction>& Set,
									  const Maze& maze) {
		Point newPoint = LastPoint;
		++newPoint.x;
		if(maze.inRange(newPoint) && maze.isFree(newPoint))
			Set.insert(Direction::E);
		
		newPoint.x -= 2;
		if(maze.inRange(newPoint) && maze.isFree(newPoint))
			Set.insert(Direction::W);
		
		++newPoint.x; ++newPoint.y;
		if(maze.inRange(newPoint) && maze.isFree(newPoint))
			Set.insert(Direction::N);
		
		newPoint.y -= 2;
		if(maze.inRange(newPoint) && maze.isFree(newPoint))
			Set.insert(Direction::S);

	}

	static void walk(Maze& maze, Direction dir = Direction::UNSET) {
		Point newPoint = LastPoint;
		switch(dir) {
			case Direction::N:
				++newPoint.y;
			   	break;	
			case Direction::S:
				--newPoint.y;
			   	break;	
			case Direction::E:
				++newPoint.x;
			   	break;	
			case Direction::W:
				--newPoint.x;
			   	break;	
			default:
				break;
		}
		maze.changeBlock(newPoint);
		LastPoint = newPoint;
	}

	static Point hunt(Maze& maze) {
		Point range = maze.range();
		LastPoint = Point(-1,-1);
		for(int i=0; i<range.x; i++)
			for(int j=0; j<range.y; j++)
				if(maze.isFree(i,j) && maze.hasNeighbours(i, j, true)) {
					LastPoint = Point(i,j);
					walk(maze);
					maze.block(LastPoint);
					return LastPoint;
				}
		return LastPoint;
	}

};

Point HaK::LastPoint = Point(-1, -1);	

#endif
