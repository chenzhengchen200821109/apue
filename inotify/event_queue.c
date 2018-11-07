#include "event_queue.h"
#include <stdlib.h>
#include <stdint.h>
#include <sys/queue.h>
#include <assert.h>

TAILQ_HEAD(queue_head, queue_entry) head;
static struct queue_head* headp;
static int queue_num = -1;

/*
 * return 1 if empty queue otherwise 0.
 */
int queue_empty(queue_t q)
{
    assert(q == &head);
    return (queue_num == 0 ? 1 : 0);
}

queue_t queue_create()
{
    TAILQ_INIT(&head);
    queue_num = 0;
    return &head;
}

void queue_enqueue(queue_entry_t e, queue_t q)
{
    assert(q == &head);
    TAILQ_INSERT_TAIL(&head, e, entries);
    queue_num++;
}

queue_entry_t queue_dequeue(queue_t q)
{
    assert(q == &head);
    queue_entry_t save = head.tqh_first;
    TAILQ_REMOVE(&head, head.tqh_first, entries);
    queue_num--;
    return save;
}

void queue_destroy(queue_t q)
{
    assert(q == &head);
    while (head.tqh_first != NULL) {
        queue_entry_t save = head.tqh_first;
        TAILQ_REMOVE(&head, head.tqh_first, entries);
        free(save->udata);
        free(save);
    }
    queue_num = -1;
    headp = 0;
}
