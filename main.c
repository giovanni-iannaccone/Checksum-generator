#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "./include/hash.h"

typedef struct {
    char *algorithm;
    char *fileName;
} args;

void hash(char string[], char algorithm[], char *output, unsigned int len) {

    if (strcmp(algorithm, "MD5") == 0)
        md5(string, output, len);

    else if(strcmp(algorithm, "SHA-256") == 0) 
        sha256(string, output, len);
    
    return;
}

void help() {
    printf("Program usage: \n\n");
    printf("%s -f [file] -a [algorithm]\t\thash the file\n", __FILE__);
    printf("%s --help | -h\t\t\t\tvisualize this message\n", __FILE__);

    printf("\nSupported algorithms are MD5, SHA-256");
    return;
}

args parseArguments(int argc, char *argv[]) {
    args arguments;

    for(unsigned int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-f") == 0)
            arguments.fileName = argv[i + 1];
        else if(strcmp(argv[i], "-a") == 0)
            arguments.algorithm = argv[i + 1];

    return arguments;
}

void printHash(uint8_t *p, unsigned int size) {
    for(unsigned int i = 0; i < size; ++i)
        printf("%02x", p[i]);
}

int readFile(char fileName[], char *string, long int *fileSize) {
    FILE* fd;

    fd = fopen(fileName, "r");
    if (fd == NULL)
        return 1;

    fseek(fd, 0, SEEK_END);
    *fileSize = ftell(fd);
    rewind(fd);
    
    string = (char*)malloc(*fileSize + 1);
    fread(string, *fileSize, 1, fd);

    fclose(fd);
    return 0;
}

int main(int argc, char* argv[]) {
    int error;
    long int fileSize;

    char *fileContents;
    uint8_t *hashOutput;
    args arguments; 

    if (argc != 5 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        help();
        return 1;
    }

    arguments = parseArguments(argc, argv);

    error = readFile(arguments.fileName, fileContents, &fileSize);
    if (error) {
        printf("Error: couldn't open %s", arguments.fileName);
        return 1;
    }

    printf("Hashing %s with %s...\n\n", arguments.fileName, arguments.algorithm);
    hash(fileContents, arguments.algorithm, hashOutput, fileSize);
    
    printHash(hashOutput, strlen(hashOutput));

    printf("\n\n");
    return 0;
}