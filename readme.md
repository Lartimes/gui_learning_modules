# GTK4学习

https://discourse.gnome.org/t/help-with-a-cmakelists-txt-for-gtk-4/7397

## controls

### GtkApplicationWindow


    **用 GtkApplication 时建议使用 GtkApplicationWindow 而不是 GtkWindow。**

### GtkTextView

​	如果添加超出窗口限制的更多字符，则高度会增加并且窗口会扩展。如果高度大于显示屏的高度，您将没法再控制窗口的大小，这是程序中的一个 Bug。这可以通过在 GtkApplicationWindow 和 GtkTextView 之间添加一个 GtkScrolledWindow 来解决。

您键入大量字符时，这次窗口不会扩展，它会滚动并显示一个滚动条。

```c
gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr), tv);
```

## string & memory

```c
#include <stdio.h>

int main(void)
{
    char a[20] , *b;
    a[0] = 'h';
    a[1] = 'e';
    a[2] = 'l';   
    a[3] = '0';

    b = a;
    printf("%s" , *b);
    printf("%s" , *(b+1));
}
```

- ### 只读字符串

  ```c
  int main(void ){
      char *s = "hello";
  //    字符串字面量是只读的。
  //    *(s + 2) = 'q'; 这种操作的结果在 C 标准中没有定义，可能会发生不好的事情，例如段错误。
      printf("%s" , s);
  }
  ```

- ## 字符数组作为字符串

  ​	如果数组的类型是`static`，那么它被放置在静态内存区域中，这样分配的内存地址在程序的生命周期内是固定的，并且可以被修改或读取。

  ​	如果数组的类型是 `auto`，则将其放入栈，如果数组是在函数内部定义的，它的默认是 `auto` 类型，当函数退出并返回给调用者时，栈区将消失，栈上定义的数组是可写的。

  ```c
  static char a[] = {'H', 'e', 'l', 'l', 'o', '\0'};
  
  void
  print_strings (void) {
    char b[] = "Hello";
  
    a[1] = 'a'; /* Because the array is static, it's writable. */
    b[1] = 'a'; /* Because the array is auto, it's writable. */
  
    printf ("%s\n", a); /* Hallo */
    printf ("%s\n", b); /* Hallo */
  }
  ```

  

- ## 在堆上创建字符串

  C库使用malloc / free 来获取内存,GLIB提供了函数g_new 和 g_free 来做同样的事

  ```c
  //g_new (struct_type, n_struct)
  //	g_new 是一个为数组分配内存的宏。
  //	struct_type 是数组元素的类型。
  //	n_struct 是数组的大小。
  //	返回值是指向数组的指针，它的类型是指向 struct_type 的指针。
  char *s;
  s = g_new (char, 10);
  /* s 指向 char 数组，数组大小为 10. */
  
  struct tuple {int x, y;} *t;
  t = g_new (struct tuple, 5);
  /* t points an array of struct tuple. */
  /* The size of the array is 5. */
  
  //g_free 释放内存：
  void g_free (gpointer mem);
  
  
  ```

  如果 `mem` 为 NULL，则 `g_free` 什么都不做。`gpointer` 是一种通用指针，它与 `void *` 相同，该指针可以转换为任何指针类型。相反，任何指针类型也可以转换为 `gpointer`。



内存泄露：

​	**如果忘记释放分配的内存，它将保持被分配状态，多次要求系统分配，用完了却不释放，随着时间推移便会有大量的已经不再使用却还未归还给操作系统的内存，这种情况称为内存泄漏**

注意：一些 GLib 函数返回的字符串不允许调用者释放，例如下面的函数：

```c
const char *
g_quark_to_string (GQuark quark);
```

## Open 信号

### G_APPLICATION_HANDLES_OPEN 标志

​	application 启动时 指定一个参数，确定了信号的接收参数

```c
typedef enum /*< prefix=G_APPLICATION >*/
{
  G_APPLICATION_FLAGS_NONE GIO_DEPRECATED_ENUMERATOR_IN_2_74_FOR(G_APPLICATION_DEFAULT_FLAGS),
  G_APPLICATION_DEFAULT_FLAGS GIO_AVAILABLE_ENUMERATOR_IN_2_74 = 0,
  G_APPLICATION_IS_SERVICE  =          (1 << 0),
  G_APPLICATION_IS_LAUNCHER =          (1 << 1),

  G_APPLICATION_HANDLES_OPEN =         (1 << 2),
  G_APPLICATION_HANDLES_COMMAND_LINE = (1 << 3),
  G_APPLICATION_SEND_ENVIRONMENT    =  (1 << 4),

  G_APPLICATION_NON_UNIQUE =           (1 << 5),

  G_APPLICATION_CAN_OVERRIDE_APP_ID =  (1 << 6),
  G_APPLICATION_ALLOW_REPLACEMENT   =  (1 << 7),
  G_APPLICATION_REPLACE             =  (1 << 8)
} GApplicationFlags;
```

我们已经使用了`G_APPLICATION_FLAGS_NONE`，这是最简单的标志，表示不允许任何参数，如果在运行应用程序时提供参数，则会发生错误。

标志 `G_APPLICATION_HANDLES_OPEN` 是第二简单的选项，它允许参数，但只允许文件名作为参数。应用程序假定所有参数都是文件名。



### open 信号

现在，当应用程序启动时，可以发出两个信号。

- activate 信号 --- 当没有参数时发出这个信号。
- open 信号 --- 当至少有一个参数时发出这个信号。

```
void user_function (GApplication *application,
                   gpointer      files,
                   gint          n_files,
                   gchar        *hint,
                   gpointer      user_data)
```

参数是：

- `application` --- 应用程序（通常是 GtkApplication）
- `files` --- GFiles 数组 [数组长度=n_files] [元素类型是 GFile]
- `n_files` --- `files` 的元素个数
- `hint` --- 调用实例提供的提示字符串（通常可以忽略）
- `user_data` --- 连接 handler 时提供的数据

接下来将描述如何读取指定文件（GFile）。



主要方法流程：

```c
if (g_file_load_contents (files[0], NULL, &contents, &length, NULL, NULL)) {
  gtk_text_buffer_set_text (tb, contents, length);
  g_free (contents);
  if ((filename = g_file_get_basename (files[0])) != NULL) {
    gtk_window_set_title (GTK_WINDOW (win), filename);
    g_free (filename);
  }
  gtk_widget_show (win);
} else {
  if ((filename = g_file_get_path (files[0])) != NULL) {
    g_print ("No such file: %s.\n", filename);
    g_free (filename);
  }
  gtk_window_destroy (GTK_WINDOW (win));
}
```

函数 `g_file_load_contents` 将文件内容加载到缓冲区中，它会自动分配缓冲区内存并让 “contents” 指向该缓冲区，缓冲区的长度设置为 `length`。如果文件的内容成功加载，则返回 TRUE，如果发生错误，则返回 FALSE。

如果此函数成功，则将内容插入 GtkTextBuffer，释放 `contents` 指向的缓冲区，设置窗口的标题，释放 `filename` 指向的内存，然后显示窗口。如果失败，它会输出错误消息并销毁窗口，导致程序退出。

## GtkNotebook



