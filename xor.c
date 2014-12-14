#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _LARGEFILE64_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE (64 * 1024)

void usage(char **argv)
{
    printf("Usage: %s KEY [INPUT] [OUTPUT]\n", argv[0]);
    exit(1);
}

int main(int argc, char **argv)
{
    if(argc < 2) usage(argv);

    char *key = argv[1];
    int keylen = strlen(key);

    int inp = argc > 2 ? open(argv[2], O_RDONLY) : STDIN_FILENO;
    if(inp < 0) return 2;

    int out = argc > 3 ? open(argv[3], O_WRONLY | O_CREAT, S_IREAD | S_IWRITE) : STDOUT_FILENO;
    if(inp < 0) return 3;

    char *buf = malloc(BUFFERSIZE);
    if(buf == NULL) return 4;

    ssize_t size;
    while((size = read(inp, buf, BUFFERSIZE)) > 0)
    {
        for(ssize_t i=0; i<size; ++i)
            buf[i] ^= key[i % keylen];
        write(out, buf, size);
    }

    free(buf);
    close(inp);
    close(out);
    return 0;
}

