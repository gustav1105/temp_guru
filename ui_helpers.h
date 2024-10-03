#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include <gtk/gtk.h>
#include <string>

void update_cpu_label(GtkBuilder *builder, const std::string& cpu_info);
void update_gpu_label(GtkBuilder *builder, const std::string& gpu_info);
GtkWidget* initialize_gtk(int argc, char *argv[], GtkBuilder **builder);  // Add this line

#endif // UI_HELPERS_H

