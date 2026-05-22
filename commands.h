#ifndef SUM_H_749837
#define SUM_H_749837
#include <stdio.h>
void func_init_repository();
void func_add_file(char *el);
void func_remove_file(char *el);
void func_make_commit(char *el);
void func_log(char *start_commit, int limit_print);
void func_diff();
void func_status();
void func_checkout(char *el);
unsigned int hash_string(char* str);
void func_print_help();
void help_prog_version();
void help_prog_version(int argc, char* argv[]);
void copy_file(char* what_to_copy, char* where_to_copy);
void compare_two_files(char *old_file_path, char *new_file_path, char *file_name);
#endif