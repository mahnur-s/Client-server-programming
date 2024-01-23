#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define True 1   // Return Value 
#define False -1 // Return Value

/* Function prototypes */
void ls();
void changedir(const char* name);
int upload(int sock, const char* file_name);
int download(int sock, const char* file_name);
int init(int argc, char *argv[]);
void terminate(int sock);
