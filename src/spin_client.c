#include "spin_client.h"

client_ctx* build_client_spin_context(struct rdma_cm_id* client_id, volatile uint64_t* metadata, uint64_t *buffer, uint64_t* node_id) {
	client_ctx *ctx = NULL;
	struct ibv_pd* pd = NULL;
    struct ibv_comp_channel* comp = NULL;
    struct ibv_cq* cq = NULL;
    struct ibv_mr *buffer_mr = NULL;
	struct ibv_mr *metadata_mr = NULL;
    struct ibv_mr *server_metadata_mr = NULL;
	struct ibv_mr *client_metadata_mr = NULL;
    struct ibv_qp_init_attr qp_init_attr;
    struct rdma_buffer_attr *server_metadata_attr = NULL;
	struct rdma_buffer_attr *client_metadata_attr = NULL;
	struct ibv_sge client_recv_sge;
	struct ibv_recv_wr client_recv_wr, *bad_client_recv_wr = NULL;

	ctx = (client_ctx*)malloc(sizeof(client_ctx));
    server_metadata_attr = (struct rdma_buffer_attr *)malloc(sizeof(struct rdma_buffer_attr));
	client_metadata_attr = (struct rdma_buffer_attr *)malloc(sizeof(struct rdma_buffer_attr));

	pd = ibv_alloc_pd(client_id->verbs);
	if (!pd) {
		rdma_error("Failed to alloc pd, errno: %d \n", -errno);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}
    comp = ibv_create_comp_channel(client_id->verbs);
	if (!comp) {
		rdma_error("Failed to create IO completion event channel, errno: %d\n", -errno);
		ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
	    return NULL;
	}

    cq = ibv_create_cq(client_id->verbs, CQ_CAPACITY, NULL, comp, 0);
	if (!cq) {
		rdma_error("Failed to create CQ, errno: %d \n", -errno);
		ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}

	if (ibv_req_notify_cq(cq, 0)) {
		rdma_error("Failed to request notifications, errno: %d\n", -errno);
		ibv_destroy_cq(cq);
		ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}
    bzero(&qp_init_attr, sizeof qp_init_attr);
    qp_init_attr.cap.max_recv_sge = MAX_SGE;
    qp_init_attr.cap.max_recv_wr = MAX_WR;
    qp_init_attr.cap.max_send_sge = MAX_SGE;
    qp_init_attr.cap.max_send_wr = MAX_WR;
    qp_init_attr.qp_type = IBV_QPT_RC;

    qp_init_attr.recv_cq = cq;
    qp_init_attr.send_cq = cq;

	if (rdma_create_qp(client_id, pd, &qp_init_attr)) {
		rdma_error("Failed to create QP, errno: %d \n", -errno);
		ibv_destroy_cq(cq);
		ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
	    return NULL;
	}

    buffer_mr = rdma_buffer_register(pd, buffer, sizeof(*buffer), (IBV_ACCESS_LOCAL_WRITE));
	if(!buffer_mr){
		perror("Failed to setup buffer mr\n");
		rdma_destroy_qp(client_id);
		ibv_destroy_cq(cq);
        ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}

	server_metadata_mr = rdma_buffer_register(pd, server_metadata_attr, sizeof(*server_metadata_attr), (IBV_ACCESS_LOCAL_WRITE));
	if(!server_metadata_mr){
		rdma_error("Failed to setup the server metadata mr , -ENOMEM\n");
		rdma_destroy_qp(client_id);
		rdma_buffer_deregister(buffer_mr);
        ibv_destroy_cq(cq);
        ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}

	client_recv_sge.addr = (uint64_t) server_metadata_mr->addr;
	client_recv_sge.length = (uint32_t) server_metadata_mr->length;
	client_recv_sge.lkey = (uint32_t) server_metadata_mr->lkey;

	bzero(&client_recv_wr, sizeof(client_recv_wr));
	client_recv_wr.sg_list = &client_recv_sge;
	client_recv_wr.num_sge = 1;
	if (ibv_post_recv(client_id->qp , &client_recv_wr, &bad_client_recv_wr)) {
		perror("Failed to pre-post the receive buffer, errno: %d \n");
		rdma_destroy_qp(client_id);
		rdma_buffer_deregister(server_metadata_mr);
		rdma_buffer_deregister(buffer_mr);
        ibv_destroy_cq(cq);
        ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}
	debug("Receive buffer pre-posting is successful \n");

	metadata_mr = rdma_buffer_register(pd, (void *)metadata, sizeof(uint64_t), (IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_ATOMIC));
	if (!metadata_mr) {
		perror("Failed to setup metadata mr: %d \n");
		rdma_destroy_qp(client_id);
		rdma_buffer_deregister(server_metadata_mr);
		rdma_buffer_deregister(buffer_mr);
        ibv_destroy_cq(cq);
        ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}

	client_metadata_attr->address = (uint64_t)metadata_mr->addr;
	client_metadata_attr->length = (uint32_t)metadata_mr->length;
	client_metadata_attr->stag.remote_stag = (uint32_t)metadata_mr->rkey;
	client_metadata_mr = rdma_buffer_register(pd, client_metadata_attr, sizeof(struct rdma_buffer_attr), (IBV_ACCESS_LOCAL_WRITE));
	if(!client_metadata_mr) {
		perror("Failed to setup client metadata mr: %d \n");
		rdma_destroy_qp(client_id);
		rdma_buffer_deregister(client_metadata_mr);
		rdma_buffer_deregister(server_metadata_mr);
		rdma_buffer_deregister(buffer_mr);
        ibv_destroy_cq(cq);
        ibv_destroy_comp_channel(comp);
        ibv_dealloc_pd(pd);
		free(ctx);
		free(server_metadata_attr);
		return NULL;
	}

	ctx->node_id = node_id;
	ctx->pd = pd;
	ctx->comp = comp;
	ctx->cq = cq;
	ctx->buffer_mr = buffer_mr;
	ctx->metadata_mr = metadata_mr;
	ctx->server_metadata_mr = server_metadata_mr;
	ctx->client_metadata_mr = client_metadata_mr;
	ctx->server_metadata_attr = server_metadata_attr;
	ctx->client_metadata_attr = client_metadata_attr;

	return ctx;
}

int acquire_spin_lock(struct rdma_cm_id *id, uint64_t *buffer) {
	client_ctx *ctx = (client_ctx *)(id->context);
	*buffer = 1;
	do {
        copmare_and_swap(id, 0, *(ctx->node_id), 0);
    } while(*buffer != 0);
	return 0;
}

int release_spin_lock(struct rdma_cm_id *id, uint64_t *buffer) {
	client_ctx *ctx = (client_ctx *)(id->context);
	*buffer = 0;
	copmare_and_swap(id, *(ctx->node_id), 0,0);
    if(*buffer != *(ctx->node_id)) {
        perror("lock release failed\n");
        return -1;
    }
	return 0;
}

struct rdma_cm_id* connect_to_spin_server(struct rdma_event_channel* cm_event_channel, struct sockaddr_in* server_sockaddr, uint64_t* node_id, uint64_t *buffer, volatile uint64_t * metadata) {
	client_ctx *ctx = NULL;
	struct rdma_cm_id *cm_client_id = NULL;
	struct rdma_cm_event *cm_event = NULL;
	struct rdma_conn_param conn_param;
	struct ibv_wc wc;

	if (rdma_create_id(cm_event_channel, &cm_client_id, NULL, RDMA_PS_TCP)) {
		rdma_error("Creating cm id failed with errno: %d \n", -errno); 
		return NULL;
	}

	if (rdma_resolve_addr(cm_client_id, NULL, (struct sockaddr*) server_sockaddr, 2000)) {
		rdma_error("Failed to resolve address, errno: %d \n", -errno);
		return NULL;
	}

	if (process_rdma_cm_event(cm_event_channel, RDMA_CM_EVENT_ADDR_RESOLVED, &cm_event)) {
		perror("Failed to receive a valid event, ret = %d \n");
		return NULL;
	}

	if (rdma_ack_cm_event(cm_event)) {
		rdma_error("Failed to acknowledge the CM event, errno: %d\n", -errno);
		return NULL;
	}

	if (rdma_resolve_route(cm_client_id, 2000)) {
		rdma_error("Failed to resolve route, erno: %d \n", -errno);
	       return NULL;
	}
	debug("waiting for cm event: RDMA_CM_EVENT_ROUTE_RESOLVED\n");

	ctx = build_client_spin_context(cm_client_id, metadata, buffer, node_id);
	if (!ctx) {
		perror("Failed to build context\n");
		return NULL;
	}

	cm_client_id->context = (void *)ctx;

	if (process_rdma_cm_event(cm_event_channel, RDMA_CM_EVENT_ROUTE_RESOLVED, &cm_event)) {
		perror("Failed to receive a valid event, ret = %d \n");
		return NULL;
	}

    if (rdma_ack_cm_event(cm_event)) {
		rdma_error("Failed to acknowledge the CM event, errno: %d \n", -errno);
		return NULL;
	}

	

    bzero(&conn_param, sizeof(conn_param));
	conn_param.initiator_depth = 3;
	conn_param.responder_resources = 3;
	conn_param.retry_count = 3;
	conn_param.private_data = (void *) node_id;
    conn_param.private_data_len = sizeof(uint64_t);
	if (rdma_connect(cm_client_id, &conn_param)) {
		rdma_error("Failed to connect to remote host , errno: %d\n", -errno);
		return NULL;
	}
	debug("waiting for cm event: RDMA_CM_EVENT_ESTABLISHED\n");

	if (process_rdma_cm_event(cm_event_channel, RDMA_CM_EVENT_ESTABLISHED, &cm_event)) {
		perror("Failed to get cm event, ret = %d \n");
	    return NULL;
	}

	if (rdma_ack_cm_event(cm_event)) {
		rdma_error("Failed to acknowledge cm event, errno: %d\n", -errno);
		return NULL;
	}

	if(send_client_metadata(cm_client_id)) {
		perror("Failed to send client metadata.\n");
		return NULL;
	}

	return cm_client_id;
}

void * spin_client(void * in) {
	struct rdma_cm_id *server = NULL;
	spinLock * spin = NULL;
    ticketLock * ticket = NULL;
    mcsLock * mcs = NULL;
	struct sockaddr_in server_sockaddr;
	struct rdma_event_channel *cm_event_channel = NULL;
	char * machine_lock_type = ((spin_client_in *)in)->machine_lock_type;
	char * parent_address = ((spin_client_in *)in)->parent_address;
	long parent_port = ((spin_client_in *)in)->parent_port;
	uint64_t *buffer = (uint64_t *)malloc(sizeof(uint64_t));
	uint64_t *node_id = (uint64_t *)malloc(sizeof(uint64_t));
	int critical_section = ((spin_client_in *) in)->critical_section;
	int noncritical_section = ((spin_client_in *) in)->noncritical_section;
	int num_aquire = ((spin_client_in *) in)->num_aquire;
	volatile uint64_t *metadata = (volatile uint64_t *)malloc(sizeof(uint64_t));
	*metadata = 0;
	clock_t start, end;
	switch(*machine_lock_type) {
        case 'm':
            mcs = ((spin_client_in *) in)->machine_lock.mcs;
            break;
        case 't':
            ticket = ((spin_client_in *)in)->machine_lock.ticket;
            break;
        case 's':
            spin = ((spin_client_in *)in)->machine_lock.spin;
            break;
        default:
            //Nothing
    }

	*node_id = ((spin_client_in *) in)->node_id;

	server_sockaddr = build_sockaddr(parent_address, parent_port);

	cm_event_channel = rdma_create_event_channel();
	if (!cm_event_channel) {
		rdma_error("Creating cm event channel failed, errno: %d \n", -errno);
		return NULL;
	}
	
	server = connect_to_spin_server(cm_event_channel, &server_sockaddr, node_id, buffer, metadata);

	wait_on_data(metadata, 1);
	*metadata = 0;

	start = clock();
	for (int i = 0; i < num_aquire; i++) {
		for (int n = 0; n < noncritical_section; n++) {
			noop(&n);
		}
		//lock
        switch(*machine_lock_type) {
            case 'm':
                lockMcs(mcs, *node_id);
                break;
            case 't':
                lockTicket(ticket , *node_id);
                break;
            case 's':
                lockSpin(spin, *node_id);
                break;
            default:
                //Nothing
        }
		acquire_spin_lock(server, node_id, buffer);

		//work
		for (int c = 0; c < critical_section; c++) {
			noop(&c);
		}
		//unlock
		release_spin_lock(server, node_id, buffer);
		switch(*machine_lock_type) {
            case 'm':
                unlockMcs(mcs);
                break;
            case 't':
                unlockTicket(ticket);
                break;
            case 's':
                unlockSpin(spin);
                break;
            default:
                //Nothing
        }
	}
	end = clock();

	disconnect_client(cm_event_channel, server);
	/* We free the buffers */
	free(metadata);
	free(buffer);
	free(node_id);

	rdma_destroy_event_channel(cm_event_channel);

	printf("%f\n",((double)(num_aquire * critical_section))/((double)(end-start)/CLOCKS_PER_SEC));
	return NULL;
}
