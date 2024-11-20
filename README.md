```
# Submarine Game

---

## **Overview**
This program simulates the placement and search of submarines on a 10x10 game board. It uses multithreading to perform submarine searches and employs `mmap` for efficient log writing to a shared log file. The game ends when all submarine parts are found.

---

## **Features**
- **Board Initialization**: Generates a 10x10 game board and randomly places submarines (`X`) within constraints:
  - Submarine lengths: 2 to 4 cells.
  - Coverage: 10-15% of the board.
  - Submarines do not overlap or extend outside boundaries.
- **Multithreading**: Five threads search for submarine parts:
  - `X`: Hit (updates to `V`).
  - `.`: Miss.
  - `V`: Skip (already found).
- **Logging**:
  - Logs thread activity (hits, misses, skips) to both the console and a shared log file (`game_log.txt`).
  - Utilizes `mmap` for efficient memory-mapped file I/O.
- **Game Termination**: Ends when all submarine parts are found, and displays the final board state.

---

## **Requirements**
- **Compiler**: GCC (GNU Compiler Collection).
- **Libraries**: `pthread`, `mman`, `unistd`, `fcntl`.
- **OS**: Linux or a Unix-based system (for `mmap` support).

---

## **Compilation**
To compile the program, use the following command in your terminal:

```bash
gcc submarine_game.c -o submarine_game -lpthread
```

---

## **Execution**
Run the program with:

```bash
./submarine_game
```

### **Program Flow**
1. Displays the initial board setup in the console.
2. Starts five threads to search for submarine parts.
3. Logs thread actions to the console and the log file (`game_log.txt`).
4. Prints the final board state and displays "Game over!" upon completion.

---

## **`mmap` Implementation**
The program employs `mmap` for efficient logging. Here's how:

1. Opens or creates the file `game_log.txt` with read-write permissions.
2. Ensures the file size is 4 KB using `ftruncate`.
3. Maps the file into memory using `mmap`, enabling direct memory access for threads to write log entries efficiently.
4. Unmaps the memory (`munmap`) and closes the file upon thread completion.

### **Advantages of `mmap`**
- Reduces file I/O overhead by bypassing intermediate buffering.
- Allows threads to write logs directly to shared memory.

---

## **Log File Format**
The `game_log.txt` file contains entries in the following format:

```
Thread 1: Missed at [3, 5]
Thread 2: Found a part of a submarine at [6, 7]
Thread 3: Skipped at [2, 2]
```

---

## **Important Notes**
- Ensure the program runs in a directory where it can create or access `game_log.txt`.
- The program relies on `mmap`, which may not work on systems without support for memory-mapped files.

Enjoy the game! 🎮
```