**Introduction:**\
This game consists of a randomly generated maze, wherein the player (represented by a yellow/green sprite) competes against the computer (represented by a red sprite) to collect a higher number of coins. When the game ends, the player with the most coins is declared the winner. An equal number of coins would result in a tie.\
\
**Combat System:**\
The original creator has implemented a shortest path algorithm, using which the red sprite follows the green sprite throughout the maze. The player can manoeuvre their sprite using the four arrow keys. Both players can collect swords and health points throughout the maze, incrementing their weaponry and health by 1 and 5, respectively. When they come into contact, they engage in combat, wherein both lose a sword and X health points, where X = 5 * the opponent's original number of swords. The game culminates when either character is eliminated (health points <= 0).\
\
**How to run the game:**\
This game's operation requires SFML and an integrated development environment (IDE). Kindly watch video #46 of the following Udemy course to install and configure the required apparatus on Windows: https://www.udemy.com/course/learn-c-game-development/. Copy and paste the contents of main.cpp, and don't forget to move the rest of the files in the project directory.\
\
P.S. You can customise the game's look by replacing the font file (arial.ttf) - make sure to update the font name in main.cpp! You can also replace the .png files with other images of the same name and resolution.\
\
**Credits:**\
https://github.com/cowster36 (Open-source) : SFML implementation, random maze generation, shortest path algorithm, single-round gameplay\
https://github.com/parhlesaadu : multi-round combat system involving swords and health points, scoreboard, AVL tree for storing player statistics
