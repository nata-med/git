#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <direct.h>
#include <time.h>
#include <io.h>

unsigned int hash_string(char* str){
    unsigned int hash=5381;
    int count;
    while (count=*str++){
        hash=((hash<<5)+hash)+count;
    }
    return hash;
}


void copy_file(char* what_to_copy, char* where_to_copy){
    FILE* file_what =fopen(what_to_copy, "rb");
    if(file_what==NULL){
        fprintf(stderr, "Error: file is empty\n");
        return;
    }
    FILE* file_where = fopen(where_to_copy, "wb");
    int c;
    while((c=fgetc(file_what))!=EOF){
        fputc(c, file_where);
    }
    fclose(file_what);
    fclose(file_where);
}

void compare_two_files(char *old_file_path, char *new_file_path, char *file_name){
    FILE* old_file=fopen(old_file_path, "r");
    FILE* new_file=fopen(new_file_path, "r");
    if(old_file!=NULL && new_file==NULL){
        printf("--- %s (Deleted)\n", file_name);
        fclose(old_file);
        return;
    }
    if(old_file==NULL && new_file!=NULL){
        printf("--- %s (New file)\n", file_name);
        char line[256];
        while (fgets(line, sizeof(line), new_file)) {
            printf("+ %s", line);
        }
        if (line[strlen(line) - 1] != '\n') printf("\n");
        fclose(new_file);
        return;
    }
    if(old_file==NULL && new_file==NULL) return;
    char old_line[100];
    char new_line[100];
    int line_num=1;
    while(1){
        char* cur_old = fgets(old_line, sizeof(old_line), old_file);
        char* cur_new = fgets(new_line, sizeof(new_line), new_file);
        if(cur_new==NULL && cur_old==NULL){
            break;
        }
        if(cur_new==NULL && cur_old!=NULL){
            printf("-%d: %s", line_num, old_line);
            if (old_line[strlen(old_line) - 1] != '\n') printf("\n");
        } else if(cur_new!=NULL && cur_old==NULL){
            printf("+%d: %s", line_num, old_line);
            if (new_line[strlen(new_line) - 1] != '\n') printf("\n");
        } else if(strcmp(old_line, new_line)!=0){
            printf("-%d: %s", line_num, old_line);
            if (old_line[strlen(old_line) - 1] != '\n') printf("\n");
            printf("+%d: %s", line_num, new_line);
            if (new_line[strlen(new_line) - 1] != '\n') printf("\n");
        }
        line_num++;
    }
    fclose(old_file);
    fclose(new_file);
}