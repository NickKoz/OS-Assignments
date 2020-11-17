#include "handler.h"
#include <signal.h>

void USR1_handler(int sig){
    if(sig == SIGUSR1){
        USR1_signals_recieved++;
    }
}

