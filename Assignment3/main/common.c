#include "common_types.h"


char* get_current_time(){

    time_t rawtime;
    struct tm * timeinfo;
    char* time_buffer = malloc(BUFFER_SIZE);
    memset(time_buffer, 0, BUFFER_SIZE);

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    struct timeval tv;

    gettimeofday(&tv, NULL);

    int millisec = lrint(tv.tv_usec/1000.0); // Round to nearest millisec
    if (millisec>=1000) { // Allow for rounding up to nearest second
        millisec -=1000;
        tv.tv_sec++;
    }

    timeinfo = localtime(&tv.tv_sec);
    
    sprintf(time_buffer, "%d:%d:%d:%d.%d", timeinfo->tm_mday, timeinfo->tm_hour,
    timeinfo->tm_min, timeinfo->tm_sec, (int)millisec/100);

    return time_buffer;

}


void write_to_log(const char* mess, int no_sldmaker, SharedSegment* shared){

    char* filename;

    if(no_sldmaker ==  GLOBAL_LOG){
        filename = shared->global_logfile;
    }
    else{
        filename = shared->logfiles[no_sldmaker-1];
    }

    FILE* fp = fopen(filename, "a+");
    char buffer_str[BUFFER_SIZE] = {0};

    char* curr_time = get_current_time();

    sprintf(buffer_str, "%s %d %s\n", curr_time, getpid(), mess);

    fputs(buffer_str, fp);

    free(curr_time);

    fclose(fp);
}
