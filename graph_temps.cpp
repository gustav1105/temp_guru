#include "graph_temps.h"
#include <cairo.h>
#define MAX_POINTS 100

static double cpu_temps[MAX_POINTS];
static double gpu_temps[MAX_POINTS];
static int point_index = 0;
static int valid_points = 0;  // Keep track of valid data points
static int time_elapsed = 0;  // Track total elapsed seconds

void init_graph(GtkBuilder *builder) {
    GtkWidget *graph_area = GTK_WIDGET(gtk_builder_get_object(builder, "graph_temps"));
    g_signal_connect(G_OBJECT(graph_area), "draw", G_CALLBACK(update_graph), NULL);
}

void add_temperature_data(double cpu_temp, double gpu_temp) {
    if (cpu_temp >= 0) {
        cpu_temps[point_index] = cpu_temp;
    } else {
        cpu_temps[point_index] = cpu_temps[(point_index - 1 + MAX_POINTS) % MAX_POINTS];
    }

    if (gpu_temp >= 0) {
        gpu_temps[point_index] = gpu_temp;
    } else {
        gpu_temps[point_index] = gpu_temps[(point_index - 1 + MAX_POINTS) % MAX_POINTS];
    }

    point_index = (point_index + 1) % MAX_POINTS;
    if (valid_points < MAX_POINTS) {
        valid_points++;  // Increment until we fill the buffer
    }
    time_elapsed++;  // Increment time by 1 second with each update
}

gboolean update_graph(GtkWidget *widget, cairo_t *cr, gpointer data) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    int width = allocation.width;
    int height = allocation.height;

    // Set min and max temperature values for the y-axis
    double min_temp = 0;
    double max_temp = 100;

    // Draw transparent background
    cairo_set_source_rgba(cr, 1, 1, 1, 0.0);  // Transparent background
    cairo_paint(cr);

    // Draw X and Y axis
    cairo_set_source_rgb(cr, 1, 1, 1);  // White lines for X and Y axis
    cairo_move_to(cr, 0, height - 20);
    cairo_line_to(cr, width, height - 20);  // X axis
    cairo_move_to(cr, 40, 0);  // Move the Y axis slightly right for label space
    cairo_line_to(cr, 40, height);  // Y axis
    cairo_stroke(cr);

    // Draw Y axis labels (temperature values)
    cairo_set_source_rgb(cr, 1, 1, 1);  // White for text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);

    for (double temp = min_temp; temp <= max_temp; temp += 10) {
        if (temp == 0) continue;
        int y = height - 20 - ((temp - min_temp) / (max_temp - min_temp)) * (height - 40);
        cairo_move_to(cr, 0, y + 5);
        char label[10];
        snprintf(label, sizeof(label), "%.0f°C", temp);
        cairo_show_text(cr, label);
    }

    // Adjust drawing for wrapping index
    int start = (point_index + MAX_POINTS - valid_points) % MAX_POINTS;

    // Draw X axis time labels
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_set_font_size(cr, 12);

    // Draw the labels for time points, aligning them with the corresponding data points
    for (int i = 0; i < valid_points; i += 10) {
        int idx = (start + i) % MAX_POINTS;  // Wrap index

        int x = 45 + (i * (width - 50)) / MAX_POINTS;
        cairo_move_to(cr, x, height - 5);

        // Calculate the time corresponding to this point
        int elapsed_for_point = time_elapsed - valid_points + i;
        if (elapsed_for_point < 0) {
            continue;  // Skip labels for points before we started recording
        }

        char time_label[20];
        int minutes = (elapsed_for_point / 60) % 60;
        int seconds = elapsed_for_point % 60;
        int hours = elapsed_for_point / 3600;

        if (hours > 0) {
            snprintf(time_label, sizeof(time_label), "%d:%02d", hours, minutes);
        } else {
            snprintf(time_label, sizeof(time_label), "%d:%02d", minutes, seconds);
        }
        cairo_show_text(cr, time_label);
    }

    // Plot CPU temps (red)
    cairo_set_source_rgb(cr, 1, 0, 0);
    for (int i = 0; i < valid_points - 1; i++) {
        int idx1 = (start + i) % MAX_POINTS;
        int idx2 = (start + i + 1) % MAX_POINTS;

        int x1 = 40 + (i * (width - 50)) / MAX_POINTS;
        int y1 = height - 20 - ((cpu_temps[idx1] - min_temp) / (max_temp - min_temp)) * (height - 40);

        int x2 = 40 + ((i + 1) * (width - 50)) / MAX_POINTS;
        int y2 = height - 20 - ((cpu_temps[idx2] - min_temp) / (max_temp - min_temp)) * (height - 40);

        cairo_move_to(cr, x1, y1);
        cairo_line_to(cr, x2, y2);
    }
    cairo_stroke(cr);

    // Plot GPU temps (blue)
    cairo_set_source_rgb(cr, 0, 0, 1);
    for (int i = 0; i < valid_points - 1; i++) {
        int idx1 = (start + i) % MAX_POINTS;
        int idx2 = (start + i + 1) % MAX_POINTS;

        int x1 = 40 + (i * (width - 50)) / MAX_POINTS;
        int y1 = height - 20 - ((gpu_temps[idx1] - min_temp) / (max_temp - min_temp)) * (height - 40);

        int x2 = 40 + ((i + 1) * (width - 50)) / MAX_POINTS;
        int y2 = height - 20 - ((gpu_temps[idx2] - min_temp) / (max_temp - min_temp)) * (height - 40);

        cairo_move_to(cr, x1, y1);
        cairo_line_to(cr, x2, y2);
    }
    cairo_stroke(cr);

    return FALSE;
}

