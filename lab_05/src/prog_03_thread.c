#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>

void *thread_run(void *fs)
{
    struct stat info;

    for (char c = 'a'; c <= 'z'; c += 2)
    {
        fprintf(fs, "%c", c);
    }
    fclose(fs);
    stat("data/out.txt", &info);
    fprintf(stdout, "fclose data/out.txt for fd2: inode is %ld, buffsize is %ld, blocksize is %ld\n", info.st_ino, info.st_size, info.st_blksize);

    return NULL;
}

int main()
{
    struct stat info;

    FILE *fd1 = fopen("data/out.txt", "w");
    stat("data/out.txt", &info);
    fprintf(stdout, "fopen #1 data/out.txt for fd1: inode is %ld, buffsize is %ld, blocksize is %ld\n", info.st_ino, info.st_size, info.st_blksize);

    FILE *fd2 = fopen("data/out.txt", "w");
    stat("data/out.txt", &info);
    fprintf(stdout, "fopen #2 data/out.txt for fd1: inode is %ld, buffsize is %ld, blocksize is %ld\n", info.st_ino, info.st_size, info.st_blksize);

    pthread_t td;
    pthread_create(&td, NULL, thread_run, fd2);

    for (char c = 'b'; c <= 'z'; c += 2)
    {
        fprintf(fd1, "%c", c);
    }
    fclose(fd1);
    stat("data/out.txt", &info);
    fprintf(stdout, "fclose data/out.txt for fd1: inode is %ld, filesize is %ld, blocksize is %ld\n", info.st_ino, info.st_size, info.st_blksize);
    pthread_join(td, NULL);

    return 0;
}