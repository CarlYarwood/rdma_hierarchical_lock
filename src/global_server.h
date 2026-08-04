#ifndef GLOBAL_SERVER_H
#define GLOBAL_SERVER_H
#include "rdma_common.h"

#define TICKET (2)
#define MCS (1)
#define SPIN (1)

typedef struct {
    int lock_size;
    int* keep_going;
    long port;
} global_server_in;

typedef struct {
    struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
    struct ibv_cq* cq;
    struct ibv_mr* buffer_mr;
    struct ibv_mr* lock_mr;
    struct ibv_mr* server_metadata_mr;
    struct ibv_mr* client_metadata_mr;
    struct rdma_buffer_attr* server_metadata_attr;
    struct rdma_buffer_attr* client_metadata_attr;
} g_ctx;

void* global_server(void *in);
#endif
