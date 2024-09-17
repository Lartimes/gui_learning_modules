#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

void read_file_2(GtkButton *btn, gpointer *user_data) {
//void read_file(GtkButton *btn, gpointer *user_data) {
    const char *file_path = "F:\\Dev\\CProjects\\gui\\GTK4Learning\\readme.md";
    FILE *file;
    long file_size;
    char *content = NULL;

    // 打开文件
    file = fopen(file_path, "rb");
    if (file == NULL) {
        perror("无法打开文件");
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 分配内存并读取文件内容
    content = (char *) malloc(file_size + 1);
    if (content == NULL) {
        perror("无法分配内存");
        fclose(file);
    }

    fread(content, 1, file_size, file);
    content[file_size] = '\0';

    // 关闭文件
    fclose(file);
    gchar *text = content;
    g_print("%s\n" , text);
    GtkTextBuffer  *buffer =gtk_text_view_get_buffer((GtkTextView *) user_data);
    gtk_text_buffer_set_text(buffer , text , -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(user_data), GTK_TEXT_BUFFER(buffer));
}


//void hello_destroy(GtkButton *btn, gpointer *user_data) {
void hello_destroy_2(GtkButton *btn, gpointer *user_data) {
    const char *str = gtk_button_get_label(btn);
    if (g_strcmp0(str, "hello.") == 0) {
        GtkAlertDialog *alert = gtk_alert_dialog_new("hello.");
        GtkWidget *parent = gtk_widget_get_parent((GtkWidget *) btn);
        if (parent) {
            GtkWindow *parent_window = (GtkWindow *) gtk_widget_get_parent(parent);
            if (parent_window) {
                // 在这里使用parent_window
                gtk_alert_dialog_show(alert, parent_window);
                return;
            }
        }
        g_print("can not alert");
    } else {
        if (GTK_WINDOW(user_data)) {
            gtk_window_destroy((GtkWindow *) user_data);
            return;
        }
    }
    return;
}

//static void app_activate(GApplication *app, gpointer *user_data) {
static void app_activate_2(GApplication *app, gpointer *user_data) {

    GtkApplicationWindow *win;
    GtkBox *box;
    GtkLabel *label;
    GtkButton *btns[3];
    GtkTextView *textView;
    GtkTextBuffer *buffer;
    gchar *text;
    GtkScrolledWindow *scr;
    text = "Once upon a time, there was an old man who was called Taketori-no-Okina. "
           "It is a japanese word that means a man whose work is making bamboo baskets.\n"
           "One day, he went into a mountain and found a shining bamboo. "
           "\"What a mysterious bamboo it is!,\" he said. "
           "He cut it, then there was a small cute baby girl in it. "
           "The girl was shining faintly. "
           "He thought this baby girl is a gift from Heaven and took her home.\n"
           "His wife was surprized at his tale. "
           "They were very happy because they had no children. ";


    box = (GtkBox *) gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    label = (GtkLabel *) gtk_editable_label_new("editable lable");
    win = (GtkApplicationWindow *) gtk_application_window_new(GTK_APPLICATION(app));
    btns[0] = (GtkButton *) gtk_button_new_with_label("hello.");
    btns[1] = (GtkButton *) gtk_button_new_with_label("destroy");
    btns[2] = (GtkButton *) gtk_button_new_with_label("read file");
    textView = (GtkTextView *) gtk_text_view_new();


    scr = (GtkScrolledWindow *) gtk_scrolled_window_new();


    buffer = gtk_text_view_get_buffer(textView);
    gtk_text_buffer_set_text(buffer, text, -1);
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(textView), GTK_TEXT_BUFFER(buffer));
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW (textView), GTK_WRAP_WORD_CHAR);


    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW (scr), (GtkWidget *) textView);
    g_signal_connect(btns[0], "clicked", G_CALLBACK(hello_destroy_2), NULL);
    g_signal_connect(btns[1], "clicked", G_CALLBACK(hello_destroy_2), win);
    g_signal_connect(btns[2], "clicked", G_CALLBACK(read_file_2), textView);
    gtk_box_append(box, (GtkWidget *) label);
    gtk_box_append(box, (GtkWidget *) btns[0]);
    gtk_box_append(box, (GtkWidget *) btns[1]);
    gtk_box_append(box, (GtkWidget *) btns[2]);
    gtk_box_append(box, (GtkWidget *) scr);
//    gtk_box_append(box , (GtkWidget *)  textView);

    gtk_window_set_title(GTK_WINDOW(win), "controls");
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 400);
    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));
    gtk_window_set_child(GTK_WINDOW(win), (GtkWidget *) box);
    gtk_widget_set_visible((GtkWidget *) win, 1);
}

//int main(int argc, char **argv) {
//    GtkApplication *app;
//    int stat;
//    app = gtk_application_new("com.github.ToshioCP.pr2", G_APPLICATION_DEFAULT_FLAGS);
//    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
//    stat = g_application_run(G_APPLICATION(app), argc, argv);
//    g_object_unref(app);
//    return stat;
//}

