#!/bin/bash

g++ -o snake -g dorian/main.cpp dorian/MainSDLWindow.cpp dorian/playground.cpp dorian/playgroundrenderer.cpp \
    dorian/score.cpp dorian/scorerenderer.cpp dorian/snake.cpp dorian/fruits.cpp -lSDL2 -lSDL2_ttf