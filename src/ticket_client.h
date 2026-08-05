#ifndef TICKET_LOCK_CLIENT_H
#define TICKET_LOCK_CLIENT_H
#include <time.h>
#include "rdma_common.h"

typedef struct {
	struct sockaddr_in server_sockaddr;
	int critical_section;
	int noncritical_section;
	int num_aquire;
} ticket_client_in ;

typedef struct {
	struct rdma_cm_id* client_id;
	struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
	struct ibv_cq* cq;
	struct ibv_mr* sync_mr;
	struct ibv_mr* response_mr;
	struct ibv_mr* server_metadata_mr;
	struct ibv_mr* client_metadata_mr;
	struct rdma_buffer_attr* server_metadata_attr;
	struct rdma_buffer_attr* client_metadata_attr;
} c_ticket_ctx;

void * ticket_client(void * in);
#endif