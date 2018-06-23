#include "../inc/mazeCreator.hpp"
#include "../inc/Astar.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
	int n;
	if(argc == 2)
		n = std::atoi(argv[1]);
	else {
		std::cout << "Enter maze size: " << "\n";	
		std::cin >> n;
	}
	Maze maze(n, n);
	std::cout << "Creating maze..\n";
	HaK::create(maze, false);
	maze.saveToFile();
	std::cout << "Saving to file\n";
	std::cout << "Solving..\n";
	std::cout << "Start point: " << maze.enter() 
			<< "\tend point: " << maze.exit() << "\n";
	if(!AStar::goThroughBoard(maze))		
		std::cout << "\nSolved!\n";
	else
		std::cout << "Cannot solve maze\n";
	std::cout << "EXIT\n";
}
