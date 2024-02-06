#ifndef SIMULATION_H
#define SIMULATION_H

extern int WIDTH;
extern int HEIGHT;
#define NUM_CREATURES 100
#define MIN_LIFESPAN 18000
#define MAX_LIFESPAN 54000
#define GROUP_DISTANCE 3
#define GROUP_TIME 200
#define DEATH_COUNT_INIT 0

typedef struct {
    int x, y;
    char symbol;
    int isAlive;
    int age;
    int lifespan;
    int groupTimer;
    int type;
} Creature;

void init_simulation();
void update_simulation();
void draw_simulation();

extern int deathCount;

#endif // SIMULATION_H
