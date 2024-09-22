#include <gtk/gtk.h>



static void app_activate(GApplication * win , gpointer * user_data){
    g_print("app-activate\n");
}
static void app_open (GApplication *application,
                      GFile **files,
                      gint n_files,
                      gchar *hint,
                      gpointer user_data) {
    GtkApplicationWindow  *win;
    GtkScrolledWindow *scr ;
    GtkNotebookPage  *notebookPage;
    GtkNotebook  *notebook;

    char *contents;
    gsize length;
    char *filename;
    GtkLabel *lab;



    win = (GtkApplicationWindow *) gtk_application_window_new(GTK_APPLICATION(application));
    scr = (GtkScrolledWindow *) gtk_scrolled_window_new();


    gtk_window_set_default_size(GTK_WINDOW(win) , 400 , 200);
    gtk_window_set_title(GTK_WINDOW(win),"edit notebook");
    gtk_window_maximize(GTK_WINDOW(win));
    gtk_window_set_child(GTK_WINDOW(win) , (GtkWidget *) scr);
    gtk_widget_set_visible((GtkWidget *) win, 1);


    for (int i = 0; i < n_files; ++i){
        if(g_file_load_contents(files[i] , NULL  ,&contents , &length ,NULL , NULL )){

        }else if ((filename == )) {
            g_print("no valid file is given\n");
            g_free(filename);
        }else
            g_print("no valid file is given\n");



    }





}
int main(int argc, char **argv) {
    GtkApplication *app;
    int stat;
    app = gtk_application_new("com.github.ToshioCP.pr2", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect (app, "activate", G_CALLBACK(app_activate), NULL);
    g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
    stat = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return stat;
}

