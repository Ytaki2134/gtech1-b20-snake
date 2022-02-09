# gtech1-b20-snake

This game is a version of Snake, developped by Dorian Gehant and Tristan Durand with the C++ langage, in a Linux environment


# Compilation

To compile the game, you will need to install the following libraries :
- libsdl2-dev, libsdl2-ttf-dev, make

for exemple, in a Debian distribution, you can achieve that by running the following command :
- sudo apt-get install libsdl2-dev libsdl2-ttf-dev make

When you have them installed, you can then move to the directory of the game and type `make`


# Playing

To start playing, make sure you are in the directory of the game and type `./snake`.

Once you are in game, you can control the direction in which the snake is moving using the directional arrows keys. 
Your goal is to get as much points as possible without hitting the borders of the game or the body of your snake. 
To get points faster, you can eat the red candy to grow bigger, but be carefull since it will make surviving harder !
There is also a green candy which makes your snake shrink, a yellow one that makes your snake move faster and one with spirals that inverts your controls.