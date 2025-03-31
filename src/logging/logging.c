/*
    file:logging.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "headers/logging.h"

const char *LOG_PATH = "../../log/logging.log";

char *get_timestamp(){
    static char buffer[30];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", t);
    return buffer;
}

void write_log(char *message){

    char *timeStamp = get_timestamp();

    FILE *logFile = fopen(LOG_PATH, "a"); 
    if (logFile == NULL) {
        perror("Errore nell'apertura del file di log");
        exit(EXIT_FAILURE);
    }

    fprintf(logFile, "[%s] %s\n", get_timestamp(), message);
    fflush(logFile); 
    fclose(logFile);
}