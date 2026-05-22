#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <direct.h>
#include <time.h>
#include <io.h>

void func_init_repository(){
    if (access(".vcs", 0)==0){
        fprintf(stderr, "Error: repository already exists\n");
        return;
    }
    mkdir(".vcs");
    mkdir(".vcs/commits");
    FILE* file1 = fopen(".vcs/index", "w");
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
    char status;
    char str[100];
    int flag=0;
    FILE *file_orig= fopen(".vcs/index", "r");
    if(file_orig==NULL){
        fprintf(stderr, "Error: there is no file ready to commit\n");
    }
    FILE *file_new= fopen(".vcs/index.tmp", "w");
    while(fscanf(file_orig, " %c %s", &status, str)==2){
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
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    if (access(filename,0)){
        fprintf(stderr, "Error: no such file names in directory\n");
        return;
    }
    char status;
    char str[100];
    int flag=0;
    FILE *file_orig= fopen(".vcs/index", "r");
    if(file_orig==NULL){
        fprintf(stderr, "Error: there is no file ready to commit\n");
    }
    FILE *file_new= fopen(".vcs/index.tmp", "w");
    while(fscanf(file_orig, " %c %s", &status, str)==2){
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
    char parent_hash[100];
    fscanf(file_h, "%s", parent_hash);
    fclose(file_h);
    long int time_to_hash=time(NULL);
    char data_to_hash[100];
    sprintf(data_to_hash, "%s_%s_%ld", parent_hash, message, time_to_hash);
    unsigned int hash_int = hash_string(data_to_hash);
    char hash_str[10];
    sprintf(hash_str, "%08x", hash_int);
    char hash[30];
    sprintf(hash, "%s/%s",  ".vcs/commits", hash_str);
    mkdir(hash);
    FILE *file_i= fopen(".vcs/index", "r");
    char status;
    char str[100];
    char copy_name[100];
    while(fscanf(file_i, " %c %s", &status, str)==2){
        if(status=='A'){
            sprintf(copy_name, "%s/%s", hash, str);
            copy_file(str, copy_name);
        }
    }
    fclose(file_i);
    char address[100];
    sprintf(address, "%s/metadata.txt", hash);
    FILE* file_meta=fopen(address, "w");
    fprintf(file_meta, "%s %s %ld\n",  parent_hash, message, time_to_hash);

    if(strcmp(parent_hash, "none")!=0 && strcmp(parent_hash, "0")!=0){
        char cur_address[100];
        sprintf(cur_address, ".vcs/commits/%s/metadata.txt", parent_hash);
        FILE* cur_file=fopen(cur_address, "r");
        char old_filename[100];
        char old_hash[10];
        fscanf(cur_file, "%s %s", old_filename, old_hash);
        while (fscanf(cur_file, "%s %s", old_filename, old_hash)==2){
            int flag=0;
            char status;
            char str[100];
            FILE* file_i=fopen(".vcs/index", "r");
            while (fscanf(file_i, " %c %s", &status, str)==2){
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
        while (fscanf(cur_file_i, " %c %s", &status, str)==2){
            if (status=='A'){
                fprintf(file_meta, "%s %s\n", str, hash_str);
            }
        }
        fclose(cur_file_i);
    fclose(file_meta);
    FILE* file_idx=fopen(".vcs/index", "w");
    fclose(file_idx);
    FILE* file_head=fopen(".vcs/HEAD", "w");
    if (file_head!=NULL) {
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
    char address[100];
    sprintf(address, ".vcs/commits/%s/metadata.txt", main_hash);
    FILE* file= fopen(address, "r");
    if (file==NULL){
        fprintf(stderr, "Error: no commits in file\n");
        return;
    }
    char buf1[100];
    char buf2[100];
    char buf3[100];
    fscanf(file, "%s %s %s", buf1, buf2, buf3);
    char cur_file_name[100];
    char cur_address[100];
    char cur_hash[100];
    while (fscanf(file, "%s %s", cur_file_name, cur_hash)==2){
        sprintf(cur_address, ".vcs/commits/%s/%s", cur_hash, cur_file_name);
        copy_file(cur_address, cur_file_name);
    }
    fclose(file);
    FILE* file_head= fopen(".vcs/HEAD", "w");
    fprintf(file_head, "%s", main_hash);
    FILE* file_index= fopen(".vcs/index", "w");
    fclose(file_index);
    fclose(file_head);
}