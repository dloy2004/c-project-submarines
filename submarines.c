#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define N 10 // Board size
#define SUBMARINE_MIN 2
#define SUBMARINE_MAX 4
#define SUBMARINE_COVERAGE_MIN 10
#define SUBMARINE_COVERAGE_MAX 15
#define THREAD_COUNT 5

typedef struct {
    char board[N][N];
    pthread_mutex_t lock;
    int remaining_parts;
} GameState;

typedef struct {
    GameState *game;
    int thread_id; // Unique ID for each thread
} ThreadData;

// Function to initialize the board
void initialize_board(GameState *game) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            game->board[i][j] = '.';
        }
    }
}

// Function to place submarines
void place_submarines(GameState *game) {
    int total_cells = N * N;
    int min_cells = (total_cells * SUBMARINE_COVERAGE_MIN) / 100;
    int max_cells = (total_cells * SUBMARINE_COVERAGE_MAX) / 100;
    int submarine_cells = 0;

    while (submarine_cells < min_cells) {
        int length = rand() % (SUBMARINE_MAX - SUBMARINE_MIN + 1) + SUBMARINE_MIN;
        int orientation = rand() % 2; // 0: Horizontal, 1: Vertical
        int row = rand() % N;
        int col = rand() % N;

        int can_place = 1;
        if (orientation == 0 && col + length <= N) { 
            for (int i = 0; i < length; i++) {
                if (game->board[row][col + i] != '.') {
                    can_place = 0;
                    break;
                }
            }
            if (can_place) {
                for (int i = 0; i < length; i++) {
                    game->board[row][col + i] = 'X';
                }
                submarine_cells += length;
            }
        } else if (orientation == 1 && row + length <= N) { 
            for (int i = 0; i < length; i++) {
                if (game->board[row + i][col] != '.') {
                    can_place = 0;
                    break;
                }
            }
            if (can_place) {
                for (int i = 0; i < length; i++) {
                    game->board[row + i][col] = 'X';
                }
                submarine_cells += length;
            }
        }
    }

    game->remaining_parts = submarine_cells;
}

// Function to print the board
void print_board(GameState *game, FILE *log_file) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%c ", game->board[i][j]);
            fprintf(log_file, "%c ", game->board[i][j]);
        }
        printf("\n");
        fprintf(log_file, "\n");
    }
    fprintf(log_file, "\n");
}

// Thread function for submarine search
void *submarine_search(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    GameState *game = data->game;
    int thread_id = data->thread_id;

    int fd = open("game_log.txt", O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("Failed to open log file");
        return NULL;
    }

    // Ensure the file is 4096 bytes in size for mmap
    if (ftruncate(fd, 4096) == -1) {
        perror("Failed to set file size");
        close(fd);
        return NULL;
    }

    char *log_mmap = mmap(NULL, 4096, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if (log_mmap == MAP_FAILED) {
        perror("Failed to mmap");
        close(fd);
        return NULL;
    }

    // Write thread logs into mmap
    int offset = 0; // Keep track of where to write in the mmap region
    srand(time(NULL) ^ pthread_self());

    while (1) {
        pthread_mutex_lock(&game->lock);

        if (game->remaining_parts == 0) {
            pthread_mutex_unlock(&game->lock);
            break;
        }

        int row = rand() % N;
        int col = rand() % N;
        char log_entry[100];

        if (game->board[row][col] == 'X') {
            game->board[row][col] = 'V';
            game->remaining_parts--;
            snprintf(log_entry, sizeof(log_entry), "Thread %d: Found a part of a submarine at [%d, %d]\n", thread_id, row, col);
        } else if (game->board[row][col] == '.') {
            snprintf(log_entry, sizeof(log_entry), "Thread %d: Missed at [%d, %d]\n", thread_id, row, col);
        } else {
            snprintf(log_entry, sizeof(log_entry), "Thread %d: Skipped at [%d, %d]\n", thread_id, row, col);
        }

        printf("%s", log_entry);
        int len = strlen(log_entry);

        if (offset + len < 4096) {
            memcpy(log_mmap + offset, log_entry, len);
            offset += len;
        } else {
            fprintf(stderr, "Log file exceeded mmap size.\n");
            break;
        }

        pthread_mutex_unlock(&game->lock);
    }

    munmap(log_mmap, 4096);
    close(fd);
    return NULL;
}


int main() {
    srand(time(NULL));

    GameState game;
    pthread_mutex_init(&game.lock, NULL);
    initialize_board(&game);
    place_submarines(&game);

    FILE *log_file = fopen("game_log.txt", "w");
    if (!log_file) {
        perror("Failed to open log file");
        return 1;
    }

    printf("Initial Board:\n");
    fprintf(log_file, "Initial Board:\n");
    print_board(&game, log_file);

    pthread_t threads[THREAD_COUNT];
    ThreadData thread_data[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].game = &game;
        thread_data[i].thread_id = i + 1; // Assign unique thread ID
        pthread_create(&threads[i], NULL, submarine_search, &thread_data[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nFinal Board:\n");
    fprintf(log_file, "\nFinal Board:\n");
    print_board(&game, log_file);

    printf("Game over!\n");
    fprintf(log_file, "Game over!\n");

    fclose(log_file);
    pthread_mutex_destroy(&game.lock);

    return 0;
}
