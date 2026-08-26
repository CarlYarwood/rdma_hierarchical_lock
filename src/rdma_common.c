/*
 * Implementation of the common RDMA functions. 
 *
 * Authors: Animesh Trivedi
 *          atrivedi@apache.org 
 */

#include "rdma_common.h"

int disconnect_client(struct rdma_event_channel* cm_event_channel, struct rdma_cm_id* id) {
	struct rdma_cm_event *cm_event = NULL;
	int ret = 0;
	if (rdma_disconnect(id)) {
		rdma_error("Failed to disconnect, errno: %d \n", -errno);
		ret = -1;
		//continuing anyways
	}
	if (process_rdma_cm_event(cm_event_channel, RDMA_CM_EVENT_DISCONNECTED, &cm_event)) {
		perror("Failed to get RDMA_CM_EVENT_DISCONNECTED event, ret = %d\n");
		ret = -1;
		//continuing anyways 
	}
	if (rdma_ack_cm_event(cm_event)) {
		rdma_error("Failed to acknowledge cm event, errno: %d\n", -errno);
		ret = -1;
		//continuing anyways
	}
			
	if(clean_up_client(id)) {
		perror("Failed to detroy context fully");
		ret = -1;
	}

	return ret;
}

int clean_up_client(struct rdma_cm_id* id) {
	client_ctx * ctx = (client_ctx *)(id->context);
	int ret = 0;
	rdma_destroy_qp(id);

	if (rdma_destroy_id(id)) {
		rdma_error("Failed to destroy client id cleanly, %d \n", -errno);
		ret = -1;
	}

	if (ibv_destroy_cq(ctx->cq)) {
		rdma_error("Failed to destroy completion queue cleanly, %d \n", -errno);
		ret = -1;
	}

	if (ibv_destroy_comp_channel(ctx->comp)) {
		rdma_error("Failed to destroy completion channel cleanly, %d \n", -errno);
		ret = -1;
		// we continue anyways;
	}

	/* Destroy memory buffers */
	rdma_buffer_deregister(ctx->client_metadata_mr);
	rdma_buffer_deregister(ctx->metadata_mr);
	rdma_buffer_deregister(ctx->server_metadata_mr);
	rdma_buffer_deregister(ctx->buffer_mr);

	if (ibv_dealloc_pd(ctx->pd)) {
		rdma_error("Failed to destroy client protection domain cleanly, %d \n", -errno);
		ret = -1;
		// we continue anyways;
	}

	free(ctx->client_metadata_attr);
	free(ctx->server_metadata_attr);
	free(ctx->node_id);
	free(ctx);

	return ret;
}

int send_client_metadata(struct rdma_cm_id * id) {
	client_ctx *ctx = (client_ctx *)(id->context);
	struct ibv_wc wc;
	struct ibv_sge client_send_sge;
	struct ibv_send_wr client_send_wr, *bad_client_send_wr = NULL;

	client_send_sge.addr = (uint64_t)(ctx->client_metadata_attr);
	client_send_sge.length = (uint32_t) sizeof(struct rdma_buffer_attr);
	client_send_sge.lkey = (uint32_t) (ctx->client_metadata_mr)->lkey;

	bzero(&client_send_wr, sizeof(client_send_wr));
	client_send_wr.sg_list = &client_send_sge;
	client_send_wr.num_sge = 1;
	client_send_wr.opcode = IBV_WR_SEND;
	client_send_wr.send_flags = IBV_SEND_SIGNALED;

	if(ibv_post_send(id->qp, &client_send_wr, &bad_client_send_wr)){
		rdma_error("Posting of client metdata failed, errno: %d \n", -errno);
	    return -errno;
	}

	if ( process_work_completion_events(ctx->cq, &wc, 2) != 2) {
	    perror("Failed to send client metadata, ret = %d \n");
	    return -1;
    }
	return 0;
}

int compare_and_swap(struct rdma_cm_id* client_id, uint64_t cmp, uint64_t swap, int offset) {
	client_ctx* ctx = (client_ctx *)client_id->context;
    uint64_t ret = -1;
    struct ibv_send_wr cas_wr, *bad_cas_wr = NULL;
    struct ibv_wc cas_wc;
    struct ibv_sge cas_sge;

    cas_sge.addr = (uint64_t) (ctx->buffer_mr)->addr;
    cas_sge.length = (uint64_t) (ctx->buffer_mr)->length;
    cas_sge.lkey = (uint64_t)(ctx->buffer_mr)->lkey;
    
    bzero(&cas_wr, sizeof(cas_wr));
    cas_wr.sg_list = &cas_sge;
    cas_wr.num_sge = 1;
    cas_wr.opcode = IBV_WR_ATOMIC_CMP_AND_SWP;
    cas_wr.wr.atomic.rkey = (ctx->server_metadata_attr)->stag.remote_stag;
    cas_wr.wr.atomic.remote_addr = (ctx->server_metadata_attr)->address + (sizeof(uint64_t) * offset);
    cas_wr.wr.atomic.compare_add = cmp;
    cas_wr.wr.atomic.swap = swap;
    cas_wr.send_flags = IBV_SEND_SIGNALED;

    ret = ibv_post_send(client_id->qp, &cas_wr, &bad_cas_wr);
    if(ret) {
        perror("Failed to send cas\n");
        return 1;
    }

    if (process_work_completion_events(ctx->cq, &cas_wc, 1) != 1) {
        perror("We failed to get 1 work completions\n");
        return 1;
    }
    return 0;
}

int rdma_read(struct rdma_cm_id* id, int offset) {
	client_ctx *ctx = (client_ctx *)(id->context);
	int ret = -1;
    struct ibv_send_wr read_wr, *bad_read_wr = NULL;
    struct ibv_wc read_wc;
    struct ibv_sge read_sge;

    read_sge.addr = (uint64_t) (ctx->buffer_mr)->addr;
    read_sge.length = (uint64_t) (ctx->buffer_mr)->length;
    read_sge.lkey = (uint64_t)(ctx->buffer_mr)->lkey;

	bzero(&read_wr, sizeof(read_wr));
    read_wr.sg_list = &read_sge;
    read_wr.num_sge = 1;
    read_wr.opcode = IBV_WR_RDMA_READ;
	read_wr.send_flags = IBV_SEND_SIGNALED;

	read_wr.wr.rdma.rkey = (ctx->server_metadata_attr)->stag.remote_stag;
    read_wr.wr.rdma.remote_addr = (ctx->server_metadata_attr)->address + sizeof(uint64_t) * offset;

	ret = ibv_post_send(id->qp, &read_wr, &bad_read_wr);
    if(ret) {
        perror("Failed to send read\n");
        return 1;
    }
    ret = process_work_completion_events(ctx->cq, &read_wc, 1);
    if (ret != 1) {
        perror("We failed to get 1 work completions\n");
        return 1;
    }
    return 0;

}

int fetch_and_add(struct rdma_cm_id* id, int offset) {
	client_ctx *ctx = (client_ctx *)(id->context);
    int ret = -1;
    struct ibv_send_wr cas_wr, *bad_cas_wr = NULL;
    struct ibv_wc cas_wc;
    struct ibv_sge cas_sge;

    cas_sge.addr = (uint64_t) (ctx->buffer_mr)->addr;
    cas_sge.length = (uint64_t) (ctx->buffer_mr)->length;
    cas_sge.lkey = (uint64_t) (ctx->buffer_mr)->lkey;
    
    bzero(&cas_wr, sizeof(cas_wr));
    cas_wr.sg_list = &cas_sge;
    cas_wr.num_sge = 1;
    cas_wr.opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
    cas_wr.wr.atomic.rkey = (ctx->server_metadata_attr)->stag.remote_stag;
    cas_wr.wr.atomic.remote_addr = (ctx->server_metadata_attr)->address + sizeof(uint64_t) * offset;
    cas_wr.wr.atomic.compare_add = 1;
    cas_wr.send_flags = IBV_SEND_SIGNALED;

    ret = ibv_post_send(id->qp, &cas_wr, &bad_cas_wr);
    if(ret) {
        perror("Failed to send cas\n");
        return 1;
    }
    ret = process_work_completion_events(ctx->cq, &cas_wc, 1);
    if (ret != 1) {
        perror("We failed to get 1 work completions\n");
        return 1;
    }
    return 0;
}

struct sockaddr_in build_sockaddr(char * address, long port) {
	struct sockaddr_in ret;
	bzero(&ret, sizeof ret);
	ret.sin_family = AF_INET;
	if (get_addr(address, (struct sockaddr*) &ret)) {
		printf("Invalid IP\n");
		return ret;
	}
	ret.sin_port = htons(parent_port);
	return ret;
}

void noop(volatile int *dummy) {
    *dummy = *dummy; 
}

void wait_on_data(volatile uint64_t *data, uint64_t val) {
	do {} while(*data != val);
}

void show_rdma_cmid(struct rdma_cm_id *id)
{
	if(!id){
		rdma_error("Passed ptr is NULL\n");
		return;
	}
	printf("RDMA cm id at %p \n", id);
	if(id->verbs && id->verbs->device)
		printf("dev_ctx: %p (device name: %s) \n", id->verbs, 
				id->verbs->device->name);
	if(id->channel)
		printf("cm event channel %p\n", id->channel);
	printf("QP: %p, port_space %x, port_num %u \n", id->qp, 
			id->ps,
			id->port_num);
}

void show_rdma_buffer_attr(struct rdma_buffer_attr *attr){
	if(!attr){
		rdma_error("Passed attr is NULL\n");
		return;
	}
	printf("---------------------------------------------------------\n");
	printf("buffer attr, addr: %p , len: %u , stag : 0x%x \n", 
			(void*) attr->address, 
			(unsigned int) attr->length,
			attr->stag.local_stag);
	printf("---------------------------------------------------------\n");
}

struct ibv_mr* rdma_buffer_alloc(struct ibv_pd *pd, uint32_t size,
    enum ibv_access_flags permission) 
{
	struct ibv_mr *mr = NULL;
	if (!pd) {
		rdma_error("Protection domain is NULL \n");
		return NULL;
	}
	void *buf = calloc(1, size);
	if (!buf) {
		rdma_error("failed to allocate buffer, -ENOMEM\n");
		return NULL;
	}
	debug("Buffer allocated: %p , len: %u \n", buf, size);
	mr = rdma_buffer_register(pd, buf, size, permission);
	if(!mr){
		free(buf);
	}
	return mr;
}

struct ibv_mr *rdma_buffer_register(struct ibv_pd *pd, 
		void *addr, uint32_t length, 
		enum ibv_access_flags permission)
{
	struct ibv_mr *mr = NULL;
	if (!pd) {
		rdma_error("Protection domain is NULL, ignoring \n");
		return NULL;
	}
	mr = ibv_reg_mr(pd, addr, length, permission);
	if (!mr) {
		rdma_error("Failed to create mr on buffer, errno: %d \n", -errno);
		return NULL;
	}
	debug("Registered: %p , len: %u , stag: 0x%x \n", 
			mr->addr, 
			(unsigned int) mr->length, 
			mr->lkey);
	return mr;
}

void rdma_buffer_free(struct ibv_mr *mr) 
{
	if (!mr) {
		rdma_error("Passed memory region is NULL, ignoring\n");
		return ;
	}
	void *to_free = mr->addr;
	rdma_buffer_deregister(mr);
	debug("Buffer %p free'ed\n", to_free);
	free(to_free);
}

void rdma_buffer_deregister(struct ibv_mr *mr) 
{
	if (!mr) { 
		rdma_error("Passed memory region is NULL, ignoring\n");
		return;
	}
	debug("Deregistered: %p , len: %u , stag : 0x%x \n", 
			mr->addr, 
			(unsigned int) mr->length, 
			mr->lkey);
	ibv_dereg_mr(mr);
}

int process_rdma_cm_event(struct rdma_event_channel *echannel, 
		enum rdma_cm_event_type expected_event,
		struct rdma_cm_event **cm_event)
{
	int ret = 1;
	ret = rdma_get_cm_event(echannel, cm_event);
	if (ret) {
		rdma_error("Failed to retrieve a cm event, errno: %d \n",
				-errno);
		return -errno;
	}
	/* lets see, if it was a good event */
	if(0 != (*cm_event)->status){
		rdma_error("CM event has non zero status: %d\n", (*cm_event)->status);
		ret = -((*cm_event)->status);
		/* important, we acknowledge the event */
		rdma_ack_cm_event(*cm_event);
		return ret;
	}
	/* if it was a good event, was it of the expected type */
	if ((*cm_event)->event != expected_event) {
		rdma_error("Unexpected event received: %s [ expecting: %s ]", 
				rdma_event_str((*cm_event)->event),
				rdma_event_str(expected_event));
		/* important, we acknowledge the event */
		rdma_ack_cm_event(*cm_event);
		return -1; // unexpected event :(
	}
	debug("A new %s type event is received \n", rdma_event_str((*cm_event)->event));
	/* The caller must acknowledge the event */
	return ret;
}


// int process_work_completion_events (struct ibv_comp_channel *comp_channel, 
// 		struct ibv_wc *wc, int max_wc)
// {
// 	struct ibv_cq *cq_ptr = NULL;
// 	void *context = NULL;
// 	int ret = -1, i, total_wc = 0;
//        /* We wait for the notification on the CQ channel */
// 	ret = ibv_get_cq_event(comp_channel, /* IO channel where we are expecting the notification */ 
// 		       &cq_ptr, /* which CQ has an activity. This should be the same as CQ we created before */ 
// 		       &context); /* Associated CQ user context, which we did set */
//        if (ret) {
// 	       rdma_error("Failed to get next CQ event due to %d \n", -errno);
// 	       return -errno;
//        }
//        /* Request for more notifications. */
//        ret = ibv_req_notify_cq(cq_ptr, 0);
//        if (ret){
// 	       rdma_error("Failed to request further notifications %d \n", -errno);
// 	       return -errno;
//        }
//        /* We got notification. We reap the work completion (WC) element. It is 
// 	* unlikely but a good practice it write the CQ polling code that 
//        * can handle zero WCs. ibv_poll_cq can return zero. Same logic as 
//        * MUTEX conditional variables in pthread programming.
// 	*/
//        total_wc = 0;
//        do {
// 	       ret = ibv_poll_cq(cq_ptr /* the CQ, we got notification for */, 
// 		       max_wc - total_wc /* number of remaining WC elements*/,
// 		       wc + total_wc/* where to store */);
// 	       if (ret < 0) {
// 		       rdma_error("Failed to poll cq for wc due to %d \n", ret);
// 		       /* ret is errno here */
// 		       return ret;
// 	       }
// 	       total_wc += ret;
//        } while (total_wc < max_wc); 
//        debug("%d WC are completed \n", total_wc);
//        /* Now we check validity and status of I/O work completions */
//        for( i = 0 ; i < total_wc ; i++) {
// 	       if (wc[i].status != IBV_WC_SUCCESS) {
// 		       rdma_error("Work completion (WC) has error status: %s at index %d", 
// 				       ibv_wc_status_str(wc[i].status), i);
// 		       /* return negative value */
// 		       return -(wc[i].status);
// 	       }
//        }
//        /* Similar to connection management events, we need to acknowledge CQ events */
//        ibv_ack_cq_events(cq_ptr, 
// 		       1 /* we received one event notification. This is not 
// 		       number of WC elements */);
//        return total_wc; 
// }

int process_work_completion_events(struct ibv_cq *cq, struct ibv_wc *wc, int max_wc) {
    int ret, total_wc = 0;
    void *context = NULL;

    /* Request interrupt notification for the next completion */
    ret = ibv_req_notify_cq(cq, 0);  // 0 = solicited-only (optional: use 1 for all)
    if (ret) {
        fprintf(stderr, "Failed to request CQ notifications: %s\n", strerror(-ret));
        return -ret;
    }

    /* Poll for completions */
    do {
        ret = ibv_poll_cq(cq, max_wc - total_wc, wc + total_wc);
        if (ret < 0) {
            fprintf(stderr, "Failed to poll CQ: %s\n", strerror(-ret));
            return ret;
        }
        total_wc += ret;
    } while (total_wc < max_wc);

    /* Check completion statuses */
    for (int i = 0; i < total_wc; i++) {
        if (wc[i].status != IBV_WC_SUCCESS) {
            fprintf(stderr, "WC error at index %d: %s\n", 
                    i, ibv_wc_status_str(wc[i].status));
            return -(wc[i].status);
        }
    }

    return total_wc;
}


/* Code acknowledgment: rping.c from librdmacm/examples */
int get_addr(char *dst, struct sockaddr *addr)
{
	struct addrinfo *res;
	int ret = -1;
	ret = getaddrinfo(dst, NULL, NULL, &res);
	if (ret) {
		rdma_error("getaddrinfo failed - invalid hostname or IP address\n");
		return ret;
	}
	memcpy(addr, res->ai_addr, sizeof(struct sockaddr_in));
	freeaddrinfo(res);
	return ret;
}

