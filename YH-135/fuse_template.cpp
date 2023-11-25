#define FUSE_USE_VERSION 31

#include <fuse/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>

/*
 *  Operation callback functions
 */

static void *my_fuse_init(struct fuse_conn_info *conn)
{
    return NULL;
}

static int my_fuse_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    return res;
}

static int my_fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
             off_t offset, struct fuse_file_info *fi)
{
    return 0;
}

static int my_fuse_open(const char *path, struct fuse_file_info *fi)
{
    return 0;
}

static int my_fuse_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi)
{
    return size;
}

static const struct fuse_operations my_fuse_oper = {
    .getattr    = my_fuse_getattr,
    .open       = my_fuse_open,
    .read       = my_fuse_read,
    .readdir    = my_fuse_readdir,
    .init       = my_fuse_init,
};

static void show_help(const char *progname)
{
    printf("usage: %s [options]\n\n", progname);
    printf("options:\n"
           "      --version\n"
           "      --help\n"
           "      --ho\n"
           "\n");
}

int main(int argc, char *argv[])
{
    /*
     *   Libfuse API documentation
     *   https://libfuse.github.io/doxygen/index.html
     */

    int ret;
    show_help(argv[0]);

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    ret = fuse_main(args.argc, args.argv, &my_fuse_oper, NULL);
    fuse_opt_free_args(&args);
    return ret;
}
