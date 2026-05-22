#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <time.h>

void func_init_repository(){
    if (access(".vcs", 0)==0){
        fprintf(stderr, "Error: repository already exists\n");
        return;
    }
    mkdir(".vcs");
    mkdir(".vcs/commits");
    FILE* file1 = fopen(".vcs/index", "w");
    if(file1==NULL){
        fprintf(stderr, "Error: issues while making .vcs/index\n");
        return;
    }
    fclose(file1);
    FILE* file2 = fopen(".vcs/HEAD","w");
    fprintf(file2, "%d", 0);
    fclose(file2);
}

void func_remove_file(char *filename){
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    if (access(filename,0)){
        fprintf(stderr, "Error: no such file names in directory\n");
        return;
    }
    struct stat path_stat;
    if (stat(filename, &path_stat) == 0) {
        if (path_stat.st_mode & S_IFDIR) {
            fprintf(stderr, "Error: '%s' is a directory. You can only add files.\n", filename);
            return;
        }
    }
    char status;
    char str[512];
    int flag=0;
    FILE *file_orig= fopen(".vcs/index", "r");
    if(file_orig==NULL){
        fprintf(stderr, "Error: there is no file ready to commit\n");
        return;
    }
    FILE *file_new= fopen(".vcs/index.tmp", "w");
    if (file_new == NULL) {
        fprintf(stderr, "Error: cannot create temporary index file (.vcs/index.tmp). Check disk space or permissions\n");
        fclose(file_orig); 
        return;
    }
    while(fscanf(file_orig, " %c %511s", &status, str)==2){
        if (strcmp(filename, str)==0){
            status='R';
            flag=1;
        }
        fprintf(file_new, "%c %s\n", status, str);
    }
    if(!flag){
        fprintf(file_new, "%c %s\n", 'R', filename);
    }
    fclose(file_new);
    fclose(file_orig);
    remove(".vcs/index");
    rename(".vcs/index.tmp", ".vcs/index");
}


void func_add_file(char *filename){
    if (strlen(filename) > 500) {
        fprintf(stderr, "Error: file name is too long (max 500 symbols)\n");
        return;
    }
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    if (access(filename,0)){
        fprintf(stderr, "Error: no such file names in directory\n");
        return;
    }
    struct stat path_stat;
    if (stat(filename, &path_stat) == 0) {
        if (path_stat.st_mode & S_IFDIR) { // Если это директория
            fprintf(stderr, "Error: '%s' is a directory. You can only add files.\n", filename);
            return;
        }
    }
    char status;
    char str[512];
    int flag=0;
    FILE *file_orig= fopen(".vcs/index", "r");
    if(file_orig==NULL){
        fprintf(stderr, "Error: there is no file ready to commit\n");
        return;
    }
    FILE *file_new= fopen(".vcs/index.tmp", "w");
    if (file_new == NULL) {
        fprintf(stderr, "Error: cannot create temporary index file (.vcs/index.tmp). Check disk space or permissions\n");
        fclose(file_orig); 
        return;
    }
    while(fscanf(file_orig, " %c %511s", &status, str)==2){
        if (strcmp(filename, str)==0){
            if(status=='R') status='A';
            flag=1;
        }
        fprintf(file_new, "%c %s\n", status, str);
    }
    if(!flag){
        fprintf(file_new, "%c %s\n", 'A', filename);
    }
    fclose(file_new);
    fclose(file_orig);
    remove(".vcs/index");
    rename(".vcs/index.tmp", ".vcs/index");
}


void func_make_commit(char *message){
    if (strlen(message) > 250) {
        fprintf(stderr, "Error: commit message is too long (max 250 symbols)\n");
        return;
    }
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    if(message==NULL){
        fprintf(stderr, "Error: there is no message to work with\n");
        return;
    }
    if (strchr(message, ' ') != NULL) {
        fprintf(stderr, "Error: the message must be without spaces, connect the words with _\n");
        return;
    }
    FILE *file= fopen(".vcs/index", "r");
    if(file==NULL){
        fprintf(stderr, "Error: there is no file ready to commit\n");
        return;
    }
    fclose(file);
    FILE *file_h= fopen(".vcs/HEAD", "r");
    if (file_h == NULL) {
        fprintf(stderr, "Error: repository is corrupted, HEAD file is missing\n");
        return;
    }
    char parent_hash[512];
    fscanf(file_h, "%511s", parent_hash);
    fclose(file_h);
    long int time_to_hash=time(NULL);
    char data_to_hash[512];
    snprintf(data_to_hash,sizeof(data_to_hash), "%s_%s_%ld", parent_hash, message, time_to_hash);
    unsigned int hash_int = hash_string(data_to_hash);
    char hash_str[512];
    snprintf(hash_str,sizeof(hash_str), "%08x", hash_int);
    char hash[512];
    snprintf(hash, sizeof(hash), "%s/%s",  ".vcs/commits", hash_str);
    mkdir(hash);
    FILE *file_i= fopen(".vcs/index", "r");
    char status;
    char str[512];
    char copy_name[512];
    while(fscanf(file_i, " %c %511s", &status, str)==2){
        if(status=='A'){
            snprintf(copy_name, sizeof(copy_name), "%s/%s", hash, str);
            copy_file(str, copy_name);
        }
    }
    fclose(file_i);
    char address[512];
    snprintf(address, sizeof(address),"%s/metadata.txt", hash);
    FILE* file_meta=fopen(address, "w");
    if (file_meta == NULL) {
        fprintf(stderr, "Error: cannot create metadata file for the new commit (%s)\n", address);
        return;
    }
    fprintf(file_meta, "%s %s %ld\n",  parent_hash, message, time_to_hash);

    if(strcmp(parent_hash, "none")!=0 && strcmp(parent_hash, "0")!=0){
        char cur_address[512];
        snprintf(cur_address,sizeof(cur_address), ".vcs/commits/%s/metadata.txt", parent_hash);
        FILE* cur_file=fopen(cur_address, "r");
        if (cur_file == NULL) {
            fprintf(stderr, "Error: cannot read parent commit metadata (%s) to copy history\n", cur_address);
            fclose(file_meta); 
            return;
        }
        char old_filename[512];
        char old_hash[512];
        fscanf(cur_file, "%511s %511s", old_filename, old_hash);
        while (fscanf(cur_file, "%511s %511s", old_filename, old_hash)==2){
            int flag=0;
            char status;
            char str[512];
            FILE* file_i=fopen(".vcs/index", "r");
            while (fscanf(file_i, " %c %511s", &status, str)==2){
                if(strcmp(old_filename, str)==0){
                    flag=1;
                    break;
                }
            }
            fclose(file_i);
            if(flag==0){
                fprintf(file_meta, "%s %s\n", old_filename, old_hash);
            }
        }
        fclose(cur_file);
    }
    FILE *cur_file_i= fopen(".vcs/index", "r");
        while (fscanf(cur_file_i, " %c %511s", &status, str)==2){
            if (status=='A'){
                fprintf(file_meta, "%s %s\n", str, hash_str);
            }
        }
        fclose(cur_file_i);
    fclose(file_meta);
    FILE* file_idx=fopen(".vcs/index", "w");
    if (file_idx == NULL) {
        fprintf(stderr, "Error: commit successful, but cannot clear the index file\n");
    } else {
        fclose(file_idx);
    }

    FILE* file_head=fopen(".vcs/HEAD", "w");
    if (file_head == NULL) {
        fprintf(stderr, "Error: commit successful, but cannot update HEAD file to the new commit\n");
    } else {
        fprintf(file_head, "%s", hash_str);
        fclose(file_head);
    }
}

void func_checkout(char* main_hash){
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    if (main_hash==NULL){
        fprintf(stderr, "Error: no hash of commit, print hash after func_checkout\n");
        return;
    }
    char address[512];
    snprintf(address, sizeof(address),".vcs/commits/%s/metadata.txt", main_hash);
    FILE* file= fopen(address, "r");
    if (file==NULL){
        fprintf(stderr, "Error: no commits in file\n");
        return;
    }
    char buf1[512];
    char buf2[512];
    char buf3[512];
    fscanf(file, "%511s %511s %511s", buf1, buf2, buf3);
    char cur_file_name[512];
    char cur_address[512];
    char cur_hash[512];
    while (fscanf(file, "%511s %511s", cur_file_name, cur_hash)==2){
        snprintf(cur_address, sizeof(cur_address),".vcs/commits/%s/%s", cur_hash, cur_file_name);
        copy_file(cur_address, cur_file_name);
    }
    fclose(file);

    FILE* file_head= fopen(".vcs/HEAD", "w");
    if (file_head == NULL) {
        fprintf(stderr, "Error: checkout completed, but cannot update HEAD file\n");
    } else {
        fprintf(file_head, "%s", main_hash);
        fclose(file_head);
    }

    FILE* file_index= fopen(".vcs/index", "w");
    if (file_index == NULL) {
        fprintf(stderr, "Error: checkout completed, but cannot clear the index file\n");
    } else {
        fclose(file_index);
    }
}