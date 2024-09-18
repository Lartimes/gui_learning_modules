//
// Created by Lartimes on 2024/9/18.
//rename copied c files method-name
//
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#define MAX_NUM 100
#define  MAX_METHOD_CHARS 30

#define  INPUT "F:/Dev/CProjects/gui/GTK4Learning/src/main.c"
#define  OUTPUT "F:/Dev/CProjects/gui/GTK4Learning/src/file/test.c"
#define  RENAME_SUFFIX "test"

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

static void trim(char *str) {
    int i, j;
    int len = strlen(str);

    // 去除前导空白字符
    for (i = 0; i < len && (str[i]) == ' '; i++);

    if (i > 0) {
        for (j = i; j < len; j++) {
            str[j - i] = str[j];
        }
        str[len - i] = '\0';
    }

    // 去除尾部空白字符
    len = strlen(str);
    for (i = len - 1; i >= 0 && (str[i]) == ' '; i--);

    if (i < len - 1) {
        str[i + 1] = '\0';
    }
}

static char *substring(const char *src, int start, int len) {
    char *dest = (char *) malloc(len + 1);
    if (dest == NULL) {
        return NULL; // 内存分配失败
    }
    strncpy(dest, src + start, len);
    dest[len] = '\0'; // 确保字符串以空字符结尾
    return dest;
}


static void check_methods() {
    FILE *file;
    int i;
    const char *delimiter = ",";
    char line[100];
    file = fopen(input, "r");
    if (file == NULL) {
        printf("无法打开文件\n");
        exit(EXIT_FAILURE);
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        trim(line);
        if (strchr(line, ';') != NULL || strchr(line, '=') != NULL) { //               包含;=
            continue;
        } else if (strchr(line, '(') != NULL && strchr(line, ')') != NULL) {
            char *pos = strchr(line, '(');
            char *end = strchr(line, ')');
            int index = pos - line;
            if (index < 0) {
                index = 0;
            }
            int length = end - line - index - 1;
            if (length < 0) {
                length = 0;
            }
            char *method_args = substring(line, index + 1, length);
            if (method_args == NULL) {
                printf("can not substring;\n");
                return;
            }
            char *token = strtok(method_args, delimiter);
            int flag = 0;
            while (token != NULL) {
                trim(token);
                if (flag) break;
                for (int tmp = 0; tmp < strlen(token); ++tmp) {
                    if (*(token + tmp) == ' ') {
                        flag = 1;
                        break;
                    }
                }
                token = strtok(NULL, delimiter);
            }
//            查看index 之前是否有空白
            char *pre_pace = strchr(line, ' ');
            if (pre_pace - line >= index) {
                continue;
            }

            if (!flag) {
                continue;
            }
//            如果是method row
            if (pos != NULL) {
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
                while (line[i] != ' ') {
                    method_name[MAX_METHOD_CHARS - 1 - minus++] = line[i--];
                }
                int tmp = 0;
                int sum = minus;
                while (minus--) {
                    method_name[tmp] = method_name[MAX_METHOD_CHARS - sum + tmp];
                    tmp++;
                }
                method_name[tmp - 1] = '\0';
                str[count] = (char *) malloc(sizeof(char) * (strlen(method_name) + 1));
                strcpy(str[count++], method_name);
            }
        } else {
            continue;
        }
    }
    fclose(file);
}

char *replace(char *str, const char *find, const char *replace) {
    int length = strlen(str) + strlen(replace) - strlen(find);
    char *new_str = malloc(sizeof(char) * length);
    char *pos;
    int end;
    while ((pos = strstr(str, find)) != NULL) {
        end = pos - str;
        int i;
        for (i = 0; i < end; ++i) {
            *(new_str + i) = *(str + i);
        }
        strcat(new_str, replace);

        for (i = end + strlen(find); i < strlen(str); ++i) {
            *(new_str + i + (strlen(replace) - strlen(find))) = *(str + i);
        }
        break;
    }
    str = malloc(sizeof(char) * length);
    strcpy(str, new_str);
    free(new_str);
    return str;
}

static void rename_logic() {
    FILE *outputFile = fopen(output, "w");
    FILE *inputFile = fopen(input, "r");
    if (outputFile == NULL) {
        printf("Error opening file.\n");
    }
    char line[100];
    char *replace_str;
    while (fgets(line, sizeof(line), inputFile) != NULL) {
        replace_str = NULL;
        char *tmp_str = malloc(sizeof(char) * 50);
        for (int i = 0; i < count; ++i) {
            if (strstr(line, str[i]) != NULL) {
                strcpy(tmp_str, str[i]);
                strcat(tmp_str, "_");
                strcat(tmp_str, RENAME_SUFFIX);
                replace_str = replace(line, str[i], tmp_str);
                break;
            } else {
                continue;
            }
        }
        if (replace_str == NULL) {
            fprintf(outputFile, "%s", line);
        } else {
            printf("%s\n", replace_str);
            fprintf(outputFile, "%s", replace_str);
            free(replace_str);
            free(tmp_str);
        }
    }
    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    check_methods();
    rename_logic();
}








