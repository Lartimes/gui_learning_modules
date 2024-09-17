#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

static void app_activate_2(GApplication *app, gpointer user_data) {
    g_print("You need a filename argument.\n");
}

static void app_open_2(GApplication *application, GFile **files, gint n_files, gchar *hint, gpointer user_data) {
    GtkApplicationWindow *win;
    GtkScrolledWindow *scr;
    GtkTextBuffer *buffer;
    GtkTextView *textView;
    char *contents;
    gsize length;
    char *filename;



    textView = (GtkTextView *) gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(textView);
    gtk_text_view_set_buffer(textView, buffer);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (textView), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (textView), FALSE);

    scr = (GtkScrolledWindow *) gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW (scr), (GtkWidget *) textView);

    win = (GtkApplicationWindow *) gtk_application_window_new(GTK_APPLICATION(application));
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);
    gtk_window_set_title(GTK_WINDOW(win), "open signal");
    gtk_window_set_child(GTK_WINDOW(win), (GtkWidget *) scr);


//    TODO: readFile

    if (g_file_load_contents(files[0], NULL, &contents, &length, NULL, NULL)) {
        gtk_text_buffer_set_text(buffer, contents, length);
        g_free(contents);
        if ((filename = g_file_get_basename(files[0])) != NULL) {
            gtk_window_set_title(GTK_WINDOW (win), filename);
            g_free(filename);
        }
        gtk_widget_set_visible((GtkWidget *) win, 1);
    } else {
        if ((filename = g_file_get_path(files[0])) != NULL) {
            g_print("No such file: %s.\n", filename);
            g_free(filename);
        }
        gtk_window_destroy(GTK_WINDOW (win));
    }
}

//int main(int argc, char **argv) {
//    GtkApplication *app;
//    int stat;
//    app = gtk_application_new("com.github.ToshioCP.pr2", G_APPLICATION_HANDLES_OPEN);
//    g_signal_connect (app, "activate", G_CALLBACK(app_activate), NULL);
//    g_signal_connect (app, "open", G_CALLBACK(app_open), NULL);
//    stat = g_application_run(G_APPLICATION(app), argc, argv);
//    g_object_unref(app);
//    return stat;
//}

