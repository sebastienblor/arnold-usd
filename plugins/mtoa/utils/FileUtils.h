#pragma once

#include <stdio.h>

inline bool fileExists(const char* name) {
    if (FILE *file = fopen(name, "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

