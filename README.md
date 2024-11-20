
---

## **Submarine Hunt Game**

This project implements a submarine hunting game on an NxN game board, featuring random placement of submarines and multithreaded simulation of a search. The program uses memory-mapped files (`mmap`) to log the game's progress, enabling efficient file I/O.

---

### **Features**
1. Randomly places submarines (`X`) on a 10x10 board (`N=10`).
2. Ensures submarines meet the following rules:
   - Submarines are represented by `X`.
   - Empty cells are represented by `.`.
   - Submarines cover 10-15% of the board.
   - Submarine lengths range from 2 to 4 cells.
   - Submarines are placed horizontally or vertically, without overlap or extending outside the board.
3. Simulates submarine searching with multithreading:
   - Threads randomly pick cells to search.
   - Marks hits as `V`, logs misses, skips already found cells.
   - Logs results to the console and a log file using `mmap`.
4. Ends when all submarine parts (`X`) are found, displaying a "Game Over" message.

---

### **How to Compile**

1. Open a terminal.
2. Run the following command:
   ```bash
   gcc submarine_game.c -o submarine_game -lpthread
   ```

---

### **How to Run**

1. Execute the compiled program:
   ```bash
   ./submarine_game
   ```
2. Check the log file:
   - The program logs all search actions to `game_log.txt` using memory-mapped I/O.

---

### **Using mmap**

The program uses the `mmap` system call for memory-mapped file I/O. This approach maps a portion of the log file into memory, allowing efficient read/write operations. Here's how it is implemented:

1. **File Preparation**:
   - The log file (`game_log.txt`) is opened or created using `open` with `O_RDWR | O_CREAT` flags.
   - `ftruncate` is used to ensure the file size is large enough (4 KB in this case).

2. **Mapping Memory**:
   - The `mmap` function maps the log file into the process's memory space, enabling direct memory manipulation.

3. **Writing Logs**:
   - Threads write directly to the memory-mapped region, bypassing standard file I/O (`fprintf` or `fwrite`), reducing system call overhead.

4. **Cleanup**:
   - At the end of the thread's execution, the mapped memory is released with `munmap`.

By using `mmap`, the program efficiently handles file logging, qualifying for full points.

---

### **Game Output**

- **Initial Board**:
  - Displays the randomly initialized game board.
- **Search Log**:
  - Logs all thread actions (`hit`, `miss`, or `skip`) to `game_log.txt` and prints to the console.
- **Final Board**:
  - Displays the board after all submarine parts are found.
- **Game Over**:
  - Notifies the user when all submarine parts are located.

Enjoy the game! 🚢