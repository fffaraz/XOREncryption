#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define KEYSIZE (1024)
#define BUFSIZE (64 * 1024)

void usage(char **argv)
{
    fprintf(stderr, "Usage: %s KEY [INPUT] [OUTPUT]\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if(argc < 2) usage(argv);

    char *keyw = argv[1];
    int keylen = strlen(keyw);
    if(keylen > KEYSIZE) return 2;

    char key[KEYSIZE];
    for(int i = 0; i < KEYSIZE; i++)
        key[i] = keyw[i % keylen];

    int inp = argc > 2 ? open(argv[2], O_RDONLY) : STDIN_FILENO;
    if(inp < 0) return 3;

    int out = argc > 3 ? open(argv[3], O_WRONLY | O_CREAT, S_IREAD | S_IWRITE) : STDOUT_FILENO;
    if(out < 0) return 4;

    char *buf = malloc(BUFSIZE);
    if(buf == NULL) return 5;

    ssize_t size;
    while((size = read(inp, buf, BUFSIZE)) > 0)
    {
        for(int i = 0; i < size; i += KEYSIZE)
        {
            char *p = &buf[i];
            for(int j = 0; j < KEYSIZE; j += 8)
            {
                p[j + 0] ^= key[j + 0];
                p[j + 1] ^= key[j + 1];
                p[j + 2] ^= key[j + 2];
                p[j + 3] ^= key[j + 3];
                p[j + 4] ^= key[j + 4];
                p[j + 5] ^= key[j + 5];
                p[j + 6] ^= key[j + 6];
                p[j + 7] ^= key[j + 7];
            }
        }
        write(out, buf, size);
    }

    free(buf);
    close(inp);
    close(out);
    return 0;
}
