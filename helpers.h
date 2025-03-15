#ifndef HELPERS_H
#define HELPERS_H

#include "declarations.h"
#include <stdio.h>
#include <stdlib.h>

static inline void panic(const char *errmsg, u8 status, bool excep) {

    perror(errmsg);
    if(excep){

        exit(status);
    }

    
}


#endif // !HELPERS_H