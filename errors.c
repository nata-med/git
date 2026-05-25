#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <io.h>
    #define access _access
#else
    #include <unistd.h>
#endif
#include "commands.h"
#include "errors.h"

void error_repository_exist(){
    if (access(".vcs", 0)==0){
        fprintf(stderr, "Error: repository already exists\n");
        exit(1);
    }
}

void error_repository_not_exist(){
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        exit(1);
    }
}

void error_no_such_filename(char *filename){
    if (access(filename,0)){
        fprintf(stderr, "Error: no such file names in directory\n");
        exit(1);
    }
}

void error_not_ready_to_commit(FILE* file_orig){
    if(file_orig==NULL){
        fprintf(stderr, "Error: there is no file ready to commit\n");
        exit(1);
    }
}

void error_issues_with(FILE* file, char* arg){
    if(file==NULL){
        fprintf(stderr, "Error: issues with making/opening %s file\n", arg);
        exit(1);
    }
}

void error_issue_and_close(FILE* file1, FILE* file2, char* arg){
    if (file1 == NULL) {
        fprintf(stderr, "Error: issues with making/opening %s file\n", arg);
        if (file2 != NULL) fclose(file2); 
        exit(1);
    }
}

void error_nothing_to_work_with(char* arg){
    if(arg==NULL){
        fprintf(stderr, "Error: there is no %s to work with\n", arg);
        exit(1);
    }
}

void error_length(char* filename, int arg, char* message){
    if (strlen(filename) > arg) {
        fprintf(stderr, "Error: %s is too long (max %d symbols)\n", message, arg);
        exit(1);
    }
}

void error_directory(char* filename){
    struct stat path_stat;
    if (stat(filename, &path_stat) == 0) {
        if (path_stat.st_mode & S_IFDIR) { 
            fprintf(stderr, "Error: '%s' is a directory. You can only add files.\n", filename);
            exit(1);
        }
    }
}

void error_success_print(FILE* file_head, char* hash, char* arg1){
    if (file_head == NULL) {
        fprintf(stderr, "Error: %s successful, but cannot update HEAD file\n", arg1);
    } else {
        fprintf(file_head, "%s", hash);
        fclose(file_head);
    }
}

void error_success_but(FILE* file_idx, char* arg1){
    if (file_idx == NULL) {
        fprintf(stderr, "Error: %s successful, but cannot clear the index file\n", arg1);
    } else {
        fclose(file_idx);
    }
}

void error_no_commit_infile(FILE* file){
    if (file==NULL){
        fprintf(stderr, "Error: no commits in file\n");
        exit(1);
    }
}    

void error_no_commits_yet(char* hash){
    if(strcmp(hash, "0")==0){
        fprintf(stderr, "Error: there is no commits yet, nothing to compare\n");
        exit(1);
    }
}

void error_no_changes_tocommit(FILE* file){
    if(file==NULL){
        fprintf(stderr, "Error: no changes to commit\n");
        exit(1);
    }
}

void error_need_name_of(int arg1, int arg2, char* arg3){
    if (arg1<arg2){
        fprintf(stderr,"Error: you need to put name of %s\n", arg3);
        exit(1);
    }
}

void error_need_message(int arg1, int arg2){
    if (arg1<arg2){
        fprintf(stderr,"Error: you need to put message\n");
        exit(1);
    }
}

void error_unknown(){
    fprintf(stderr,"Error: unknown command(\n");
    exit(1);
}