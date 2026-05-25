#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "errors.h"
#include <time.h>
#define length_of_text 512

void func_status(){
    error_repository_not_exist();
    FILE *file= fopen(".vcs/index", "r");
    error_no_changes_tocommit(file);
    char status;
    char str[length_of_text];
    while(fscanf(file, " %c %511s", &status, str)==2){
        if (status=='A'){
            printf("Added %s\n", str);
        } else if (status=='R'){
            printf("Removed %s\n", str);
        } else if (status=='M'){
            printf("Modified %s\n", str);
        }
    }
}


void func_log(char *start_commit, int limit_print){
    error_repository_not_exist();
    char hash[length_of_text];
    if (start_commit==NULL){ 
        FILE* file= fopen(".vcs/HEAD", "r");
        error_issues_with(file, "HEAD");
        fscanf(file, "%511s", hash);
        fclose(file);
    }else{
        strcpy(hash, start_commit);
    }

    int print_counter=0;
    while(strcmp(hash, "0")!=0){
        if(print_counter>=limit_print && limit_print!=-1) break;
        char cur_address[length_of_text];
        char cur_message[length_of_text];
        char parent_hash[length_of_text];
        long long int cur_time=0;
        snprintf(cur_address,sizeof(cur_address), "%s/%s/metadata.txt", ".vcs/commits", hash);
        FILE* meta_file=fopen(cur_address, "r");
        error_issues_with(meta_file, "metadata");
        fscanf(meta_file, "%511s %511s %lld", parent_hash, cur_message, &cur_time);
        time_t timee = (time_t)cur_time;
        struct tm* time_info = localtime(&timee);
        char time_str[length_of_text];
        strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M", time_info);
        change_z_on_space(cur_message);
        printf("message: %s,  time: %s,  commit: %s\n", cur_message, time_str, hash);
        strcpy(hash, parent_hash);
        fclose(meta_file);
        print_counter++;
    }
}
void func_diff(char *main_commit){
    error_repository_not_exist();
    FILE* file_head= fopen(".vcs/HEAD", "r");
    error_issues_with(file_head, "HEAD");
    char cur_hash[length_of_text];
    fscanf(file_head, "%511s", cur_hash);
    fclose(file_head); 
    error_no_commits_yet(cur_hash);
    if (main_commit==NULL){
        char cur_address[length_of_text];
        snprintf(cur_address, sizeof(cur_address),".vcs/commits/%s/metadata.txt", cur_hash);
        FILE* file_meta=fopen(cur_address, "r");
        error_issues_with(file_meta, "metadata");
        char buf1[length_of_text];
        char buf2[length_of_text];
        char buf3[length_of_text];
        fscanf(file_meta, "%511s %511s %511s", buf1, buf2, buf3);
        
        char cur_file_name[length_of_text];
        char old_file_path[length_of_text];
        while (fscanf(file_meta, "%511s %511s", cur_file_name, cur_hash)==2){
            snprintf(old_file_path, sizeof(old_file_path),".vcs/commits/%s/%s", cur_hash, cur_file_name);
            compare_two_files(old_file_path, cur_file_name, cur_file_name);
        }
        fclose(file_meta);

    }else{
        char curr_address[length_of_text];
        snprintf(curr_address, sizeof(curr_address),".vcs/commits/%s/metadata.txt", cur_hash);
        FILE* head_meta=fopen(curr_address, "r"); 
        error_issues_with(head_meta, "metadata");
        char target_address[length_of_text];
        snprintf(target_address, sizeof(target_address),".vcs/commits/%s/metadata.txt", main_commit);
        FILE* target_meta=fopen(target_address, "r");
        error_issue_and_close(target_meta, head_meta, "metadata");
        
        char buf1[length_of_text], buf2[length_of_text], buf3[length_of_text];
        char cur_file_name[length_of_text], cur_file_hash[length_of_text];
        char target_file_name[length_of_text], target_file_hash[length_of_text];

        fscanf(head_meta, "%511s %511s %511s", buf1, buf2, buf3);
        while (fscanf(head_meta, "%511s %511s", cur_file_name, cur_file_hash)==2){
            rewind(target_meta);
            fscanf(target_meta, "%511s %511s %511s", buf1, buf2, buf3);
            int flag=0;
            while (fscanf(target_meta, "%511s %511s", target_file_name, target_file_hash)==2){
                if(strcmp(cur_file_name, target_file_name)==0){
                    flag=1;
                    if(strcmp(cur_file_hash, target_file_hash)!=0){
                        printf("* %s (hash changed: %s -> %s)\n", cur_file_name, cur_file_hash, target_file_hash);
                    }
                    break;
                }
            }
            if(!flag){
                printf("+ %s (hash: %s)\n", cur_file_name, cur_file_hash);  
            }
        }

        rewind(target_meta);
        fscanf(target_meta, "%511s %511s %511s", buf1, buf2, buf3);
        while (fscanf(target_meta, "%511s %511s", target_file_name, target_file_hash)==2){
            rewind(head_meta);
            fscanf(head_meta, "%511s %511s %511s", buf1, buf2, buf3);
            int flag=0;
            while (fscanf(head_meta, "%511s %511s", cur_file_name, cur_file_hash)==2){
                if(strcmp(target_file_name, cur_file_name)==0){
                    flag=1;
                    break;
                }
            }
            if(!flag){
                printf("- %s (hash: %s)\n", target_file_name, target_file_hash);
            }
        }
        
        fclose(head_meta); 
        fclose(target_meta); 
    }
}