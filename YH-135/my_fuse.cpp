#define FUSE_USE_VERSION 31

#include <fuse/fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>

static void show_usage(const char *progname)
{
    printf("usage: %s [options] <mount point>\n\n", progname);
    printf("options:\n"
           "      --version \tdisplay version information\n"
           "      -V\n"
           "      --help \t\tdisplay help information\n"
           "      --ho\n"
           "\n");
}

static int my_fuse_getattr (const char *path, struct stat *stbuf)
{
    int res = 0;
    printf("--> my_fuse_getattr() invoked and path is : %s;\n", path);
    memset(stbuf, '\0', sizeof(struct stat));

    stbuf->st_mode   = S_IFDIR | 0755;
    stbuf->st_nlink  = 1;
    stbuf->st_size   = 2048;
    stbuf->st_uid    = getuid();
    stbuf->st_gid    = getgid();
    stbuf->st_atime  = stbuf->st_mtime = time(NULL);
    return res;
};

static void *my_fuse_init (struct fuse_conn_info *conn)
{
    (void) conn;
    printf("--> my_fuse_init() has been invoked;\n");
    return NULL;
};


static const struct fuse_operations my_fuse_oper = {
    .getattr     = my_fuse_getattr,
    .init        = my_fuse_init,
};

int main(int argc, char *argv[])
{
    int ret;
    if ( argc < 2 ) {
        show_usage(argv[0]);
        return 1;
    }

    /*
     *  Libfuse program pattern:
     *    1) #include <fuse/fuse.h>
     *    2) Initiallize args               <-- struct fuse_args is used for fuse_parse_cmdline()
     *    3) declare struct fuse_operations <-- struct fuse_operations is used for fuse_main()
     *    4) call and execute fuse_main()
     *    5) free fuse_args object allocated memory
     */

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    ret = fuse_main(args.argc, args.argv, &my_fuse_oper, NULL);
    fuse_opt_free_args(&args);
    return ret;
}
