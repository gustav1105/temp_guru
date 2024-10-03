#ifndef GRAPH_TEMPS_H
#define GRAPH_TEMPS_H

#include <gtk/gtk.h>

void init_graph(GtkBuilder *builder);
gboolean update_graph(GtkWidget *widget, cairo_t *cr, gpointer data);
void add_temperature_data(double cpu_temp, double gpu_temp);

#endif // GRAPH_TEMPS_H

