#include <stdio.h>

void error_repository_exist();
void error_repository_not_exist();
void error_no_such_filename(char *filename);
void error_not_ready_to_commit(FILE* file_orig);
void error_issues_with(FILE* file, char* arg);
void error_issue_and_close(FILE* file1, FILE* file2, char* arg);
void error_nothing_to_work_with(char* arg);
void error_length(char* filename, int arg, char* message);
void error_directory(char* filename);
void error_success_print(FILE* file_head, char* hash, char* arg1);
void error_success_but(FILE* file_idx, char* arg1);
void error_no_commit_infile(FILE* file);
void error_no_commits_yet(char* hash);
void error_no_changes_tocommit(FILE* file);
void error_need_name_of(int arg1, int arg2, char* arg3);
void error_need_message(int arg1, int arg2);
void error_unknown();
void error_outside_repository(char* filename);