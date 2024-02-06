#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    GtkWidget *num_creatures_entry;
    GtkWidget *min_lifespan_entry;
    GtkWidget *max_lifespan_entry;
    GtkWidget *group_distance_entry;
    GtkWidget *group_time_entry;
    char *original_content;
} AppWidgets;

AppWidgets app_widgets;

const char *default_values[] = {
    "100",    // NUM_CREATURES
    "18000",  // MIN_LIFESPAN
    "54000",  // MAX_LIFESPAN
    "3",      // GROUP_DISTANCE
    "200"     // GROUP_TIME
};

static void update_entry_widgets_with_values(const char **values) {
    gtk_entry_set_text(GTK_ENTRY(app_widgets.num_creatures_entry), values[0]);
    gtk_entry_set_text(GTK_ENTRY(app_widgets.min_lifespan_entry), values[1]);
    gtk_entry_set_text(GTK_ENTRY(app_widgets.max_lifespan_entry), values[2]);
    gtk_entry_set_text(GTK_ENTRY(app_widgets.group_distance_entry), values[3]);
    gtk_entry_set_text(GTK_ENTRY(app_widgets.group_time_entry), values[4]);
}

static void load_current_values() {
    FILE *file = fopen("simulation.h", "r");
    if (!file) {
        perror("Failed to open simulation.h for reading");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    app_widgets.original_content = malloc(fsize + 1);
    fread(app_widgets.original_content, 1, fsize, file);
    fclose(file);

    app_widgets.original_content[fsize] = 0;
    update_entry_widgets_with_values(default_values);
}

static void save_changes_to_file(GtkWidget *widget, gpointer data) {
    FILE *file = fopen("simulation.h", "w");
    if (!file) {
        perror("Failed to open simulation.h for writing");
        return;
    }

    fprintf(file, "#ifndef SIMULATION_H\n#define SIMULATION_H\n\nextern int WIDTH;\nextern int HEIGHT;\n");
    fprintf(file, "#define NUM_CREATURES %s\n", gtk_entry_get_text(GTK_ENTRY(app_widgets.num_creatures_entry)));
    fprintf(file, "#define MIN_LIFESPAN %s\n", gtk_entry_get_text(GTK_ENTRY(app_widgets.min_lifespan_entry)));
    fprintf(file, "#define MAX_LIFESPAN %s\n", gtk_entry_get_text(GTK_ENTRY(app_widgets.max_lifespan_entry)));
    fprintf(file, "#define GROUP_DISTANCE %s\n", gtk_entry_get_text(GTK_ENTRY(app_widgets.group_distance_entry)));
    fprintf(file, "#define GROUP_TIME %s\n", gtk_entry_get_text(GTK_ENTRY(app_widgets.group_time_entry)));
    fprintf(file, "#define DEATH_COUNT_INIT 0\n\n");
    fprintf(file, "typedef struct {\n    int x, y;\n    char symbol;\n    int isAlive;\n    int age;\n    int lifespan;\n    int groupTimer;\n    int type;\n} Creature;\n\n");
    fprintf(file, "void init_simulation();\nvoid update_simulation();\nvoid draw_simulation();\n\nextern int deathCount;\n\n#endif // SIMULATION_H\n");

    fclose(file);
}

static void reset_values(GtkWidget *widget, gpointer data) {
    update_entry_widgets_with_values(default_values);
}

static void revert_changes(GtkWidget *widget, gpointer data) {
    reset_values(widget, data);
}

static void run_program(GtkWidget *widget, gpointer data) {
    system("gnome-terminal -- bash -c 'gcc -o program_name main.c simulation.c utils.c && ./program_name; exec bash'");
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *button_save, *button_reset, *button_revert, *button_run, *grid;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simulation Configurator");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    app_widgets.num_creatures_entry = gtk_entry_new();
    app_widgets.min_lifespan_entry = gtk_entry_new();
    app_widgets.max_lifespan_entry = gtk_entry_new();
    app_widgets.group_distance_entry = gtk_entry_new();
    app_widgets.group_time_entry = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("NUM_CREATURES:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_widgets.num_creatures_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("MIN_LIFESPAN:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_widgets.min_lifespan_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("MAX_LIFESPAN:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_widgets.max_lifespan_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("GROUP_DISTANCE:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_widgets.group_distance_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("GROUP_TIME:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), app_widgets.group_time_entry, 1, 4, 1, 1);

    button_save = gtk_button_new_with_label("Save");
    g_signal_connect(button_save, "clicked", G_CALLBACK(save_changes_to_file), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_save, 0, 5, 2, 1);

    button_reset = gtk_button_new_with_label("Reset");
    g_signal_connect(button_reset, "clicked", G_CALLBACK(reset_values), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_reset, 0, 6, 1, 1);

    button_revert = gtk_button_new_with_label("Revert");
    g_signal_connect(button_revert, "clicked", G_CALLBACK(revert_changes), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_revert, 1, 6, 1, 1);

    button_run = gtk_button_new_with_label("Run Program");
    g_signal_connect(button_run, "clicked", G_CALLBACK(run_program), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_run, 0, 7, 2, 1);

    load_current_values();

    gtk_widget_show_all(window);

    gtk_main();

    if (app_widgets.original_content) {
        free(app_widgets.original_content);
    }

    return 0;
}

