#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <direct.h>
#include <time.h>
#include <io.h>

void func_print_help(){
    printf(" Here you will see all instructions for using the commands:\n ");
    printf(" command : init  -- used to create a repository\n");
    printf(" command : add  -- adds the specified file to the list of files to be included in the next commit\n");
    printf(" command : remove  -- adds the specified file to the list of files that should be included in the next commit, with a note that the specified file has been removed\n");
    printf(" command : commit  -- creates a commit, committing the state of the files changed by the functions above\n");
    printf(" command : log  -- shows all commits\n");
    printf(" command : diff  -- compares current files with a previous version\n");
    printf(" command : status  -- shows a list of files that you have changed, added, or deleted but not yet saved\n");
    printf(" command : checkout  -- gets the old version of a file from a commit and returns it to the current directory\n");
    printf(" command : help_prog_version  -- shows a detailed description of all functions, with a list of arguments and general operating logic\n");
    printf(" If you want to know deeply about functions, ask 'help_prog_version' ");
}


void help_prog_version(int argc, char* argv[]){
    
    if (argc<3){
        printf("What function do you want to know about?\n");
        printf("Print one of given variants: init, add, remove, commit, log, diff, status, checkout, help\n");        
        return;
    }
    
    if (strcmp(argv[2], "init")==0){
        printf("What it accepts: Nothing\n");
        printf("Data handling: 1. Creates a structure: the root directory .vcs/ and .vcs/commits/ to store future files.\n");
        printf("2. Initializes the project index: creates an empty file .vcs/index.\n");
        printf("3. Initializes the state pointer: creates a file .vcs/HEAD and writes the initial text value '0' to it.\n");
        
    }else if (strcmp(argv[2], "add")==0){        
        printf("What it accepts: The filename or relative path to a file in the current directory.\n");
        printf("Data handling: 1. Opens the current index .vcs/index for reading and creates a temporary file .vcs/index.tmp for writing.\n");
        printf("2. In a while loop, reads [status] [filename] pairs from the index. If the passed file was already in the index with the status 'R' (removed), it changes its status to 'A' (added). All remaining lines are copied as is to .vcs/index.tmp.\n");
        printf("3. If the file was not in the index, appends a line of the following format to the end of the temporary file: A filename.\n");
        
    }else if (strcmp(argv[2], "remove")==0){
        printf("What it accepts: The filename (to be marked as removed in the next commit).\n");
        printf("Data handling: 1. Opens the index and creates a temporary index file.\n");
        printf("2. Scans the index. If the file is already listed in the index, it changes its status to 'R' (Removed). If it's not in the index yet (it means that index was cleared after commit), it appends a new line 'R filename'.\n");
        printf("3. Replaces the old index with the updated temporary index.\n");
        
    }else if (strcmp(argv[2], "commit")==0){
        printf("What it accepts: Text message for the commit.\n");
        printf("Data handling: 1. Reads the parent hash from HEAD and generates a new unique hash using the parent hash, message, and current time.\n");
        printf("2. Creates a new directory .vcs/commits/[hash]/ and copies all physical files marked 'A' from the index into it.\n");
        printf("3. Creates metadata.txt with the commit header. Takes unmodified file records from the parent's metadata.txt and appends the new added files.\n");
        printf("4. Clears the index and updates HEAD with the newly generated commit hash.\n");
        
    }else if (strcmp(argv[2], "log")==0){
        printf("What it accepts: Starting commit hash (or reads from HEAD by default) and an integer print limit.\n");
        printf("Data handling: 1. Loops through history starting from the given hash by reading metadata.txt of each commit.\n");
        printf("2. Extracts and prints the commit message, time, and hash to the console.\n");
        printf("3. Shifts the pointer to the parent hash, moving backward in time until it hits the print limit or the initial '0' commit.\n");
        
    }else if (strcmp(argv[2], "diff")==0){
        printf("What it accepts: Nothing\n");
        printf("Data handling: 1. Opens metadata.txt of the target commit and skips the header.\n");
        printf("2. For each file, opens both the saved version in .vcs/commits/ and the physical file in the current working directory.\n");
        printf("3. Compares them line by line and prints the differences (+ for added lines, - for removed lines) to the console.\n");
        
    }else if (strcmp(argv[2], "status")==0){
        printf("What it accepts: Nothing.\n");
        printf("Data handling: 1. Opens the .vcs/index file for reading.\n");
        printf("2. Reads each line and prints 'Added [filename]' if the status is 'A', or 'Removed [filename]' if the status is 'R'.\n");
        
    }else if (strcmp(argv[2], "checkout")==0){
        printf("What it accepts: The target commit hash to restore.\n");
        printf("Data handling: 1. Opens metadata.txt of the target commit and skips the technical header.\n");
        printf("2. Reads the file list and copies each stored file from .vcs/commits/[hash]/ back to the main working directory, overwriting current files.\n");
        printf("3. Updates HEAD to the target hash and clears the index.\n");
        
    }else if (strcmp(argv[2], "--help")==0 || strcmp(argv[2], "help")==0){
        printf("What it accepts: Nothing.\n");
        printf("Data handling: It's a general user manual (func_print_help) explaining what each command does from a user's perspective.\n");
        
    }else{
        fprintf(stderr, "Error: Wrong name. Write name one of the given command: init, add, remove, commit, log, diff, status, checkout, help\n");
        return;
    }
}