#include "local_lock.h"

void lockSpin(spinLock *lock, uint64_t node_id) {
    do {} while(lock->owner != 0);
    pthread_mutex_lock(lock->lock_mutex);
    if(lock->owner != 0) {
        pthread_mutex_unlock(lock->lock_mutex);
        return lockSpin(lock, node_id);
    }
    lock->owner = node_id;
    pthread_mutex_unlock(lock->lock_mutex);
}

void unlockSpin(spinLock *lock) {
    pthread_mutex_lock(lock->lock_mutex);
    lock->owner = 0;
    pthread_mutex_unlock(lock->lock_mutex);
}

void lockTicket(ticketLock *lock, uint64_t node_id) {
    uint64_t ticket;
    pthread_mutex_lock(lock->lock_mutex);
    ticket = lock->next;
    lock->next ++;
    pthread_mutex_unlock(lock->lock_mutex);
    do {} while(lock->now != ticket);
    pthread_mutex_lock(lock->lock_mutex);
    lock->owner = node_id;
    pthread_mutex_unlock(lock->lock_mutex);
}

void unlockTicket(ticketLock *lock) {
    pthread_mutex_lock(lock->lock_mutex);
    lock->now ++;
    pthread_mutex_unlock(lock->lock_mutex);
}

volatile void lockMcs(mcsLock* lock, uint64_t node_id) {
    last = (struct mcsQMember *)malloc(sizeof(struct mcsQMember));
    last->owner = node_id;
    last->next = NULL;
    pthread_mutex_lock(lock->lock_mutex);
    if(lock->owner == 0) {
        lock->owner = node_id;
        free(last);
        pthread_mutex_unlock(lock->lock_mutex);
        return;
    } elif (lock->next  == NULL) {
        lock->next = last;
        lock->last = last;
        pthread_mutex_unlock(lock->lock_mutex);
        return;
    }
    lock->last->next = last;
    lock->last = last;
    pthread_mutex_unlock(lock->lock_mutex);
    do {} while(lock->owner != node_id);
}

void unlockMcs(mcsLock *lock) {
    pthread_mutex_lock(lock->lock_mutex);
    if (lock->next == NULL) {
        lock->owner = 0;
        lock->next = NULL;
        lock->last = NULL;
        pthread_mutex_unlock(lock->lock_mutex);
        return;
    }
    lock->owner = lock->next->owner;
    struct mcsQmember* trash = lock->next;
    lock->next = lock->next->next;
    free(trash);
    pthread_mutex_unlock(lock->lock_mutex);
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
    lock->next = NULL;
    lock->last = NULL;
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
