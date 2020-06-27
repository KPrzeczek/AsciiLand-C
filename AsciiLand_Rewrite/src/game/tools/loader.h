#ifndef LOADER_H
#define LOADER_H

#include "../util/maths/v2.h"
#define MAX_PAIRS 256

typedef struct FileData {
    const char *file_name;
    char *file_contents;
    
    char* strings[MAX_PAIRS];
    int numstr;
    
    int ints[MAX_PAIRS];
    int numints;
    
    V2 vecs[MAX_PAIRS];
    int numvecs;
} FileData;

void *Loader_ReloadFromFile(FileData *data, const char *fileName);
FileData *Loader_LoadFromFile(const char *fileName);
static char *Loader_ReturnValue(char **pbuf, const char *ind);

FileData *Loader_ReadFile(const char *fileName);
void Loader_ParseFileContents(FileData *data);

void Loader_UnloadData(FileData* data);

#endif //LOADER_H
