CC=g++
# Adapt the variables below to your needs:
EXE=snake
CPP=dorian/main.cpp dorian/MainSDLWindow.cpp dorian/playground.cpp dorian/snake.cpp dorian/playgroundrenderer.cpp dorian/score.cpp dorian/scorerenderer.cpp dorian/fruits.cpp
HPP=dorian/MainSDLWindow.hpp dorian/playground.hpp dorian/snake.hpp dorian/playgroundrenderer.hpp dorian/score.hpp dorian/scorerenderer.hpp dorian/fruits.hpp

# The default is build the executable file:
default: $(EXE)

# The executable file must be rebuilt if source files changed:
$(EXE): $(CPP) $(HPP)
	$(CC) $(CPP) -lSDL2 -lSDL2_ttf -o $(EXE)

# Run of the (always up-to-date) executable:
run: $(EXE)
	./$(EXE)

# Build the UML diagram as a PNG image:
uml/classes.png: $(HPP)
	./uml/mk_uml.sh $@ $^

# Count the number of lines of code:
nblines:
	@wc -l $(CPP) $(HPP)
