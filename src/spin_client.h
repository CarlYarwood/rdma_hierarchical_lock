#ifndef SPIN_CLIENT_H
#define SPIN_CLIENT_H
#include <time.h>
#include "spin.h"
#include "local_lock.h"

typedef struct {
	volatile uint64_t * sync;
	char * gated;
	char * machine_lock_type;
	char * parent_address;
	long parent_port;
	uint64_t node_id;
	int critical_section;
	int noncritical_section;
	int num_aquire;
	union machine_lock machine_lock;
} spin_client_in;

typedef struct {
	struct rdma_cm_id* client_id;
	struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
	struct ibv_cq* cq;
	struct ibv_mr* response_mr;
	struct ibv_mr* sync_mr;
	struct ibv_mr* server_metadata_mr;
	struct ibv_mr* client_metadata_mr;
	struct rdma_buffer_attr* server_metadata_attr;
	struct rdma_buffer_attr* client_metadata_attr;
} c_spin_ctx;

void* spin_client(void* in);
c_spin_ctx* build_client_spin_context(struct rdma_cm_id* client_id, uint64_t *response, volatile uint64_t* sync);
int destroy_spin_context(c_spin_ctx* ctx);
int send_client_spin_metadata(c_spin_ctx* ctx);
int copmare_and_swap(c_spin_ctx* ctx, uint64_t cmp, uint64_t swap);
int acquire_spin_lock(c_spin_ctx * ctx,uint64_t *node_id, uint64_t *response);
int release_spin_lock(c_spin_ctx *ctx, uint64_t* node_id, uint64_t *response);
c_spin_ctx* connect_to_spin_server(struct rdma_event_channel* cm_event_channel, struct sockaddr_in* server_sockaddr,uint64_t* node_id, uint64_t *response, volatile uint64_t *sync);
int disconnect_from_spin_server(struct rdma_event_channel* cm_event_channel, c_spin_ctx* ctx);
#endif