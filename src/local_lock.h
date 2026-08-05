#ifndef LOCAL_LOCK_H
#define LOCAL_LOCK_H
#include <pthread.h>

typedef struct {
    volatile uint64_t next;
} mcsQueueMember;

typedef struct {
    pthread_mutex_t* lock_mutex;
    volatile uint64_t owner;
    volatile mcsQueueMember* queueEnd;
} mcsLock;

typedef struct {
    pthread_mutex_t* lock_mutex;
    volatile uint64_t owner;
    volatile uint64_t now;
    volatile uint64_t next;
} ticketLock;

typedef struct {
    pthread_mutex_t* lock_mutex;
    volatile uint64_t owner;
} spinLock;

void* lock(spinLock* lock, uint64_t node_id);
void unlock(spinLock* lock, void*);
void* lock(ticketLock* lock, uint64_t node_id);
void unlock(ticketLock* lock, void*);
mcsQueueMember* lock(mcsLock* lock, uint64_t node_id);
void unlock(mcsLock* lock, mcsQueueMember* next);
spinLock* buildSpinLock();
ticketLock* buildTicketLock();
mcsLock* buildMcsLock();
void destorySpinLock(spinLock* spin);
void detroyTicketLock(ticketLock* ticket);
void detroyMcsLock(mcsLock* mcs);
#endif