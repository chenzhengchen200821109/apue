#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_

#include <sys/inotify.h>
#include <sys/queue.h>

struct queue_data
{
    // for debug
    int value;
    struct inotify_event inot_ev;
};

struct queue_entry
{
    struct queue_data* udata;              // user defined data
    TAILQ_ENTRY(queue_entry) entries;      // <sys/queue.h> defined data
};

typedef struct queue_entry* queue_entry_t;
typedef struct queue_head* queue_t;

int queue_empty(queue_t q);
queue_t queue_create();
void queue_enqueue(queue_entry_t e, queue_t q);
queue_entry_t queue_dequeue(queue_t q);
void queue_destroy(queue_t q);

#endif
