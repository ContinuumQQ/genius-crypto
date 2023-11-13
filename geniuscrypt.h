#ifndef CRYPT_H
#define CRYPT_H

#define DEFAULT_BLOCK_SIZE              512;

struct arguments
{
        char                    filePath[255];
        char                    chose[2];
        unsigned short          help;
        short                   cryptShift;
        unsigned int            blockSize;
};

void start(struct arguments arguments);

void showHelp();
struct arguments argumentsParse(int argc, char ** argv);
short argumentsValidation(struct arguments *arguments);

void fileWriteCrypt(FILE *file, short shift, int bsize);
void changeBlock(char * block, short shift, int bsize);
char * readBlock(FILE *file, int bsize);
FILE * openFile(char filename[]);

long getFileSize(FILE *file);
void fileEncrypt(FILE *file, short shift, int bsize);
void fileDecrypt(FILE *file, short shift, int bsize);

#endif