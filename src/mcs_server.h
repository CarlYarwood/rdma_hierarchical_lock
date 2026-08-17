#ifndef MCS_LOCK_SERVER_H
#define MCS_LOCK_SERVER_H
#include "mcs.h"

typedef struct {
    int num_children;
    volatile int * ready;
    volatile uint64_t *lock;
    struct rdma_cm_id ** id_arr;
} mcs_server_in;

typedef struct {
    uint64_t * node_id;
    struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
    struct ibv_cq* cq;
    struct ibv_mr* lock_mr;
    struct ibv_mr* buffer_mr;
    struct ibv_mr* server_metadata_mr;
    struct ibv_mr* client_metadata_mr;
    struct rdma_buffer_attr* server_metadata_attr;
    struct rdma_buffer_attr* client_metadata_attr;
} s_mcs_ctx;

void* mcs_server(void *);
#endif