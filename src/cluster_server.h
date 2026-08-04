#ifndef CLUSTER_SERVER_H
#define CLUSTER_SERVER_H

#define CLUSTER_CHILDREN (10)

typedef struct {
	int parent_lock_type;
    int local_lock_size;
} cluster_in ;

struct c_ctx {
    uint64_t* cluster_id;
    struct ibv_pd* pd;
    struct ibv_comp_channel* comp;
    struct ibv_cq* cq;
    struct ibv_mr* buffer_mr;
    struct ibv_mr* metadata_mr;
    struct ibv_mr* server_metadata_mr;
    struct ibv_mr* client_metadata_mr;
    struct rdma_buffer_attr* server_metadata_attr;
    struct rdma_buffer_attr* client_metadata_attr;
};

#endif
