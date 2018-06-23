.PHONY: clean
CC = g++
CXXFLAGS = -Wall -pedantic -c
obj = obj/main.o 
SFML = -lsfml-graphics -lsfml-window -lsfml-system

maze: $(obj)
	$(CC) $(obj) -o $@ $(SFML)

obj/%.o: src/%.cpp
	$(CC) $< $(CXXFLAGS) -o $@

clean: 
	rm -rf obj/*.o maze
