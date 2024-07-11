#ifndef DISK_H
#define DISK_H

#include <stdio.h>

typedef struct{
    char* name;
    size_t size;
}disk;

disk physical_disks[4];
int current_physical_disk_size = 0;

void register_disk(char* disk){
    physical_disks[current_physical_disk_size++] = disk;
}


#endif