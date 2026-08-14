#ifndef MCS_LOCK_SERVER_H
#define MCS_LOCK_SERVER_H
#include "mcs.h"

typedef struct {
    int num_children;
    volatile uint64_t *lock;
    struct rdma_cm_id ** id_arr;
} mcs_server_in;

struct s_mcs_ctx {
    struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
    struct ibv_cq* cq;
    struct ibv_mr* lock_mr;
    struct ibv_mr* buffer_mr;
    struct ibv_mr* server_metadata_mr;
    struct ibv_mr* client_metadata_mr;
    struct rdma_buffer_attr* server_metadata_attr;
    struct rdma_buffer_attr* client_metadata_attr;
};

void* mcs_server(void *);
#endif