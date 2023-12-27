#define FUSE_USE_VERSION 31

#include <fuse3/fuse_lowlevel.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

struct fuse_session *se;

static void my_fuse3_ll_init(void *userdata, struct fuse_conn_info *conn)
{
        /*
         * Capability bits for 'fuse_conn_info.capable' defined fuse_common.h
         */

        (void) userdata;

        printf("--> Protocol version: %d.%d\n", conn->proto_major, conn->proto_minor);
        printf("Capabilities:\n");
        if(conn->capable & FUSE_CAP_ASYNC_READ)
                        printf("\tFUSE_CAP_ASYNC_READ\n");
        if(conn->capable & FUSE_CAP_POSIX_LOCKS)
                        printf("\tFUSE_CAP_POSIX_LOCKS\n");
        if(conn->capable & FUSE_CAP_ATOMIC_O_TRUNC)
                        printf("\tFUSE_CAP_ATOMIC_O_TRUNC\n");
        if(conn->capable & FUSE_CAP_EXPORT_SUPPORT)
                        printf("\tFUSE_CAP_EXPORT_SUPPORT\n");
        if(conn->capable & FUSE_CAP_DONT_MASK)
                        printf("\tFUSE_CAP_DONT_MASK\n");
        if(conn->capable & FUSE_CAP_SPLICE_MOVE)
                        printf("\tFUSE_CAP_SPLICE_MOVE\n");
        if(conn->capable & FUSE_CAP_SPLICE_READ)
                        printf("\tFUSE_CAP_SPLICE_READ\n");
        if(conn->capable & FUSE_CAP_SPLICE_WRITE)
                        printf("\tFUSE_CAP_SPLICE_WRITE\n");
        if(conn->capable & FUSE_CAP_FLOCK_LOCKS)
                        printf("\tFUSE_CAP_FLOCK_LOCKS\n");
        if(conn->capable & FUSE_CAP_IOCTL_DIR)
                        printf("\tFUSE_CAP_IOCTL_DIR\n");
        if(conn->capable & FUSE_CAP_AUTO_INVAL_DATA)
                        printf("\tFUSE_CAP_AUTO_INVAL_DATA\n");
        if(conn->capable & FUSE_CAP_READDIRPLUS)
                        printf("\tFUSE_CAP_READDIRPLUS\n");
        if(conn->capable & FUSE_CAP_READDIRPLUS_AUTO)
                        printf("\tFUSE_CAP_READDIRPLUS_AUTO\n");
        if(conn->capable & FUSE_CAP_ASYNC_DIO)
                        printf("\tFUSE_CAP_ASYNC_DIO\n");
        if(conn->capable & FUSE_CAP_WRITEBACK_CACHE)
                        printf("\tFUSE_CAP_WRITEBACK_CACHE\n");
        if(conn->capable & FUSE_CAP_NO_OPEN_SUPPORT)
                        printf("\tFUSE_CAP_NO_OPEN_SUPPORT\n");
        if(conn->capable & FUSE_CAP_PARALLEL_DIROPS)
                        printf("\tFUSE_CAP_PARALLEL_DIROPS\n");
        if(conn->capable & FUSE_CAP_POSIX_ACL)
                        printf("\tFUSE_CAP_POSIX_ACL\n");
        if(conn->capable & FUSE_CAP_CACHE_SYMLINKS)
                        printf("\tFUSE_CAP_CACHE_SYMLINKS\n");
        if(conn->capable & FUSE_CAP_NO_OPENDIR_SUPPORT)
                        printf("\tFUSE_CAP_NO_OPENDIR_SUPPORT\n");
        if(conn->capable & FUSE_CAP_EXPLICIT_INVAL_DATA)
                        printf("\tFUSE_CAP_EXPLICIT_INVAL_DATA\n");
        fuse_session_exit(se);
}


static const struct fuse_lowlevel_ops my_fuse3_ll_oper = {
        .init           = my_fuse3_ll_init,
};

static void show_usage(const char *progname)
{
    printf("usage: %s [options] <mountpoint>\n\n", progname);
    printf("options:\n"
           "      --version \tdisplay version information\n"
           "      -V\n"
           "      --help \t\tdisplay help information\n"
           "      --ho\n"
           "\n");
}

int main(int argc, char **argv)
{
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    struct fuse_cmdline_opts opts;
    int ret = -1;

    if (fuse_parse_cmdline(&args, &opts) != 0) {
        show_usage(argv[0]);
        ret = 1;
        return ret;
    }
    printf("--> Step 1 : fuse_parse_cmdline() passed\n");

    /*
     *  Low Level library has :
     *  2 help functions and 2 version functions
     *      fuse_cmdline_help(void)         <-- Print available options for fuse_parse_cmdline()
     *      fuse_lowlevel_help(void)        <-- Print available low-level options to stdout.
     *      fuse_lowlevel_version(void)     <-- Print low-level version information to stdout.
     *      fuse_pkgversion()               <-- Get the full package version string of the library
     */

    ret = 0;
    if (opts.show_help) {
        printf("usage: %s [options] <mountpoint>\n\n", argv[0]);
        printf("Command Line Help fuse_cmdline_help()\n");
        fuse_cmdline_help();
        printf("Low Level Help fuse_lowlevel_help()\n");
        fuse_lowlevel_help();
        return ret;
    } else if (opts.show_version) {
        printf("FUSE library version : %s\n", fuse_pkgversion());
        fuse_lowlevel_version();
        return ret;
    }

    if(opts.mountpoint == NULL) {
        // show_usage(argv[0]);
        printf("opts.mountpoint is NULL : %s\n", opts.mountpoint);
        return ret;
    }
    printf("--> Step 2 : opts.mountpoint is not NULL : %s\n", opts.mountpoint);

    se = fuse_session_new(&args, &my_fuse3_ll_oper, sizeof(my_fuse3_ll_oper), NULL);
    if (se == NULL) {
        printf("fuse_session_new() fail\n");
        ret = -1;
        return ret;
    }
    printf("--> Step 3 : fuse_session_new() OK, fuse_session (%p)\n", se);

    if (fuse_set_signal_handlers(se) == 0) {
        printf("--> Step 4 : fuse_set_signal_handlers()  OK\n");
        if (fuse_session_mount(se, opts.mountpoint) == 0) {
            printf("--> Step 5 : fuse_session_mount()  OK\n");
            printf("             opts.foreground is : %d\n", opts.foreground);

            fuse_daemonize(opts.foreground);
            ret = fuse_session_loop(se);   // <-- Block until ctrl+c or fusermount -u

            fuse_session_unmount(se);
            printf("--> Step 5 : fuse_session_unmount()\n");
        }
        fuse_remove_signal_handlers(se);
        printf("--> Step 4 : fuse_remove_signal_handlers()\n");
    } else {
        printf("fuse_session_mount()  Failed\n");
    }

    fuse_session_destroy(se);
    free(opts.mountpoint);         // <-- Free mountpoint allocated memory
    fuse_opt_free_args(&args);     // <-- Free the contents of argument list

    return ret;
}
 
