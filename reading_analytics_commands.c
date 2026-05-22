#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <time.h>

void func_status(){
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    FILE *file= fopen(".vcs/index", "r");
    if(file==NULL){
        fprintf(stderr, "Error: no changes to commit\n");
        return;
    }
    char status;
    char str[512];
    while(fscanf(file, " %c %511s", &status, str)==2){
        if (status=='A'){
            printf("Added %s\n", str);
        } else if (status=='R'){
            printf("Removed %s\n", str);
        }
    }
}


void func_log(char *start_commit, int limit_print){
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    char hash[512];
    if (start_commit==NULL){ 
        FILE* file= fopen(".vcs/HEAD", "r");
        if (file == NULL) { 
            fprintf(stderr, "Error: repository is corrupted, HEAD file is missing\n");            
            return;
        }
        fscanf(file, "%511s", hash);
        fclose(file);
    }else{
        strcpy(hash, start_commit);
    }

    int print_counter=0;
    while(strcmp(hash, "0")!=0){
        if(print_counter>=limit_print && limit_print!=-1) break;
        char cur_address[512];
        char cur_message[512];
        char parent_hash[512];
        long long int cur_time=0;
        snprintf(cur_address,sizeof(cur_address), "%s/%s/metadata.txt", ".vcs/commits", hash);
        FILE* meta_file=fopen(cur_address, "r");
        if(meta_file==NULL){
            fprintf(stderr, "Error: it's the end of history or error of reading a commit\n");
            break;
        }
        fscanf(meta_file, "%511s %511s %lld", parent_hash, cur_message, &cur_time);
        time_t timee = (time_t)cur_time;
        struct tm* time_info = localtime(&timee);
        char time_str[512];
        strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M", time_info);
        printf("message: %s,  time: %s,  commit: %s\n", cur_message, time_str, hash);
        strcpy(hash, parent_hash);
        fclose(meta_file);
        print_counter++;
    }
}

void func_diff(){
    if(access(".vcs", 0)!=0){
        fprintf(stderr, "Error: repository is not exist yet, print 'init' to create it\n");
        return;
    }
    FILE* file_head= fopen(".vcs/HEAD", "r");
    if (file_head == NULL) { 
        fprintf(stderr, "Error: repository is corrupted, HEAD file is missing\n");        
        return;
    }
    char cur_hash[512];
    fscanf(file_head, "%511s", cur_hash);
    if(strcmp(cur_hash, "0")==0){
        fprintf(stderr, "Error: there is no commits yet, nothing to compare\n");
        fclose(file_head);
        return;
    }
    char cur_address[512];
    snprintf(cur_address, sizeof(cur_address),".vcs/commits/%s/metadata.txt", cur_hash);
    FILE* file_meta=fopen(cur_address, "r");
    if (file_meta==NULL){
        fprintf(stderr, "Error: cannot open metadata for commit %s\n", cur_hash);
        fclose(file_head);
        return;
    }
    char buf1[512];
    char buf2[512];
    char buf3[512];
    fscanf(file_meta, "%511s %511s %511s", buf1, buf2, buf3);
    char cur_file_name[512];
    char old_file_path[512];
    while (fscanf(file_meta, "%511s %511s", cur_file_name, cur_hash)==2){
        snprintf(old_file_path, sizeof(old_file_path),".vcs/commits/%s/%s", cur_hash, cur_file_name);
        compare_two_files(old_file_path, cur_file_name, cur_file_name);
    }

    fclose(file_meta);
    fclose(file_head);
}
