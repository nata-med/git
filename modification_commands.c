#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "errors.h"
#include <time.h>
#define length_of_text 512

void func_init_repository(){
    error_repository_exist();
    mkdir(".vcs");
    mkdir(".vcs/commits");
    FILE* file1 = fopen(".vcs/index", "w");
    error_issues_with(file1, "index");
    fclose(file1);
    FILE* file2 = fopen(".vcs/HEAD","w");
    error_issues_with(file2, "HEAD");
    fprintf(file2, "%d", 0);
    fclose(file2);
}

void func_remove_file(char *filename){
    error_repository_not_exist();
    error_no_such_filename(filename);
    error_directory(filename);
    char status;
    char str[length_of_text];
    int flag=0;
    FILE *file_orig= fopen(".vcs/index", "r");
    error_not_ready_to_commit(file_orig);
    FILE *file_new= fopen(".vcs/index.tmp", "w");
    error_issue_and_close(file_new, file_orig, "index.tmp");
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
    error_length(filename, 500, "file_name");
    error_repository_not_exist();
    error_no_such_filename(filename);
    error_directory(filename);

    char target_status = 'A';            
    FILE* head_file = fopen(".vcs/HEAD", "r");
    error_issues_with(head_file, "HEAD");
    if (head_file != NULL) {
        char head_hash[length_of_text];
        if (fscanf(head_file, "%511s", head_hash) == 1 && strcmp(head_hash, "0") != 0) {
            char meta_path[length_of_text];
            snprintf(meta_path, sizeof(meta_path), ".vcs/commits/%s/metadata.txt", head_hash);
            FILE* meta_file = fopen(meta_path, "r");
            error_issues_with(meta_file, "metadata");
            if (meta_file != NULL) {
                char buf1[length_of_text];
                char buf2[length_of_text];
                char buf3[length_of_text];
                fscanf(meta_file, "%511s %511s %511s", buf1, buf2, buf3); 
                char meta_name[length_of_text];
                char meta_hash[length_of_text];
                while (fscanf(meta_file, "%511s %511s", meta_name, meta_hash) == 2) {
                    if (strcmp(meta_name, filename) == 0) {
                        target_status = 'M'; 
                        break;
                    }
                }
                fclose(meta_file);
            }
        }
        fclose(head_file);
    }

    char status;
    char str[length_of_text];
    int flag=0;
    FILE *file_orig= fopen(".vcs/index", "r");
    error_not_ready_to_commit(file_orig);
    FILE *file_new= fopen(".vcs/index.tmp", "w");
    error_issue_and_close(file_new, file_orig, "index.tmp");
    while(fscanf(file_orig, " %c %511s", &status, str)==2){
        if (strcmp(filename, str)==0){
            status = target_status; 
            flag=1;
        }
        fprintf(file_new, "%c %s\n", status, str);
    }
    if(!flag){
        fprintf(file_new, "%c %s\n", target_status, filename); 
    }
    fclose(file_new);
    fclose(file_orig);
    remove(".vcs/index");
    rename(".vcs/index.tmp", ".vcs/index");
}


void func_make_commit(char *message){
    error_length(message, 250, "commit_message");
    error_repository_not_exist();
    error_nothing_to_work_with("message");
    change_space_on_z(message);
    FILE *file= fopen(".vcs/index", "r");
    error_not_ready_to_commit(file);
    fclose(file);
    FILE *file_h= fopen(".vcs/HEAD", "r");
    error_issues_with(file_h, "HEAD");
    char parent_hash[length_of_text];
    fscanf(file_h, "%511s", parent_hash);
    fclose(file_h);
    long int time_to_hash=time(NULL);
    char data_to_hash[length_of_text];
    snprintf(data_to_hash,sizeof(data_to_hash), "%s_%s_%ld", parent_hash, message, time_to_hash);
    unsigned int hash_int = hash_string(data_to_hash);
    char hash_str[length_of_text];
    snprintf(hash_str,sizeof(hash_str), "%08x", hash_int);
    char hash[length_of_text];
    snprintf(hash, sizeof(hash), "%s/%s",  ".vcs/commits", hash_str);
    mkdir(hash);
    FILE *file_i= fopen(".vcs/index", "r");
    char status;
    char str[length_of_text];
    char copy_name[length_of_text];
    while(fscanf(file_i, " %c %511s", &status, str)==2){
        if(status=='A' || status=='M'){
            snprintf(copy_name, sizeof(copy_name), "%s/%s", hash, str);
            create_dirs_for_file(copy_name);
            copy_file(str, copy_name);
        }
    }
    fclose(file_i);
    char address[length_of_text];
    snprintf(address, sizeof(address),"%s/metadata.txt", hash);
    FILE* file_meta=fopen(address, "w");
    error_issues_with(file_meta, "metadata");
    fprintf(file_meta, "%s %s %ld\n",  parent_hash, message, time_to_hash);

    if(strcmp(parent_hash, "none")!=0 && strcmp(parent_hash, "0")!=0){
        char cur_address[length_of_text];
        snprintf(cur_address,sizeof(cur_address), ".vcs/commits/%s/metadata.txt", parent_hash);
        FILE* cur_file=fopen(cur_address, "r");
        error_issues_with(cur_file, "metadata");
        char old_filename[length_of_text];
        char old_hash[length_of_text];
        char skip_time[length_of_text];
        fscanf(cur_file, "%511s %511s %511s", old_filename, old_hash, skip_time);     
        while (fscanf(cur_file, "%511s %511s", old_filename, old_hash)==2){
            int flag=0;
            char status;
            char str[length_of_text];
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
            if (status=='A' || status=='M'){
                fprintf(file_meta, "%s %s\n", str, hash_str);
            }
        }
        fclose(cur_file_i);
    fclose(file_meta);
    FILE* file_idx=fopen(".vcs/index", "w");
    error_success_but(file_idx, "commit");
    FILE* file_head=fopen(".vcs/HEAD", "w");
    error_success_print(file_head, hash_str, "commit");
}

void func_checkout(char* main_hash){
    error_repository_not_exist();
    error_nothing_to_work_with("hash");
    char address[length_of_text];
    snprintf(address, sizeof(address),".vcs/commits/%s/metadata.txt", main_hash);
    FILE* file= fopen(address, "r");
    error_no_commit_infile(file);
    char buf1[length_of_text];
    char buf2[length_of_text];
    char buf3[length_of_text];
    fscanf(file, "%511s %511s %511s", buf1, buf2, buf3);
    char cur_file_name[length_of_text];
    char cur_address[length_of_text];
    char cur_hash[length_of_text];
    while (fscanf(file, "%511s %511s", cur_file_name, cur_hash)==2){
        snprintf(cur_address, sizeof(cur_address),".vcs/commits/%s/%s", cur_hash, cur_file_name);
        create_dirs_for_file(cur_file_name);
        copy_file(cur_address, cur_file_name);
    }
    fclose(file);

    FILE* file_head= fopen(".vcs/HEAD", "w");
    error_success_print(file_head, main_hash, "checkout");
    FILE* file_index= fopen(".vcs/index", "w");
    error_success_but(file_index, "checkout");
    printf("Checkout completed. Working directory restored to %s\n", main_hash); 
}