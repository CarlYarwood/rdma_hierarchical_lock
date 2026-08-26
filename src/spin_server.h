#ifndef SPIN_SERVER_H
#define SPIN_SERVER_H
#include "spin.h"

void *spin_server(void *);
server_ctx* build_server_spin_context(struct rdma_cm_id* client_id, volatile uint64_t *lock, volatile uint64_t *buffer, uint64_t * node_id);
int send_server_spin_metadata(struct rdma_cm_id* client_id);
int clean_up_spin_context(struct rdma_cm_id* client_id);
int rdma_spin_write(struct rdma_cm_id *client_id, int offset);
int notify_spin_clients(struct rdma_cm_id ** id_arr, volatile uint64_t *buffer, int num_children);
#endif