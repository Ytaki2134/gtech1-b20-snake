#!/bin/bash

g++ -g -o snake main.cpp MainSDLWindow.cpp playground.cpp playgroundrenderer.cpp \
    score.cpp scorerenderer.cpp snake.cpp fruits.cpp -lSDL2 -lSDL2_ttf 
