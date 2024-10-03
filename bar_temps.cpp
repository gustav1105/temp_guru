#include "bar_temps.h"

GtkWidget *cpu_squares[20];  // Array for CPU temperature squares
GtkWidget *gpu_squares[20];  // Array for GPU temperature squares

void fill_bar_with_squares(GtkBuilder *builder, const char *box_name, GtkWidget *squares[]) {
    GtkWidget *bar_temp = GTK_WIDGET(gtk_builder_get_object(builder, box_name));
    
    if (!bar_temp) {
        g_print("Error: GtkBox '%s' not found\n", box_name);
        return;
    }

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        " .green-square { background-color: green; }"
        " .yellow-square { background-color: yellow; }"
        " .amber-square { background-color: orange; }"
        " .red-square { background-color: red; }"
        " .gray-square { background-color: lightgray; }", -1, NULL);

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider),
        GTK_STYLE_PROVIDER_PRIORITY_USER);

    for (int i = 0; i < 20; i++) {
        squares[i] = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);  // Create a square-like box
        gtk_widget_set_size_request(squares[i], 20, 20);
        gtk_style_context_add_class(gtk_widget_get_style_context(squares[i]), "gray-square");
        gtk_box_pack_start(GTK_BOX(bar_temp), squares[i], TRUE, TRUE, 5);
    }

    gtk_widget_show_all(bar_temp);
    g_object_unref(css_provider);
}

void update_temp_squares(GtkWidget *squares[], double temp) {
    int colored_squares = temp / 5;  // Each square represents 5 degrees

    for (int i = 0; i < 20; i++) {
        GtkStyleContext *context = gtk_widget_get_style_context(squares[i]);

        // Remove all existing classes before applying a new one
        gtk_style_context_remove_class(context, "green-square");
        gtk_style_context_remove_class(context, "yellow-square");
        gtk_style_context_remove_class(context, "amber-square");
        gtk_style_context_remove_class(context, "red-square");
        gtk_style_context_remove_class(context, "gray-square");

        // Apply the appropriate class based on the temperature
        if (i < colored_squares) {
            if (temp <= 40) {
                gtk_style_context_add_class(context, "green-square");
            } else if (temp <= 60) {
                gtk_style_context_add_class(context, "yellow-square");
            } else if (temp <= 80) {
                gtk_style_context_add_class(context, "amber-square");
            } else {
                gtk_style_context_add_class(context, "red-square");
            }
        } else {
            gtk_style_context_add_class(context, "gray-square");
        }
    }
}

