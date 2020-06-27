#include "loader.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "logger.h"

//==============================================//
#define FILE_PATH "D:/_Programming_Projects/C/AsciiLand_Rewrite/res/"
#define FILE_EXT ".ascdat"
//==============================================//
#define FILE_END_INDICATOR "END"
#define FILE_STRING_INDICATOR "STR"
#define FILE_INT_INDICATOR "INT"
#define FILE_VEC_INDICATOR "V2"
//==============================================//

void load_getFullFilePath(char *reciever, const char *name)
{
    char buf[128];
    strcpy(buf, FILE_PATH);
    strcat(buf, name);
    strcat(buf, FILE_EXT);
    strcpy(reciever, buf);
}

void *Loader_ReloadFromFile(FileData *data, const char *fileName)
{
    Loader_UnloadData(data);
    data = Loader_LoadFromFile(fileName);
}

FileData *Loader_LoadFromFile(const char *fileName)
{
    FileData* data = Loader_ReadFile(fileName);
    data->file_name = fileName;
    if(data) Loader_ParseFileContents(data);
    return data;
}

FileData *Loader_ReadFile(const char *fileName)
{
    char full_file_path[50];
    load_getFullFilePath(full_file_path, fileName);
    
    FILE *f = fopen(full_file_path, "rb");
    
    char *buf = 0;
    long length;
    
    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        
        fseek(f, 0, SEEK_SET);
        buf = malloc(length + 1);
        
        if (buf)
        {
            fread(buf, 1, length, f);
            *(buf+length) = 0;
        }
        
        fclose(f);
    }
    
    if (buf)
    {
        FileData *data = malloc(sizeof(FileData));
        memset(data, 0, sizeof(FileData));
        
        data->file_contents = buf;
        return data;
    }
    
    
    return 0;
}


static char *Loader_ReturnValue(char **pbuf, const char *ind) {
    
    char *pp = *pbuf;
    pp = strstr(pp, ind);
    
    if(!pp) {
        return 0;
    }
    
    pp += strlen(ind);
    while(*pp == '\n' || *pp == '\r') pp++;
    
    char *buf = pp;
    pp = strstr(pp, FILE_END_INDICATOR);
    
    if(!pp) {
        Logger_LogMessage("FileLoadError", "END not found");
        exit(1);
    }
    
    *pp = 0;
    pp += strlen(FILE_END_INDICATOR);
    
    *pbuf = pp;
    return buf;
}

void Loader_ParseFileContents(FileData *data)
{
    char *pp = data->file_contents;
    char *buf;
    
    // Int
    while (buf = Loader_ReturnValue(&pp, FILE_INT_INDICATOR))
    {
        data->ints[data->numints++] = atoi(buf);
        *(buf+strlen(buf)) = FILE_END_INDICATOR[0];
    }
    
    // String
    pp = data->file_contents;
    while (buf = Loader_ReturnValue(&pp, FILE_STRING_INDICATOR))
    {
        data->strings[data->numstr++] = buf;
        *(buf+strlen(buf)) = FILE_END_INDICATOR[0];
    }
    
    // Vec2
    pp = data->file_contents;
    while (buf = Loader_ReturnValue(&pp, FILE_VEC_INDICATOR))
    {
        int x, y;
        sscanf(buf, "%d %d", &x, &y);
        
        data->vecs[data->numvecs].x = x;
        data->vecs[data->numvecs].y = y;
        
        data->numvecs++;
        *(buf+strlen(buf)) = FILE_END_INDICATOR[0];
    }
    
    return;
}

void Loader_UnloadData(FileData *data)
{
    if (!data) return;
    if (data->file_contents) free(data->file_contents);
    free(data);
}
