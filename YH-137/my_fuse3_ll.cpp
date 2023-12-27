#define FUSE_USE_VERSION 34

#include <fuse3/fuse_lowlevel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

struct fuse_session *se;

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

static void my_fuse3_ll_getattr (fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi) {
    (void) fi;
    struct stat stbuf;
    memset(&stbuf, '\0', sizeof(stbuf));
    stbuf.st_ino     = ino;
    stbuf.st_mode    = S_IFDIR | 0755;
    stbuf.st_nlink   = 1;
    stbuf.st_size    = 2048;
    stbuf.st_uid     = getuid();
    stbuf.st_gid     = getgid();
    stbuf.st_atime   = stbuf.st_mtime = time(NULL);
    fuse_reply_attr(req, &stbuf, 1.0);
}

static void my_fuse3_ll_init (void *userdata, struct fuse_conn_info *conn) {
    (void) userdata;
    printf("From function my_fuse3_ll_init() \n");
    printf("--> Protocol version : %d.%d\n", conn->proto_major, conn->proto_minor);
}

static const fuse_lowlevel_ops  my_fuse3_ll_oper = {
    .init = my_fuse3_ll_init,
    .getattr = my_fuse3_ll_getattr,
};

int main(int argc, char *argv[])
{
    /*
     *  usage: %s [options] <mountpoint>
     *
     *  Libfuse program pattern:
     *    1) #include <fuse3/fuse_lowlevel.h>
     *    2) Initiallize fuse_args          <-- struct fuse_args and struct fuse_cmdline_opts
     *                   fuse_cmdline_opts      are both used for fuse_parse_cmdline()
     *    3) parse command line opts        <-- call fuse_parse_cmdline(&args, &opts)
     *
     *  Low Level library has :
     *  2 help functions and 2 version functions
     *      fuse_cmdline_help(void)         <-- Print available options for fuse_parse_cmdline()
     *      fuse_lowlevel_help(void)        <-- Print available low-level options to stdout.
     *      fuse_lowlevel_version(void)     <-- Print low-level version information to stdout.
     *      fuse_pkgversion()               <-- Get the full package version string of the library
     */

    int ret = 0;
    struct fuse_args    args = FUSE_ARGS_INIT(argc, argv);
    struct fuse_cmdline_opts   opts;

    if ( fuse_parse_cmdline(&args, &opts) != 0 ) {
        show_usage(argv[0]);
        ret = -1;
        return ret;
    }

    if ( opts.show_help ) {
        printf("***** Command Line Help fuse_cmdline_help() *****\n");
        fuse_cmdline_help();
        printf("***** Low Level Help fuse_lowlevel_help() *****\n");
        fuse_lowlevel_help();
        return ret;
    } else if (opts.show_version ) {
        printf("FUSE library version : %s\n", fuse_pkgversion());
        fuse_lowlevel_version();
        return ret;
    }

    if ( opts.mountpoint == NULL ) {
        printf("opts.mountpoint is NULL : %s \n", opts.mountpoint);
        ret = 2;
        return ret;
    }
    printf("--> Step 1 : opts.mountpoint is not NULL : %s\n", opts.mountpoint);

    /*
     *    4) Initialize Low level
     *       filesystem operations          <-- Struct fuse_lowlevel_ops
     *    5) Initialize low level session.  <-- fuse_session_new()
     *    6) Set Signal Handlers            <-- fuse_set_signal_handlers(se)
     *    7) Session Mount File System      <-- fuse_session_mount()
     *    8) Enter single block event loop. <-- fuse_session_loop()
     *    9) Session Unmount File System    <-- fuse_session_unmount(se);
     *   10) terminate, destory session     <-- fuse_session_destroy() Destroy a session
     *                                          fuse_session_exit()    Flag session as terminated.
     */

    se =  fuse_session_new(&args, &my_fuse3_ll_oper, sizeof(my_fuse3_ll_oper), NULL);
    if ( se == NULL ) {
        printf("fuse_session_new() failed\n");
        ret = -1;
        return ret;
    }
    printf("--> Step 2 : fuse_session_new() SUccess, fuse_session (%p) \n", se);
    if ( fuse_set_signal_handlers(se) == 0 ) {
        // TODO
        printf("--> Step 3 : fuse_set_signal_handlers() OK\n");
        if ( fuse_session_mount(se, opts.mountpoint) == 0) {
            printf("--> Step 4 : fuse_session_mount() Success\n");
            fuse_daemonize(opts.foreground);
            ret = fuse_session_loop(se); // <-- Block untill ctrl+c or fusermount -u
            fuse_session_unmount(se);
            printf("--> Step 5 : fuse_session_unmount() \n");
        }

        fuse_remove_signal_handlers(se);
        printf("--> Step 6 : fuse_remove_signal_handlers() \n");
    }


    fuse_session_destroy(se);
    free(opts.mountpoint);          // <-- Free mountpoint allocated memory
    fuse_opt_free_args(&args);      // <-- Free the contents of argument list
    return ret;
}
