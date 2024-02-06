#include "simulation.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int WIDTH = 80;
int HEIGHT = 20;
Creature creatures[NUM_CREATURES * 3];
int deathCount = DEATH_COUNT_INIT;

bool isPositionOccupied(int x, int y) {
    for (int i = 0; i < NUM_CREATURES * 3; i++) {
        if (creatures[i].isAlive && creatures[i].x == x && creatures[i].y == y) {
            return true;
        }
    }
    return false;
}

void init_simulation() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    WIDTH = w.ws_col;
    HEIGHT = w.ws_row - 1;

    srand(time(NULL));
    for (int i = 0; i < NUM_CREATURES; i++) {
        do {
            creatures[i].x = rand() % WIDTH;
            creatures[i].y = rand() % HEIGHT;
        } while (isPositionOccupied(creatures[i].x, creatures[i].y));

        int symbolType = rand() % 4;
        switch (symbolType) {
            case 0:
                creatures[i].symbol = 'a' + (rand() % 26);
                creatures[i].type = 0;
                break;
            case 1:
                creatures[i].symbol = '0' + (rand() % 10);
                creatures[i].type = 1;
                creatures[i].lifespan = MIN_LIFESPAN / 2 + rand() % ((MIN_LIFESPAN / 2) - 1);
                break;
            case 2:
                creatures[i].symbol = 'A' + (rand() % 26);
                creatures[i].type = 2;
                break;
            case 3:
                creatures[i].symbol = '@';
                creatures[i].type = 3;
                break;
        }
        creatures[i].isAlive = 1;
        creatures[i].age = 0;
        creatures[i].groupTimer = 0;
        if (creatures[i].type != 1) {
            creatures[i].lifespan = MIN_LIFESPAN + rand() % (MAX_LIFESPAN - MIN_LIFESPAN);
        }
    }
}

void duplicateEman(int index, int duplicateCount) {
    while (duplicateCount-- > 0) {
        bool duplicated = false;
        for (int j = 0; j < NUM_CREATURES * 3; j++) {
            if (!creatures[j].isAlive) {
                creatures[j] = creatures[index];
                creatures[j].age = 0;
                creatures[j].groupTimer = 0;
                creatures[j].lifespan = MIN_LIFESPAN + rand() % (MAX_LIFESPAN - MIN_LIFESPAN);
                creatures[j].x = (creatures[index].x + (rand() % 3 - 1) + WIDTH) % WIDTH;
                creatures[j].y = (creatures[index].y + (rand() % 3 - 1) + HEIGHT) % HEIGHT;
                creatures[j].isAlive = 1;
                duplicated = true;
                break;
            }
        }
        if (!duplicated) break; // If unable to duplicate due to lack of space, break out of the loop
    }
}


void update_simulation() {
    for (int i = 0; i < NUM_CREATURES * 3; i++) {
        if (!creatures[i].isAlive) continue;

        creatures[i].age++;
        if (creatures[i].age > creatures[i].lifespan) {
            creatures[i].isAlive = 0;
            deathCount++;
            continue;
        }

        int moveX = rand() % 3 - 1;
        int moveY = rand() % 3 - 1;
        int newX = creatures[i].x + moveX;
        int newY = creatures[i].y + moveY;

        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
            if (!isPositionOccupied(newX, newY)) {
                creatures[i].x = newX;
                creatures[i].y = newY;
            } else {
                for (int j = 0; j < NUM_CREATURES * 3; j++) {
                    if (i != j && creatures[j].isAlive && creatures[j].x == newX && creatures[j].y == newY) {
                        bool eatCondition = (creatures[i].type != creatures[j].type && (rand() % 100 < 40));
                        if (eatCondition) {
                            creatures[j].isAlive = 0;
                            deathCount++;
                            int duplicates = (creatures[i].type == 2 || creatures[i].type == 3) ? 3 : 1;
                            duplicateEman(i, duplicates);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void draw_simulation() {
    system("clear");
    char grid[HEIGHT][WIDTH + 1];
    int aliveCount = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            grid[y][x] = ' ';
        }
        grid[y][WIDTH] = '\0';
    }

    for (int i = 0; i < NUM_CREATURES * 3; i++) {
        if (creatures[i].isAlive) {
            grid[creatures[i].y][creatures[i].x] = creatures[i].symbol;
            aliveCount++;
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        printf("%s\n", grid[y]);
    }
    printf("Death Count: %d\n", deathCount);
}

