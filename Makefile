CC=g++
# Adapt the variables below to your needs:
EXE=snake
CPP= main.cpp MainSDLWindow.cpp playground.cpp snake.cpp playgroundrenderer.cpp score.cpp scorerenderer.cpp fruits.cpp
HPP= MainSDLWindow.hpp playground.hpp snake.hpp playgroundrenderer.hpp score.hpp scorerenderer.hpp fruits.hpp

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
