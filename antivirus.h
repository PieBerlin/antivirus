/*antivirus.h*/
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <pieutils.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <string.h>
#include <fcntl.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

// Constants 
#define BlockSize 50000

// for typecasting
#define $8 (int8*)
#define $c (char *)
#define $16 (int16)
#define $32 (int32)
#define $64 (int64)
#define $i (int)

#define linux_dirent dirent 


// All the data structures we're going to use 

typedef int8 Dir[64];
typedef int8 File[32];

typedef enum e_filetype{
    file    = 1,
    dir     = 2,
    other   = 3
}Filetype;

//A struct to store the directory and a file pointer
typedef struct s_entry{
    Filetype type;
    Dir dir;
    File file;
}Entry;

typedef struct s_database{
    Entry *entries;
    int32 capacity; // How many entries can we store 
    int32 num ; // How many enries we have 
}Database;


typedef bool (*function)(Entry);

Database *filter(Database*,function);
// A function to create a database. It takes  an int16 initial capacity
Database *mkdatabase(void);
bool adddir(Database *,int8*);
// This function tests if a file is an elf binary
bool iself(Entry);
// 
void addtodb(Database*,Entry);
// Function to destroy the Database and free space
void destroydb(Database*);
// Prints a database
void showdb(Database*);
int main(int, char **);
