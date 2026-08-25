#ifndef SPIN_SERVER_H
#define SPIN_SERVER_H
#include "spin.h"

typedef struct {
    int num_children;
    volatile int *num_conn;
    volatile int * ready;
    volatile uint64_t * lock;
    volatile uint64_t * buffer;
    struct rdma_cm_id ** id_arr;
} spin_server_in; 

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
} s_spin_ctx;

void *spin_server(void *);
s_spin_ctx* build_server_spin_context(struct rdma_cm_id* client_id, volatile uint64_t *lock, volatile uint64_t *buffer, uint64_t * node_id);
int send_server_spin_metadata(struct rdma_cm_id* client_id);
int clean_up_spin_context(struct rdma_cm_id* client_id);
int rdma_spin_write(struct rdma_cm_id *client_id, int offset);
int notify_spin_clients(struct rdma_cm_id ** id_arr, volatile uint64_t *buffer, int num_children);
#endif