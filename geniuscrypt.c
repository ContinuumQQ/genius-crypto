#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "geniuscrypt.h"
#include <sys/stat.h>

int main(int argc, char **argv)
{
        struct arguments arg = argumentsParse(argc, argv);

        start(arg);

        return 0;
}


FILE * openFile(char filename[])
{
        FILE *openFile = fopen(filename, "rb+");
        if (openFile == NULL){
                perror("Open File");
                exit(EXIT_FAILURE);
        }
        else 
                return openFile;

}


long getFileSize(FILE *file)
{
        long size = 0;

        fseek(file, 0, SEEK_END);
        
        size = ftell(file);

        rewind(file);

        return size;
}


char * readBlock(FILE *file, int bsize)
{
        char *block = (char *)malloc(sizeof(char) * bsize);

        fread(block, bsize, 1, file);

        return block;
}


void fileWriteCrypt(FILE *file, short shift, int bsize)
{
        char * block = NULL;
        long fsize = getFileSize(file);
        long curPos = 0L;

        while (1){
                curPos = ftell(file);
                block = readBlock(file, bsize);

                if (fsize - curPos > bsize){
                        changeBlock(block, shift, bsize);
                        fseek(file, -bsize, SEEK_CUR);
                        fwrite(block, bsize, 1, file);
                } 
                else{
                        changeBlock(block, shift, fsize - curPos);
                        fseek(file, -1 * (fsize - curPos), SEEK_CUR);
                        fwrite(block, fsize - curPos, 1, file);
                        break;
                }
        }

        free(block);
}


void changeBlock(char * block, short shift, int bsize)
{
        int i = 0;

        for (i = 0; i < bsize; i++){
                block[i] += shift;
        }
}


struct arguments argumentsParse(int argc, char ** argv)
{
        int i = 0;
        struct arguments args;

        memset(&args, 0, sizeof(args));

        
                for (i = 1; i < argc; i++){
                        
                        if(strcmp("-p", argv[i]) == 0)
                                strcpy(args.filePath, argv[i + 1]);
                        
                        if(strcmp("-e", argv[i]) == 0 || strcmp("-d", argv[i]) == 0)
                                strcpy(args.chose, argv[i]);

                        if(strcmp("-s", argv[i]) == 0 && argv[i + 1] != NULL)
                                args.cryptShift = atoi(argv[i + 1]);
                        
                        if(strcmp("-b", argv[i]) == 0 && argv[i + 1] != NULL)
                                args.blockSize = atoi(argv[i + 1]);

                        if(strcmp("-h", argv[i]) == 0)
                                args.help = 1;
                }

        return args; 
}


short argumentsValidation(struct arguments *arguments){

        if(arguments -> help == 1)
                return 2;

        if (arguments -> blockSize < 1){
                arguments -> blockSize = DEFAULT_BLOCK_SIZE;
        }

        if (arguments -> cryptShift < 1){
                printf("[shift] cannot be equal 0 or above 127\n");
                return 0;
        }

        if(strcmp("-e", arguments -> chose) != 0 && strcmp("-d", arguments -> chose) != 0){
                printf("[key] unknown\n");
                return 0;
        }
                
        return 1;
}


void showHelp()
{
        printf("Usage: crypto [KEY]... \n");
        printf("Encrypts the file with [shift]\n");
        printf("\n");
        printf("[KEY]\n");
        printf("-p ...            filename or Path to file\n");
        printf("-d                decrypts the file \n");
        printf("-e                encrypts the file \n");
        printf("-s ...            shifts bytes in the file by this value\n");
        printf("-b ...            write a file with a block size (default: 512)\n");
        printf("-h                print help\n");
        printf("\n");
        printf("Example:\n");
        printf("crypto -p ~/path/example -e -s 10 -b 1024\n");
}


void start(struct arguments arguments)
{
        int valid = argumentsValidation(&arguments);

        if (valid == 2){
                showHelp();
                exit(EXIT_SUCCESS);
        }

        if (valid == 0){
                printf("Arguments error\n");
                exit(EXIT_FAILURE);
        }

        if (valid == 1){
                FILE *fileCrypt = openFile(arguments.filePath);
                
                if (strcmp("-e", arguments.chose) == 0){
                        fileWriteCrypt(fileCrypt, arguments.cryptShift, arguments.blockSize);
                        printf("file is crypted!\n");
                }
                else if (strcmp("-d", arguments.chose) == 0){
                        fileWriteCrypt(fileCrypt, -(arguments.cryptShift), arguments.blockSize);
                        printf("file is decrypted!\n");
                }
                fclose(fileCrypt);
        }
}