# Hard Pong

This small project is a pong game, with added difficulties.

## Getting Started

Really simple, just dl everything and run the .exe in bin/. 
There are several compiled files; by default is the hardcore one. For the tests, the others are coded with a sleep(X) for the file pongCompiledX, to make it easier by slowing down the ball.
If you want to modify the code, go for .c files

## Particularities

There are several ones, which basically add some strange behaviour to the ball. The ball can switch wall for example, change direction (weak probability), stop for a few miliseconds and so one.
The scores are coded in a way that a modification of the storing file would reinitialize them. It's possible to crack it with the help of the code itself.

## Built With

* [Simple DirectMedia Layer](https://www.libsdl.org/)
* [Code::Blocks](http://www.codeblocks.org/)

## Authors

* **Benoit Audigier** - *Initial work* - [BenoitAudigier](https://github.com/BenoitAudigier)

## Acknowledgments

* Program to learn how to C inspired by [OpenClassrooms' C tutorial](https://openclassrooms.com/courses/apprenez-a-programmer-en-c)
