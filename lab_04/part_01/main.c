#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <dirent.h>

#include "common.h"

#define BUF_SIZE 0x100

void fprint_environ(const pid_t pid, FILE *f_out)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/environ", pid);
	FILE *f = fopen(path, "r");
	fprintf(f_out, "\n========ENVIRON========\n");

	int len;
	char buf[BUF_SIZE];

	while ((len = fread(buf, 1, sizeof(buf), f)) > 0)
	{
		for (int i = 0; i < len; i++)
        {
            if (buf[i] == 0)
            {
                buf[i] = 10;
            }
        }
		buf[len] = 10;
		fprintf(f_out, "%s", buf);
	}

	fclose(f);
}

void fprint_stat(const pid_t pid, FILE *f_out)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/stat", pid);
	FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========STAT========\n");

    char buf[BUF_SIZE];
    fread(buf, 1, BUF_SIZE, f);

    char *token = strtok(buf, " ");
    for (int i = 0; token != NULL; i++)
    {
        fprintf(f_out, WITH_DESCR[i], token);
        token = strtok(NULL, " \n");
    }

    fclose(f);
}

void fprint_cmdline(const pid_t pid, FILE *f_out)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/cmdline", pid);
	FILE *f = fopen(path, "r");
	fprintf(f_out, "\n========CMDLINE========\n");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, f);
    buf[len - 1] = 0;

    fprintf(f_out, "%s\n", buf);

    fclose(f);
}

void fprint_fd(const pid_t pid, FILE *f_out)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/fd", pid);
	//FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========FD========\n");

    DIR *dir = opendir(path);
    struct dirent *readDir;
    char string[PATH_MAX];
    char cur_path[BUF_SIZE] = {'\0'};
    while ((readDir = readdir(dir)) != NULL)
    {
        if ((strcmp(readDir->d_name, ".") != 0) && (strcmp(readDir->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", cur_path, readDir->d_name);
            readlink(cur_path, string, PATH_MAX);
            fprintf(f_out, "%s -> %s\n", readDir->d_name, string);
        }
    }

    closedir(dir);
}

void fprint_cwd(const pid_t pid, FILE *f_out)
{
    char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/cwd", pid);
	//FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========CWD========\n");
	
    char buf[BUF_SIZE] = {'\0'};
    readlink(path, buf, BUF_SIZE);
    fprintf(f_out, "%s\n", buf);
}

void fprint_exe(const pid_t pid, FILE *f_out)
{
    char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/exe", pid);
	//FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========EXE========\n");

    char buf[BUF_SIZE] = {'\0'};
    readlink(path, buf, BUF_SIZE);
    fprintf(f_out, "%s\n", buf);
}

void fprint_root(const pid_t pid, FILE *f_out)
{
    char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/root", pid);
	//FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========ROOT========\n");
	
    char buf[BUF_SIZE] = {'\0'};
    readlink(path, buf, BUF_SIZE);
    fprintf(f_out, "%s\n", buf);
}

void fprint_maps(const pid_t pid, FILE *f_out)
{
    char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/maps", pid);
	FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========MAPS========\n");

    char buf[BUF_SIZE] = {'\0'};
    int len;
    while ((len = fread(buf, 1, BUF_SIZE, f)))
    {
        buf[len] = '\0';
        fprintf(f_out, "%s\n", buf);
    }
    fclose(f);
}

void fprint_pagemap(const pid_t pid, FILE *f_out)
{
    char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/pagemap", pid);
	FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========PAGEMAP========\n");

    char buf[BUF_SIZE] = {'\0'};
    int len;
    while ((len = fread(buf, 1, BUF_SIZE, f)))
    {
        buf[len] = '\0';
        fprintf(f_out, "%s\n", buf);
    }
    fclose(f);
}

void fprint_comm(const pid_t pid, FILE *f_out)
{
    char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/comm", pid);
	FILE *f = fopen(path, "r");
    fprintf(f_out, "\n========COMM========\n");

    char buf[BUF_SIZE];
    int len = fread(buf, 1, BUF_SIZE, f);
    buf[len - 1] = 0;
    fprintf(f_out, "%s\n", buf);

    fclose(f);
}

void fprint_io(const pid_t pid, FILE *f_out)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/io", pid);
	FILE *f = fopen(path, "r");
	fprintf(f_out, "\n========I/O========\n");

	int len;
	char buf[BUF_SIZE];

	while ((len = fread(buf, 1, sizeof(buf), f)) > 0)
	{
		for (int i = 0; i < len; i++)
        {
            if (buf[i] == 0)
            {
                buf[i] = 10;
            }
        }
		buf[len] = 10;
		fprintf(f_out, "%s", buf);
	}

	fclose(f);
}

void fprint_task(const pid_t pid, FILE *f_out)
{
	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%d/task", pid);
	fprintf(f_out, "\n========TASK========\n");

    DIR *d;
    struct dirent *dir;

    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            fprintf(f_out, "%s\n", dir->d_name);
        }
        closedir(d);
    }
}

int main(int argc, char *argv[])
{
	if (argc != 3 && argc != 2) 
	{
		printf("Usage: %s [pid] [output_filename]\n", argv[0]);
		return EXIT_FAILURE;
	}

	pid_t pid;
	FILE *f_out;

	if (argc == 2)
	{
		pid = getpid();
		f_out = fopen(argv[1], "w");
	}
	else
	{
		pid = atoi(argv[1]);
		f_out = fopen(argv[2], "w");
	}

	fprint_environ(pid, f_out);
	fprint_stat(pid, f_out);
	fprint_cmdline(pid, f_out);
	fprint_fd(pid, f_out);
	fprint_cwd(pid, f_out);
	fprint_exe(pid, f_out);
	fprint_root(pid, f_out);
	fprint_maps(pid, f_out);
	fprint_comm(pid, f_out);
    fprint_io(pid, f_out);
    fprint_task(pid, f_out);
    //fprint_pagemap(pid, f_out);

	fclose(f_out);
}