#include <gtk/gtk.h>
void hello_destroy(GtkButton *btn , gpointer *user_data){
    const char * str = gtk_button_get_label(btn);
    if(g_strcmp0(str , "hello.") == 0){
        GtkAlertDialog  * alert = gtk_alert_dialog_new("hello.");
        GtkWidget *parent = gtk_widget_get_parent((GtkWidget *) btn);
        if (parent) {
            GtkWindow *parent_window = (GtkWindow *) gtk_widget_get_parent(parent);
            if (parent_window) {
                // 在这里使用parent_window
                gtk_alert_dialog_show(alert,parent_window);
                return;
            }
        }
        g_print("can not alert");
    }else{
        if(GTK_WINDOW(user_data)){
            gtk_window_destroy((GtkWindow *) user_data);
            return;
        }
    }
    return;
}
static void app_activate(GApplication *app, gpointer *user_data) {

    GtkApplicationWindow *win;
    GtkBox  *box;
    GtkLabel *label;
    GtkButton *btns[2];
    box = (GtkBox *) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    label = (GtkLabel *) gtk_editable_label_new("editable lable");
    win = (GtkApplicationWindow *) gtk_application_window_new(GTK_APPLICATION(app));
    btns[0] = (GtkButton *) gtk_button_new_with_label("hello.");
    btns[1] = (GtkButton *) gtk_button_new_with_label("destroy");

    g_signal_connect(btns[0] , "clicked" , G_CALLBACK(hello_destroy) , NULL);
    g_signal_connect(btns[1] , "clicked" , G_CALLBACK(hello_destroy) , win);
    gtk_box_append(box , (GtkWidget *) label);
    gtk_box_append(box , (GtkWidget *) btns[0]);
    gtk_box_append(box , (GtkWidget *) btns[1]);
    gtk_window_set_title(GTK_WINDOW (win), "title----01");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 200);
    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

    gtk_window_set_child(GTK_WINDOW(win) , (GtkWidget *) box);
    gtk_widget_set_visible((GtkWidget *) win, 1);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int stat;
    app = gtk_application_new("com.github.ToshioCP.pr2", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK(app_activate), NULL);
    stat = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return stat;
}
