#include "event_queue.h"
#include <stdlib.h>
#include <stdint.h>
#include <sys/queue.h>
#include <assert.h>

TAILQ_HEAD(queue_head, queue_entry) head;
struct queue_head* headp;

int queue_empty(queue_t q)
{
    assert(q == &head);
    return headp == 0;
}

queue_t queue_create()
{
    TAILQ_INIT(&head);
    return &head;
}

void queue_enqueue(queue_entry_t e, queue_t q)
{
    assert(q == &head);
    TAILQ_INSERT_TAIL(&head, e, entries);
}

queue_entry_t queue_dequeue(queue_t q)
{
    assert(q == &head);
    queue_entry_t save = head.tqh_first;
    TAILQ_REMOVE(&head, head.tqh_first, entries);
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
    headp = 0;
}
