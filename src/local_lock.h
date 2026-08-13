#ifndef LOCAL_LOCK_H
#define LOCAL_LOCK_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#include <netdb.h>
#include <netinet/in.h>	
#include <arpa/inet.h>
#include <sys/socket.h>

#include <rdma/rdma_cma.h>
#include <infiniband/verbs.h>
typedef struct {
    uint64_t * owner;
    mcsQMember * volatile next;
} mcsQMember;

typedef struct {
    pthread_mutex_t* lock_mutex;
    volatile uint64_t owner;
    mcsQMember * volatile next;
    mcsQMember * volatile last;
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

union machine_lock {
    mcsLock * mcs;
    ticketLock * ticket;
    spinLock * spin;
};

void lockSpin(spinLock* lock, uint64_t node_id);
void unlockSpin(spinLock* lock);
void lockTicket(ticketLock* lock, uint64_t node_id);
void unlockTicket(ticketLock* lock);
volatile uint64_t* lockMcs(mcsLock* lock, uint64_t node_id);
void unlockMcs(mcsLock* lock, volatile uint64_t* next);
spinLock* buildSpinLock();
ticketLock* buildTicketLock();
mcsLock* buildMcsLock();
void destroySpinLock(spinLock* spin);
void destroyTicketLock(ticketLock* ticket);
void destroyMcsLock(mcsLock* mcs);
#endif