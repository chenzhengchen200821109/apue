#include "apue.h"
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/un.h>

#define MAXSLEEP 128
#define QLEN 10
#define STALE 30 /* client's name can't be older than this (sec) */
#define CLI_PATH "/var/tmp"
#define CLI_PERM S_IRWXU /* rwx for user only */

int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;

    /* Try to connect with exponential backoff */
    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) {
        if ((fd = socket(domain, type, protocol)) < 0)
            return (-1);
        if (connect(fd, addr, alen) == 0) {
            /* connection accepted */
            return (fd);
        }
        close(fd);

        /* delay before trying again */
        if (numsec <= MAXSLEEP/2)
            sleep(numsec);
    }
    return (-1);
}

/* 
 * This implementation is incomplete. 
 */
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;
    int err = 0;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return (-1);
    if (bind(fd, addr, alen) < 0)
        goto errout;
    if (type == SOCK_STREAM || type == SOCK_SEQPACKET) {
        if (listen(fd, qlen) < 0)
            goto errout;
    }
    return (fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return(-1);
}

/* turn on close-on-exec flag */
int set_cloexec(int fd)
{
    int val;

    if ((val = fcntl(fd, F_GETFD, 0)) < 0)
        return (-1);

    val |= FD_CLOEXEC; /* enable close-on-exec */

    return (fcntl(fd, F_SETFD, val));
}

/*
 * Create a server endpoint of a connection.
 * Returns fd if all OK, <0 on error.
 */
int serv_listen(const char *name)
{
    int fd, len, err, rval;
    struct sockaddr_un un;

    if (strlen(name) >= sizeof(un.sun_path)) {
        errno = ENAMETOOLONG;
        return (-1);
    }

    /* Create a UNIX domain stream socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-2);

    unlink(name); /* in case it already exists */

    /* fill in socket address structure */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    strcpy(un.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);

    /* bind the name to the descriptor */
    if (bind(fd, (struct sockaddr *)&un, len) < 0) {
        rval = -3;
        goto errout;
    }

    if (listen(fd, QLEN) < 0) { /* tell kernel we're a server */
        rval = -4;
        goto errout;
    }

    return (fd);

errout:
    err = errno;
    close(fd);
    errno = err;
    return (rval);
}

/*
 * Wait for a client connection to arrive, and accept it.
 * We also obtain the client's user ID from the pathname that
 * it must bind before calling us.
 * Returns new fd if all OK, <0 on error.
 */
int serv_accept(int listenfd, uid_t *uidptr)
{
    int clifd, err, rval;
    socklen_t len;
    time_t staletime;
    struct sockaddr_un un;
    struct stat statbuf;
    char *name;

    /* allocate enough space for longer name plus terminating null */
    if ((name = malloc(sizeof(un.sun_path) + 1)) == NULL)
        return (-1);
    len = sizeof(un);
    if ((clifd = accept(listenfd, (struct sockaddr *)&un, &len)) < 0) {
        free(name);
        return (-2); /* often errno = EINTR, if signal caught */
    }

    /* obtain the client's uid from its calling address */
    len -= offsetof(struct sockaddr_un, sun_path); /* length of pathname */
    memcpy(name, un.sun_path, len);
    name[len] = 0; /* null terminate */
    if (stat(name, &statbuf) < 0) {
        rval = -3;
        goto errout;
    }

#ifdef S_ISSOCK /* not defined for SVR4 */
    if (S_ISSOCK(statbuf.st_mode) == 0) {
        rval = -4; /* not a socket */
        goto errout;
    }
#endif

    if ((statbuf.st_mode & (S_IRWXG | S_IRWXO)) || (statbuf.st_mode & S_IRWXU) != S_IRWXU) {
        rval = -5; /* is not rwx------ */
        goto errout;
    }

   staletime = time(NULL) - STALE;
   if (statbuf.st_atime < staletime || statbuf.st_ctime < staletime || statbuf.st_mtime < staletime) {
       rval = -6; /* i-node is too old */
       goto errout;
   }

   if (uidptr != NULL)
       *uidptr = statbuf.st_uid; /* return uid of caller */
   unlink(name);
   free(name);
   return (clifd);

errout:
   err = errno;
   close(clifd);
   free(name);
   errno = err;
   return (rval);
}

/*
 * Create a client endpoint and connect to a server.
 * Returns fd if all OK, <0 on error.
 */
int cli_conn(const char *name)
{
    int fd, len, err, rval;
    struct sockaddr_un un, sun;
    int do_unlink = 0;

    if (strlen(name) >= sizeof(un.sun_path)) {
        errno = ENAMETOOLONG;
        return (-1);
    }

    /* create a UNIX domain stream socket */
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        return (-1);

    /* fill socket address structure with our address */
    memset(&un, 0, sizeof(un));
    un.sun_family = AF_UNIX;
    sprintf(un.sun_path, "%s%05ld", CLI_PATH, (long)getpid());
    len = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);

    unlink(un.sun_path); /* in case it already exists */
    if (bind(fd, (struct sockaddr *)&un, len) < 0) {
        rval = -2;
        goto errout;
    }

    if (chmod(un.sun_path, CLI_PERM) < 0) {
        rval = -3;
        do_unlink = 1;
        goto errout;
    }

    /* fill socket address structure with server's address */
    memset(&sun, 0, sizeof(sun));
    sun.sun_family = AF_UNIX;
    strcpy(sun.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if (connect(fd, (struct sockaddr *)&sun, len) < 0) {
        rval = -4;
        do_unlink = 1;
        goto errout;
    }

    return (fd);

errout:
    err = errno;
    close(fd);
    if (do_unlink)
        unlink(un.sun_path);
    errno = err;
    return (rval);
}



