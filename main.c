#include "simulation.h"  // Includes the definitions and declarations from simulation.h
#include <unistd.h>      // Includes POSIX constants and functions, such as usleep.

int main() {
    init_simulation(); // Initialize the simulation environment and creatures.

    while (1) { // Infinite loop to keep the simulation running.
        draw_simulation(); // Draw the current state of the simulation.
        update_simulation(); // Update the simulation state for the next cycle.
        usleep(300000); // Pause for 300,000 microseconds (0.3 seconds) between updates.
    }

    return 0; // End of the main function.
}

