#ifndef MCS_CLIENT_H
#define MCS_CLIENT_H
#include <time.h>
#include "mcs.h"
#include "local_lock.h"

#define NEXT (0)
#define NOTIFY (1)

#define SERVER (0)

typedef struct {
    char * parent_address;
    long parent_port;
    char ** peer_addresses;
    long * peer_ports;
	uint64_t node_id;
	int critical_section;
	int noncritical_section;
	int num_aquire;
    int num_peers;
    char * machine_lock_type;
    union machine_lock machine_lock;
} mcs_client_in;

client_ctx* build_mcs_context(struct rdma_cm_id* client_id, volatile uint64_t *metadata, uint64_t *buffer, uint64_t* node_id);
int acquire_mcs_lock(struct rdma_cm_id ** id_arr, uint64_t *node_id, uint64_t *buffer, volatile uint64_t* metadata);
int release_mcs_lock(struct rdma_cm_id** id_arr, uint64_t* node_id, uint64_t *buffer, volatile uint64_t* metadata);
struct rdma_cm_id* connect_to_peer(struct sockaddr_in* server_sockaddr, struct rdma_event_channel* cm_event_channel, uint64_t *node_id, uint64_t peer_id, uint64_t *buffer, volatile uint64_t *metadata);
void* mcs_client(void *in);
#endif