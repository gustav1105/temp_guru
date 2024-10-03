#ifndef BAR_TEMPS_H
#define BAR_TEMPS_H

#include <gtk/gtk.h>

extern GtkWidget *cpu_squares[20];  // External declaration for CPU squares
extern GtkWidget *gpu_squares[20];  // External declaration for GPU squares

void fill_bar_with_squares(GtkBuilder *builder, const char *box_name, GtkWidget *squares[]);
void update_temp_squares(GtkWidget *squares[], double temp);

#endif // BAR_TEMPS_H

