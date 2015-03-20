
                             Rock 'Em Soccer Robots
                          Stephen Sanders & Jake Singer

Welcome to the Rock 'Em Soccer Robots source repository! While the game
currently only runs in Windows, the directory and source structure has been
designed to allow ease of portability in the future. As Visual Studio likes to
think it has full control of the file system, we use symbolic links as an
abstraction to our directory layout. This means that before you can do anything,
config.bat must be run. This file sets up directory junctions in the
proj\visualstudio\{engine, game, support} locations, as well as compiles all
support libraries required to compile the game itself.


PUBLISHING:
If you wish to simply play the game:
	1 - Run config.bat
	2 - Run utility\publish.bat
	3 - Run the game from "archive\bin\Rock Em' Soccer Robots.exe"


COMPILING:
In order to compile the game from within Visual Studio:
	1 - Run config.bat
	2 - Open proj\game_vs11.sln
	3 - Build (and Run)!


OBJECTIVE:
To win the game, you must score 5 points against the opposing team. In order to
accomplish this, you can run into the ball, boost (into the ball or across the
field), kick the ball into the air, or punch opposing players (into the ball
or across the field). As long as the ball enters the opposing team's goal, you
earn a point. This means that it is possible to win without ever touching the
ball (though it may be hard)!


CONTROLS:

	Keyboard defaults as Blue with Red being the first controller plugged
	in. If two controllers are plugged in, the second controller controls
	Blue instead of the keyboard.

          -----------------------------------------------------------
                    | KEYBOARD                  | CONTROLLER
          ===========================================================
           Move     | Arrow Keys                | Left Analog Stick
           Jump     | Spacebar                  | A
           Boost    | Shift                     | X
           Kick Up  | Z                         | Y
           Punch    | X                         | B
           Pause    | Esc                       | Start
          -----------------------------------------------------------


FILES AND FOLDERS:
 | archive/                             generated content by publish.bat
 | bin/                                 compiled files
 | data/                                game data files
 | engine/                              engine source and internal headers
 | include/                             public headers
 | lib/                                 compiled libraries
 + proj/                                project files
 |-+ visualstudio/                      visual studio support - not for use
 | | game_vs11.sln                      open this for game and engine stuff
 | + support_vs11.sln                   use this to compile other libraries
 | src/                                 game source
 | utility/                             scripts to automate tasks
 |-+ clean.bat                          remove windows config and compile files
 | + publish.bat                        create a presentable game folder
 | config.bat                           sets up compile environment on windows
 + README                               you are here
