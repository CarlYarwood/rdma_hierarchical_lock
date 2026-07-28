#include "<pthread.h>"
#include "rdma_server.h"

typedef struct {
    int lock_type;
    int critical_section;
    int noncritical_section;
    int num_aquire;
    char * lock_type;
} config;

pthread_mutex_t* lock_mutex = NULL;

typedef struct {
    uint64_t next;
} mcsQueueMember;

typedef struct {
    uint64_t owner;
    mcsQueueMember* queueEnd;
} mcsLock

typedef struct {
    uint64_t now;
    uint64_t next;
} ticketLock;

typedef struct {
    uint64_t owner
} spinLock;

void lock_local_spin(spinLock *lock, uint64_t node_id) {
    do {} while(spinLock->owner != 0);
    pthread_lock(lock_mutex);
    if(spinLock->owner != 0) {
        pthread_unlock(lock_mutex);
        return acquire_local_spin(lock, node_id);
    }
    spinLock->owner = node_id;
    pthread_unlock(lock_mutex);
}

void unlock_local_spin(spinLock *lock) {
    pthread_lock(lock_mutex);
    lock->owner = 0;
    pthread_unlock(lock_mutex);
}

void lock_local_ticket(ticketLock *lock) {
    uint64_t ticket;
    pthread_lock(lock_mutex);
    ticket = lock->next;
    lock->next ++;
    pthread_unlock(lock_mutex);
    do {} while(lock->now != ticket);
}

void unlock_local_ticket(ticketLock *lock) {
    pthread_lock(lock_mutex);
    lock->now ++;
    pthread_unlock(lock_mutex);
}

mcsQueueMember* lock_local_mcsLock(mcsLock* lock, uint64_t node_id) {
    mcsQueueMember* ret = (mcsQueueMember *)malloc(sizeof(mcsQueueMember));
    ret->next = 0;
    pthread_lock(lock_mutex);
    if(lock->owner == 0 && lock->queueEnd == NULL) {
        lock->owner = node_id;
        lock->queueEnd = ret;
        pthread_unlock(lock_mutex);
        return ret;
    }
    lock->queueEnd->next = node_id;
    lock->queueEnd = ret;
    pthread_unlock(lock_mutex);
    do {} while(lock->owner != node_id);
    return ret;
}

void unlock_local_mcsLock(mcsLock *lock, mcsQueueMember* next) {
    pthread_lock(lock_mutex);
    if (next->next == 0) {
        lock->owner = 0;
        lock->next = NULL;
        pthread_unlock(lock_mutex);
    }
    lock->owner = next->next;
    free(next);
    pthread_unlock(lock_mutex);
}


int main(int argc, char ** argv){
    int option = 0;
    char * lock_type;
    int num_handover;

    while ((option = getopt(argc, argv, "s:")) != -1) {
		switch (option) {
		}
	}
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock