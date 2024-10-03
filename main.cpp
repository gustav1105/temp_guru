#include <gtk/gtk.h>
#include "cpu_info.h"
#include "ui_helpers.h"
#include "gpu_info.h"
#include "graph_temps.h"
#include "bar_temps.h"  // Include the bar_temps header

gboolean update_cpu_temp_label(gpointer data) {
    GtkBuilder *builder = (GtkBuilder *)data;
    GtkWidget *cpu_temp_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_temp"));

    if (cpu_temp_label) {
        std::string temp_str = get_cpu_temp();
        double cpu_temp = std::stod(temp_str);
        gtk_label_set_text(GTK_LABEL(cpu_temp_label), temp_str.c_str());
        
        // Add to graph data
        add_temperature_data(cpu_temp, -1);  // Only update CPU temp
        gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(builder, "graph_temps")));

        // Update the CPU temp squares
        update_temp_squares(cpu_squares, cpu_temp);
    }

    return TRUE;
}

gboolean update_gpu_temp_label(gpointer data) {
    GtkBuilder *builder = (GtkBuilder *)data;
    GtkWidget *gpu_temp_label = GTK_WIDGET(gtk_builder_get_object(builder, "gpu_temp"));

    if (gpu_temp_label) {
        std::string temp_str = get_gpu_temp();
        double gpu_temp = std::stod(temp_str);
        gtk_label_set_text(GTK_LABEL(gpu_temp_label), temp_str.c_str());
        
        // Add to graph data
        add_temperature_data(-1, gpu_temp);  // Only update GPU temp
        gtk_widget_queue_draw(GTK_WIDGET(gtk_builder_get_object(builder, "graph_temps")));

        // Update the GPU temp squares
        update_temp_squares(gpu_squares, gpu_temp);
    }

    return TRUE;
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;

    // Initialize GTK and load the Glade file
    window = initialize_gtk(argc, argv, &builder);
    if (!window) {
        return 1;
    }

    init_graph(builder);

    // Update the label with CPU and GPU info
    update_cpu_label(builder, get_cpu_info());
    update_gpu_label(builder, get_gpu_info());

    // Fill the CPU and GPU temperature bars with squares
    fill_bar_with_squares(builder, "bar_temp_cpu", cpu_squares);
    fill_bar_with_squares(builder, "bar_temp_gpu", gpu_squares);

    // Set up periodic updates for CPU and GPU temperatures
    g_timeout_add(1000, update_cpu_temp_label, builder);
    g_timeout_add(1000, update_gpu_temp_label, builder); 

    // Show the window and start the GTK main loop
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

