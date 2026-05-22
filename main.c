#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "commands.h"

int main(int argc, char *argv[]){
    //if argc is not enough
    if (argc<2){
        fprintf(stderr,"Error: you need to put name of command");
        return 1;
    }
    if (strcmp(argv[1], "init")==0){
        func_init_repository();

    }else if (strcmp(argv[1], "add")==0){        
        if (argc<3){
            fprintf(stderr,"Error: you need to put name of file");
            return 1;
        }
        func_add_file(argv[2]);

    }else if (strcmp(argv[1], "remove")==0){
        if (argc<3){
            fprintf(stderr,"Error: you need to put name of file");
            return 1;
        }
        func_remove_file(argv[2]);

    }else if (strcmp(argv[1], "commit")==0){
        if (argc<3){
            fprintf(stderr,"Error: you need to put message");
            return 1;
        }
        func_make_commit(argv[2]);

    }else if (strcmp(argv[1], "log")==0){
        char* start_commit=NULL;
        int limit=-1;
        for(int i=2; i<argc; i++){
            if(strcmp(argv[i], "--n")==0  && i+1<argc){
                limit=atoi(argv[i+1]);
                i++;
            }else{
                start_commit=argv[i];
            }
        }
        func_log(start_commit, limit);

    }else if (strcmp(argv[1], "diff")==0){
        if (argc>= 3) {
            func_diff(argv[2]); 
        } else {
            func_diff(NULL);   
        }

    }else if (strcmp(argv[1], "status")==0){
        func_status();

    }else if (strcmp(argv[1], "checkout")==0){
        if (argc<3){
            fprintf(stderr,"Error: you need to put name of file");
            return 1;
        }
        func_checkout(argv[2]);
    }else if (strcmp(argv[1], "--help")==0 || strcmp(argv[1], "help")==0){
        func_print_help();
    }else if (strcmp(argv[1], "help_prog_version")==0){
        help_prog_version(argc, argv);
    }else{
        fprintf(stderr,"Error: unknown command(");
        return 1;
    }

    return 0;
}