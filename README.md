
Submarine Game
Overview
This program simulates the placement and search of submarines on a 10x10 game board. It uses multithreading to perform submarine searches and employs mmap for efficient log writing to a shared log file. The game ends when all submarine parts are found.

Features
Board Initialization: Generates a 10x10 game board and randomly places submarines (X) within constraints:
Submarine lengths: 2 to 4 cells.
Coverage: 10-15% of the board.
Submarines do not overlap or extend outside boundaries.
Multithreading: Five threads search for submarine parts:
X: Hit (updates to V).
.: Miss.
V: Skip (already found).
Logging:
Logs thread activity (hits, misses, skips) to both the console and a shared log file (game_log.txt).
Utilizes mmap for efficient memory-mapped file I/O.
Game Termination: Ends when all submarine parts are found, and displays the final board state.
Requirements
Compiler: GCC (GNU Compiler Collection).
Libraries: pthread, mman, unistd, fcntl.
OS: Linux or a Unix-based system (for mmap support).
Compilation
To compile the program, run the following command in your terminal:

bash
Copy code
gcc submarine_game.c -o submarine_game -lpthread
Execution
To execute the program, use:

bash
Copy code
./submarine_game
The program will:

Display the initial board setup in the console.
Run five threads to search for submarine parts.
Log all thread actions to the console and game_log.txt.
Print the final board state and a "Game over!" message.
mmap Explanation
The program uses mmap for logging thread actions. Here's how it works:

The file game_log.txt is opened with read-write permissions. If it does not exist, it is created.
ftruncate ensures the file is resized to 4 KB to match the memory mapping size.
mmap maps the file into memory for direct access, allowing threads to write log messages to the file efficiently without constant I/O system calls.
After logging, munmap unmaps the memory, and the file is closed.
Benefits of mmap:

Reduces the overhead of file I/O by eliminating intermediate buffering.
Ensures logs are immediately available in the shared file.
Log File Format
The game_log.txt file records thread actions, such as:

less
Copy code
Thread 1: Missed at [3, 5]
Thread 2: Found a part of a submarine at [6, 7]
Thread 3: Skipped at [2, 2]
Notes
Ensure the program is run in a directory where game_log.txt can be created or accessed.
The program may not work on systems without mmap support.
Enjoy the game! 🎮