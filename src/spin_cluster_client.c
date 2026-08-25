#include "spin_cluster_client.h"

void * spin_cluster_client(void * in) {
    c_spin_ctx *ctx = NULL;
	struct sockaddr_in server_sockaddr;
	struct rdma_event_channel *cm_event_channel = NULL;
	char * parent_address = ((spin_cluster_client_in *)in)->parent_address;
	long parent_port = ((spin_cluster_client_in *)in)->parent_port;
	uint64_t *response = calloc(1, sizeof(uint64_t));
	uint64_t *node_id = calloc(1, sizeof(uint64_t));
	volatile uint64_t *sync = ((spin_cluster_client_in *) in)->sync;
    struct rdma_cm_id ** id_arr = ((spin_cluster_client_in *)in)->id_arr;
    uint64_t * lock = ((spin_cluster_client_in *)in)->lock;
    uint64_t * buffer = ((spin_cluster_client_in *)in)->buffer;
    int * num_conn = ((spin_cluster_client_in *)in)->num_conn;
    int keepgoing = 1;
    int handover = ((spin_cluster_client_in *)in)->handover;
    int initial_handover_val = handover;
    int is_locked = 0;
	clock_t start, end;
    

	*node_id = ((spin_client_in *) in)->node_id;

	bzero(&server_sockaddr, sizeof server_sockaddr);
    server_sockaddr.sin_family = AF_INET;    
    if (get_addr(parent_address, (struct sockaddr*) &server_sockaddr)) {
		rdma_error("Invalid IP \n");
		return NULL;
	}
    server_sockaddr.sin_port = htons(parent_port);

	cm_event_channel = rdma_create_event_channel();
	if (!cm_event_channel) {
		rdma_error("Creating cm event channel failed, errno: %d \n", -errno);
		return NULL;
	}
	
	ctx = connect_to_spin_server(cm_event_channel, &server_sockaddr, node_id, response, sync);

    wait_on_sync(sync);
	*sync = 0;
    
    uint64_t last = 0;
    do {
        if(num_conn > 0) {
            keepgoing = 0;
        }
        if(*lock != last ) {
            last = *lock;
            if(*lock != 0) {
                if(is_locked == 0){
                    acquire_spin_lock(ctx, node_id, response);
                    is_locked = 1;
                }
                *buffer = 1;
                if(rdma_spin_write(id_arr[*lock - 1], SPIN_SYNC)){
                    printf("Failed to send sync");
                }
                handover --;
                if(handover == 0) {
                    release_spin_lock(ctx, node_id, response);
                    is_locked = 0;
                    handover = initial_handover_val;
                }
            }
        }
    } while(*num_conn > 0 || keepgoing == 1);
    if(is_locked == 1) {
        release_spin_lock(ctx, node_id, response);
    }


    //new_work

    disconnect_from_spin_server(cm_event_channel, ctx);
	/* We free the buffers */
	free(node_id);
	free(response);

	rdma_destroy_event_channel(cm_event_channel);
    return NULL;
}
