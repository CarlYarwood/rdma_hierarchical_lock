#include "local_lock.h"

void* lockSpin(spinLock *lock, uint64_t node_id) {
    do {} while(lock->owner != 0);
    pthread_mutex_lock(lock->lock_mutex);
    if(lock->owner != 0) {
        pthread_mutex_unlock(lock->lock_mutex);
        return lockSpin(lock, node_id);
    }
    lock->owner = node_id;
    pthread_mutex_unlock(lock->lock_mutex);
    return NULL;
}

void unlockSpin(spinLock *lock, void*) {
    pthread_mutex_lock(lock->lock_mutex);
    lock->owner = 0;
    pthread_mutex_unlock(lock->lock_mutex);
}

void* lockTicket(ticketLock *lock, uint64_t node_id) {
    uint64_t ticket;
    pthread_mutex_lock(lock->lock_mutex);
    ticket = lock->next;
    lock->next ++;
    pthread_mutex_unlock(lock->lock_mutex);
    do {} while(lock->now != ticket);
    pthread_mutex_lock(lock->lock_mutex);
    lock->owner = node_id;
    pthread_mutex_unlock(lock->lock_mutex);
    return NULL;
}

void unlockTicket(ticketLock *lock, void*) {
    pthread_mutex_lock(lock->lock_mutex);
    lock->now ++;
    pthread_mutex_unlock(lock->lock_mutex);
}

volatile uint64_t* lockMcs(mcsLock* lock, uint64_t node_id) {
    uint64_t* next = (uint64_t *)malloc(sizeof(uint64_t));
    *next = 0;
    pthread_mutex_lock(lock->lock_mutex);
    if(lock->owner == 0 && lock->queueEnd == NULL) {
        lock->owner = node_id;
        lock->queueEnd = next;
        pthread_mutex_unlock(lock->lock_mutex);
        return next;
    }
    *(lock->queueEnd) = node_id;
    lock->queueEnd = next;
    pthread_mutex_unlock(lock->lock_mutex);
    do {} while(lock->owner != node_id);
    return next;
}

void unlockMcs(mcsLock *lock, volatile uint64_t * next) {
    pthread_mutex_lock(lock->lock_mutex);
    if (*next == 0) {
        lock->owner = 0;
        lock->queueEnd = NULL;
        pthread_mutex_unlock(lock->lock_mutex);
        return;
    }
    lock->owner = *next;
    pthread_mutex_unlock(lock->lock_mutex);
    free(next);
}

spinLock* buildSpinLock() {
    spinLock *lock = NULL;
    pthread_mutex_t *lock_mutex = NULL;
    lock = (spinLock *)malloc(sizeof(spinLock));
    lock_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock_mutex, NULL);
    lock->owner = 0;
    lock->lock_mutex = lock_mutex;
    return lock;
}

ticketLock* buildTicketLock() {
    ticketLock *lock = NULL;
    pthread_mutex_t *lock_mutex = NULL;
    lock = (ticketLock *)malloc(sizeof(ticketLock));
    lock_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock_mutex, NULL);
    lock->owner = 0;
    lock->now = 0;
    lock->next = 0;
    lock->lock_mutex = lock_mutex;
    return lock;
}

mcsLock* buildMcsLock() {
    mcsLock* lock = NULL;
    pthread_mutex_t *lock_mutex = NULL;
    lock = (mcsLock *)malloc(sizeof(mcsLock));
    lock_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock_mutex, NULL);
    lock->queueEnd = NULL;
    lock->owner = 0;
    lock->lock_mutex = lock_mutex;
    return lock;
}

void destroySpinLock(spinLock* spin) {
    pthread_mutex_destroy(spin->lock_mutex);
    free(spin->lock_mutex);
    free(spin);
}

void destroyTicketLock(ticketLock* ticket) {
    pthread_mutex_destroy(ticket->lock_mutex);
    free(ticket->lock_mutex);
    free(ticket);
}

void destroyMcsLock(mcsLock* mcs) {
    pthread_mutex_destroy(mcs->lock_mutex);
    free(mcs->lock_mutex);
    free(mcs);
}
