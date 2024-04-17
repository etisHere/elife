# eLife Simulation

## Overview
eLife is a terminal-based simulation showcasing a simplified ecosystem of "emans," symbolic representations of life forms that interact within a confined environment. The simulation explores concepts of life, grouping behavior, predation, and reproduction through simple rules governing the actions of each eman.

## Features
- **Dynamic Environment:** A grid where emans move, interact, and evolve over time.
- **Diverse Life Forms:** Emans represented by characters (`A-Z`, `a-z`), numbers (`0-9`), and symbols (`!-)`), each with unique behaviors.
- **Grouping and Duplication:** Emans form groups and can duplicate, simulating social behavior and reproduction.
- **Predation:** A chance-based mechanism where emans can "eat" each other to gain more life, with different probabilities for character vs. number emans.
- **Adaptive Lifespans:** Emans have lifespans that vary between 10 to 30 minutes, introducing natural selection dynamics.
- **Continuous Evolution:** The simulation evolves, with emans duplicating under certain conditions, and numeric/symbol/Cap(`A-Z`) emans showing a preference for eating.
- **Gui:** A Gui to change the emans life to your liking

## Running the Simulation
Change line 81 if gnome-terminal is not installed or just install it. 

GTK(Install this on your os):
`libgtk-3-dev`

Run the compile executable:

`chmod +x compile.sh`
`./compile.sh`

Run the gui:

`./gui`

## Understanding eLife
eLife serves as an educational tool, demonstrating basic principles of ecology, evolution, and behavior in a highly abstracted form. It allows observers to contemplate the emergence of complex behaviors from simple rules and the interplay between different forms of life within a shared environment.

