Project description:
  
  This is my implementation of the classic game snake. Some things to note about the program:

    1) At startup the user is presented with a tui, which can be navigating using the arrow
       keys along with the enter key.

    2) In the settings menu there are options for changing the screen size as well as the 
       speed that the snake travels at. The screen size will take up a set percentage of
       the size of the users terminal at start up, so the play space depends heavily on
       the dimensions of the terminal.

    3) In the game, the snake grows as it eats the 'fruit'. The game ends when the snake
       runs into a wall or itself.

Compiling && Running
  
  Included is a Makefile who's base rule will compile the program and create an executable
  called 'Snake'. There is also a command to compile and immediately run the program called
  'run'.

Files:

  Coordinate.cpp    Contains the definition of the coordinate data structure
  Coordinate.hh     Header file for Coordinate.cpp

  Game.cpp          Contains the main game loop as well as the main for the program
  Game.hh           Header file for Game.hh

  Screen.cpp        Contains the functions for drawing to screen as well as the objects
                    used to make the initial tui that the user can use to change settings
                    for the game
  Screen.hh         Header file for Screen.hh

  Snake.cpp         Contains the object model for the snake in the game, as well as the
                    logic it uses for movement
  Snake.hh          Header file for Snake.cpp

  statusCodes.hh    Contains a definition for what a return code means
