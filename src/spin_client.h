#ifndef SPIN_CLIENT_H
#define SPIN_CLIENT_H
#include <time.h>
#include "spin.h"
#include "local_lock.h"

typedef struct {
	char * parent_address;
	long parent_port;
	uint64_t node_id;
	int critical_section;
	int noncritical_section;
	int num_aquire;
	char * machine_lock_type;
	union machine_lock machine_lock;
} spin_client_in;

client_ctx* build_client_spin_context(struct rdma_cm_id* client_id, volatile uint64_t* metadata, uint64_t *buffer, uint64_t* node_id);
int acquire_spin_lock(struct rdma_cm_id *id, uint64_t *buffer);
int release_spin_lock(struct rdma_cm_id *id, uint64_t *buffer);
struct rdma_cm_id* connect_to_spin_server(struct rdma_event_channel* cm_event_channel, struct sockaddr_in* server_sockaddr,uint64_t* node_id, uint64_t *buffer, volatile uint64_t * metadata);
void * spin_client(void * in);
#endif