#ifndef LOCAL_LOCK
#define LOCAL_LOCK
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

void lock_local_spin(spinLock* lock, uint64_t node_id);
void unlock_local_spin(spinLock* lock);
void lock_local_ticket(ticketLock* lock, uint64_t node_id);
void unlock_local_ticket(ticketLock* lock);
mcsQueueMember* lock_local_mcs(mcsLock* lock, uint64_t node_id);
void unlock_local_mcs(mcsLock* lock, mcsQueueMember* next);
spinLock* buildSpinLock();
ticketLock* buildTicketLock();
mcsLock* buildMcsLock();
void destorySpinLock(spinLock* spin);
void detroyTicketLock(ticketLock* ticket);
void detroyMcsLock(mcsLock* mcs);
#endif