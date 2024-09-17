//
// Created by Lartimes on 2024/9/18.
//rename copied c files method-name
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX_NUM 100
#define  MAX_METHOD_CHARS 30


#define  INPUT "main.c"
#define  OUTPUT "file/test.c"
//获取所有方法，
//      如何判断？
//              注意规范
//           是否不包含 ; =
//              *  如果包含()  splitby ',’
//                                  两个args == 一个method expression
static char *str[MAX_NUM];
static const char *input = INPUT;
static const char *output = OUTPUT;
static int count = 0;

static void init() {
    for (int i = 0; i < MAX_NUM; ++i) {
        str[i] = (char *) malloc(sizeof(char) * MAX_METHOD_CHARS);
    }
    return;
}

static void check_methods() {
    FILE *file;
    int i;
    char line[100];
    file = fopen(input, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
        if (strchr(line, ';') != NULL || strchr(line, '=') != NULL) { //               包含;=
            continue;
        } else if (strchr(line, '(') != NULL && strchr(line, ')') != NULL) {
//            如果, args == 2 继续 此处不继续判断
            char *pos = strchr(line, '(');
            if (pos != NULL) {
                int index = pos - line;
                for (i = index - 1; i >= 0; i--) {
                    if (line[i] != ' ') {
                        break;
                    } else {
//                        找到最前一个' '
                        int first = i;
                        while (line[first] == ' ') {
                            first--;
                        }
                        int gap = i - first;
                        int length = sizeof line / sizeof(char);
                        for (int tmp = index; tmp < length; ++tmp) {
                            line[tmp - gap] = line[tmp];
                        }
                    }
                }
//                logic
                pos = strchr(line, '(');
                i = pos - line;
                char method_name[MAX_METHOD_CHARS];
                int minus = 0;
                while (line[i] != ' '){
                    method_name[MAX_METHOD_CHARS-1-minus++] = line[i--];
                }
                int tmp = 0;
                int sum = minus;
                while(minus--) {
                    method_name[tmp] =method_name[MAX_METHOD_CHARS-sum+tmp];
                    tmp ++;
                }
                method_name[tmp] = '\n';
                printf("debug =====================: %s" , method_name);
                str[count++] = method_name;
            }
        } else {
            continue;
        }
    }
    fclose(file);
}

//TODO
static  void rename_logic(){

}






