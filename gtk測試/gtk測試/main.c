#include <gtk/gtk.h>

int main(int argc, char* argv[]) {
    gtk_init();
    GtkWidget* window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "GTK Test");
    gtk_window_present(GTK_WINDOW(window));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
    return 0;
}
