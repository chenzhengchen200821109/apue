#include "event_queue.h"
#include "inotify.h"
#include "../apue.h"
#include <limits.h>

#define EVENT_BUFFER_LEN ((sizeof(struct inotify_event) + NAME_MAX + 1) * 10)

extern int keep_running;
static int watched_items;

/* Create an inotify instance and open a file descriptor
   to access it */
int open_inotify_fd ()
{
    int fd;
    watched_items = 0;
    if ((fd = inotify_init ()) < 0)
        err_msg("inotify_init() failed");

    return fd;
}


/* Close the open file descriptor that was opened with inotify_init() */
int close_inotify_fd(int fd)
{
    int ret;

    if ((ret = close (fd)) < 0)
        err_msg("close() failed");
    watched_items = 0;

    return ret;
}

/*
 * This method does the work of determining what happened,
 * then allows us to act appropriately
 */
void handle_event(queue_entry_t event)
{
    /* If the event was associated with a filename, 
     * we will store it here 
     */
    char* cur_event_filename = NULL;
    char* cur_event_file_or_dir = NULL;
    /* This is the watch descriptor the event occurred on */
    int cur_event_wd = event->udata->inot_ev.wd;
    int cur_event_cookie = event->udata->inot_ev.cookie;
    uint32_t flags;

    if (event->udata->inot_ev.len)
        cur_event_filename = event->udata->inot_ev.name;
    if (event->udata->inot_ev.mask & IN_ISDIR)
        cur_event_file_or_dir = "Dir";
    else 
        cur_event_file_or_dir = "File";
    flags = event->udata->inot_ev.mask & ~(IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED );

    /* 
     * Perform event dependent handler routines.
     * The mask is the magic that tells us what file operation occurred 
     */
    switch (event->udata->inot_ev.mask & (IN_ALL_EVENTS | IN_UNMOUNT | IN_Q_OVERFLOW | IN_IGNORED)) {
        /* File was accessed */
        case IN_ACCESS:
            printf ("access %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* File was modified */
        case IN_MODIFY:
            printf ("modify %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* File changed attributes */
        case IN_ATTRIB:
            printf ("attribute change on %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* File open for writing was closed */
        case IN_CLOSE_WRITE:
            printf ("closed for writing %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* File open read-only was closed */
        case IN_CLOSE_NOWRITE:
            printf ("closed for reading only %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* File was opened */
        case IN_OPEN:
            printf ("open %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* File was moved from X */
        case IN_MOVED_FROM:
            printf ("moved from %s \"%s\" on watch descriptor #%i. Cookie=%d\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd, cur_event_cookie);
            break;

        /* File was moved to X */
        case IN_MOVED_TO:
            printf ("moved to %s \"%s\" on watch descriptor #%i. Cookie=%d\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd, cur_event_cookie);
            break;

        /* Subdir or file was deleted */
        case IN_DELETE:
            printf ("deleted %s \"%s\" on watch descriptor #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* Subdir or file was created */
        case IN_CREATE:
            printf ("CREATE: %s \"%s\" on WD #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* Watched entry was deleted */
        case IN_DELETE_SELF:
            printf ("DELETE_SELF: %s \"%s\" on WD #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* Watched entry was moved */
        case IN_MOVE_SELF:
            printf ("MOVE_SELF: %s \"%s\" on WD #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* Backing FS was unmounted */
        case IN_UNMOUNT:
            printf ("UNMOUNT: %s \"%s\" on WD #%i\n", cur_event_file_or_dir, cur_event_filename, cur_event_wd);
            break;

        /* 
         * Too many FS events were received without reading them some event notifications 
         * were potentially lost.
         */
        case IN_Q_OVERFLOW:
            printf ("Warning: AN OVERFLOW EVENT OCCURRED: \n");
            break;

        /* 
         * Watch was removed explicitly by inotify_rm_watch or automatically because file 
         * was deleted, or file system was unmounted.
         */
        case IN_IGNORED:
            watched_items--;
            printf ("IGNORED: WD #%d\n", cur_event_wd);
            printf("Watching = %d items\n",watched_items); 
            break;

        /* Some unknown message received */
        default:
            printf ("unknown event \"%ud\" occurred for file \"%s\" on watch descriptor #%i\n", event->udata->inot_ev.mask, cur_event_filename, cur_event_wd);
            break;
    }
    /* If any flags were set other than IN_ISDIR, report the flags */
    if (flags & (~IN_ISDIR)) {
        flags = event->udata->inot_ev.mask;
        printf ("Flags=%ud\n", flags);
    }
}

/*
 * handle_events() is used by consumers.
 */
void handle_events(queue_t q)
{
    queue_entry_t event;
    while (!queue_empty(q)) {
        event = queue_dequeue(q);
        handle_event(event);
        free(event);
        free(event->udata);
    }
}

/*
 * read_events() is used by producers.
 */
int read_events(queue_t q, int fd)
{
    //char buffer[16384];
    char buffer[EVENT_BUFFER_LEN + 1];
    size_t index;
    struct inotify_event *pevent;
    queue_entry_t event;
    ssize_t r;
    size_t event_size, q_event_size;
    int count = 0;

    if ((r = read(fd, buffer, 16384)) <= 0)
        return r; // read() fails
    else if(r == EVENT_BUFFER_LEN + 1)
        printf("\ntoo many events occurred, only ten of them handled\n");

    index = 0;
    q_event_size = sizeof(struct queue_entry);
    while(index < r) {
        /* Parse events and queue them. */
        pevent = (struct inotify_event *)&buffer[index];
        event_size =  offsetof(struct inotify_event, name) + pevent->len;
        event = malloc(q_event_size);
        event->udata = malloc(event_size);
        memmove(&(event->udata->inot_ev), pevent, event_size);
        queue_enqueue(event, q);
        index += event_size;
        count++;
    }
    printf ("\n%d events queued\n", count);

    return count;
}

int event_check (int fd)
{
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    /* 
     * Wait until an event happens or we get interrupted by a signal that we catch 
     */
    return select(FD_SETSIZE, &fds, NULL, NULL, NULL);
}

int process_inotify_events (queue_t q, int fd)
{
    while(keep_running && (watched_items > 0)) {
        if(event_check(fd) > 0) {
	        int ret;
	        if ((ret = read_events(q, fd)) < 0)
                break;
            else 
                handle_events(q);
        }
    }
    return 0;
}

int watch_directory(int fd, const char *dirname, uint32_t mask)
{
    int wd;
    if ((wd = inotify_add_watch(fd, dirname, mask)) < 0)
        err_msg("inotify_add_watch() failed");
    else {
        watched_items++;
        printf("watching %s watch descriptor %d\n", dirname, wd);
        printf("watching %d items\n", watched_items);
    }
    return wd;
}

int ignore_wd(int fd, int wd)
{
    int ret;
    if ((ret = inotify_rm_watch(fd, wd)) < 0)
        err_msg("inotify_rm_watch() failed");
    else
        watched_items--;
    return ret;
}
