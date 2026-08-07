#ifndef TICKET_LOCK_SERVER_H
#define TICKET_LOCK_SERVER_H
#include "ticket.h"

typedef struct {
    int num_children;
} ticket_server_in;

typedef struct {
    struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
    struct ibv_cq* cq;
    struct ibv_mr* lock_mr;
    struct ibv_mr* buffer_mr;
    struct ibv_mr* server_metadata_mr;
    struct ibv_mr* client_metadata_mr;
    struct rdma_buffer_attr* server_metadata_attr;
    struct rdma_buffer_attr* client_metadata_attr;
} s_ticket_ctx;

void *ticket_server(void*);
#endif