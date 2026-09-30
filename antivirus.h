#ifndef ANTIVIRUS_H
#define ANTIVIRUS_H


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
#include <time.h>
#include <fcntl.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

// Constants 
#define BlockSize       50000
#define Version         "0.1"
#define BufSize         32

// for typecasting
#define $8 (int8*)
#define $c (char *)
#define $16 (int16)
#define $32 (int32)
#define $64 (int64)
#define $i (int)


#define log(f,args) printf(f,args); fflush(stdout)
#define a2h(x)      (((x) > 0x2f) && ((x) < 0x3a)) ? \
    ((x) - 0x30) : \
        (((x) > 0x60) && ((x) < 0x67)) ? \
        ((x) - 0x57) : \
        0

#define linux_dirent dirent 


// All the data structures we're going to use 

typedef int8 Dir[256];
typedef int8 File[64];
typedef unsigned long long int Timestamp;

typedef enum e_filetype{
    file    = 1,
    dir     = 2,
    other   = 3
}Filetype;


typedef enum e_bufstate{
    idle = 0,
    newline = 1,
    space = 2 
    
}Bufstate;

typedef struct s_buffer{
    int32 fd;
    Bufstate state;
    int32 filepos;
    int8 *bufpos; 
    int8 *start;
    int8 *end;
    int8 *eol;
    bool eof;
    int8 buf[BufSize];
}Buffer;

// A data structure to track the state of binary files 
typedef enum e_state{
    unstaged = 0,
    unscanned = 1 ,
    scanning = 2,
    infected = 3,
    healed = 4

}estate;

typedef struct s_state{
    estate stage;
    int8 virus[32];
}State;




//A struct to store the directory and a file pointer
typedef struct s_entry{
    Filetype type;
    Dir dir;
    File file;
    Timestamp lastscanned;
    State state;
}Entry;

typedef struct s_database{
    Entry *entries;
    int32 capacity; // How many entries can we store 
    int32 num ; // How many enries we have 
}Database;


typedef bool (*function)(Entry);

// Constructors for our data structures

State mkstate(void);
// A function to create a database. It takes  an int16 initial capacity
Database *mkdatabase(void);

// Scans the database for virus and returns a database pointer
Database *scan(Database*,int32);

int8 *parsehex(int8*);
int8 ascii2hex(int8*);
//This function prepares for the creation aof a database 
Database* prepare(void);
Timestamp unixtime(void);
Database *filter(Database*,function);
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


#endif
