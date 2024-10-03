#include <gtk/gtk.h>
#include "ui_helpers.h"
#include "gpu_info.h"
#include "cpu_info.h"
// Function to update the label with CPU info
void update_cpu_label(GtkBuilder *builder, const std::string& cpu_info) {
    GtkWidget *cpu_info_label = GTK_WIDGET(gtk_builder_get_object(builder, "cpu_info"));

    if (cpu_info_label) {
        gtk_label_set_text(GTK_LABEL(cpu_info_label), cpu_info.c_str());
    } else {
        g_printerr("CPU info label not found\n");
    }
}

// Function to update the label with GPU info
void update_gpu_label(GtkBuilder *builder, const std::string& gpu_info) {
    GtkWidget *gpu_info_label = GTK_WIDGET(gtk_builder_get_object(builder, "gpu_info"));

    if (gpu_info_label) {
        gtk_label_set_text(GTK_LABEL(gpu_info_label), gpu_info.c_str());
    } else {
        g_printerr("GPU info label not found\n");
    }
}

// Function to initialize GTK, load Glade file, and return the main window
GtkWidget* initialize_gtk(int argc, char *argv[], GtkBuilder **builder) {
    gtk_init(&argc, &argv);

    *builder = gtk_builder_new();
    if (!gtk_builder_add_from_file(*builder, "app.glade", NULL)) {
        g_printerr("Error loading Glade file\n");
        return NULL;
    }

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(*builder, "app_window"));
    if (!window) {
        g_printerr("Main window not found\n");
    }

    // Connect the "destroy" signal to properly quit the GTK main loop
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    return window;
}

