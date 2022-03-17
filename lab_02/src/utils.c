 #include "utils.h"

void log_entry(const char *const entry_name, size_t offset, ino_t inode)
{
    fprintf(stdout, " ");

    for (size_t i = 0; i < offset; i++)
    {
        fprintf(stdout, " ---> " );
    }

    fprintf(stdout, "%s (inode = %lu)\n", entry_name, inode);
}
