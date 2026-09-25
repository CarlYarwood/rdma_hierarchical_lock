#include "general_client.h"

void* general_client(void *in) {
    basicLock * basic = NULL;
    spinLock * spin = NULL;
    ticketLock * ticket = NULL;
    mcsLock * mcs = NULL;
    uint64_t * node_id = (uint64_t *)malloc(sizeof(uint64_t));
    clock_t start, end;
    int num_parents = ((general_client_in *)in)->num_parents;
    char * parent_types = ((general_client_in *)in)->parent_types;
    int critical_section = ((general_client_in *)in)->critical_section;
    int noncritical_section = ((general_client_in *)in)->noncritical_section;
    int num_aquire = ((general_client_in *)in)->num_aquire;
    char * machine_lock_type = ((general_client_in *)in)->machine_lock_type;
    union client_info * ci = ((general_client_in *)in)->client_info;
    struct rdma_event_channel *cm_event_channel = NULL;
    union lock_info * li = (union lock_info *)malloc(sizeof(union lock_info) * num_parents);
    *node_id = ((general_client_in *)in)->node_id;
    switch(*machine_lock_type) {
        case 'm':
            mcs = ((general_client_in *) in)->machine_lock.mcs;
            break;
        case 't':
            ticket = ((general_client_in *)in)->machine_lock.ticket;
            break;
        case 's':
            spin = ((general_client_in *)in)->machine_lock.spin;
            break;
        case 'b':
            basic = ((general_client_in *)in)->machine_lock.basic;
            break;
        default:
            //Nothing
    }

    cm_event_channel = rdma_create_event_channel();
    
    for (int i = 0; i < num_parents; i++) {
        switch(parent_types[i]) {
            case 'm':
                li[i].mcs_l_info = (mcs_lock_info *)malloc(sizeof(mcs_lock_info));
                li[i].mcs_l_info->id_arr = (struct rdma_cm_id **)malloc(sizeof(struct rdma_cm_id *) * (ci[i].mcs_c_info->num_peers + 1));
                for(int m = 0; m < (ci[i].mcs_c_info->num_peers + 1); m++) {
                    li[i].mcs_l_info->id_arr[m] = NULL;
                }
                li[i].mcs_l_info->node_id = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].mcs_l_info->node_id) = ci[i].mcs_c_info->node_id;
                li[i].mcs_l_info->buffer = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].mcs_l_info->buffer) = 0;
                li[i].mcs_l_info->metadata = (uint64_t *)malloc(sizeof(uint64_t) * 3);
                li[i].mcs_l_info->metadata[NEXT] = 0;
                li[i].mcs_l_info->metadata[NOTIFY] = 0;
                li[i].mcs_l_info->metadata[MCS_SYNC] = 0;
                li[i].mcs_l_info->num_conn = (int *)malloc(sizeof(int));
                *(li[i].mcs_l_info->num_conn) = 0;
                connect_to_mcs(ci[i].mcs_c_info->parent_address, ci[i].mcs_c_info->parent_port, ci[i].mcs_c_info->peer_addresses, ci[i]mcs_c_info->peer_ports, ci[i].mcs_c_info->num_peers, li[i].mcs_l_info->id_arr, cm_event_channel, li[i].mcs_l_info->node_id, li[i].mcs_l_info->buffer, li[i].mcs_l_info->metadata, li[i].mcs_l_info->num_conn);
                break;
            case 't':
                struct sockaddr_in ticket_sockaddr = build_sockaddr(ci[i].ticket_c_info->parent_address, ci[i].ticket_c_info->parent_port);
                li[i].ticket_l_info = (ticket_lock_info *)malloc(sizeof(ticket_lock_info));
                li[i].ticket_l_info->metadata = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].ticket_l_info->metadata) = 0;
                li[i].ticket_l_info->buffer = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].ticket_l_info->buffer) = 0;
                li[i].ticket_l_info->node_id = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].ticket_l_info->node_id) = ci[i].ticket_c_info->node_id;
                li[i].ticket_l_info->ticket = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].ticket_l_info->ticket) = 0;
                li[i].ticket_l_info->server_id = connect_to_spin_server(cm_event_channel, &ticket_sockaddr, li[i].ticket_l_info->node_id, li[i].ticket_l_info->buffer, li[i].ticket_l_info->metadata);
                wait_on_data(li[i].ticket_l_info->metadata, 1);
                *(li[i].ticket_l_info->metadata) = 0;
                break;
            case 's':
                struct sockaddr_in spin_sockaddr = build_sockaddr(ci[i].spin_c_info->parent_address, ci[i].spin_c_info->parent_port);
                li[i].spin_l_info = (spin_lock_info *)malloc(sizeof(spin_lock_info));
                li[i].spin_l_info->metadata = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].spin_l_info->metadata) = 0;
                li[i].spin_l_info->buffer = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].spin_l_info->buffer) = 0;
                li[i].spin_l_info->node_id = (uint64_t *)malloc(sizeof(uint64_t));
                *(li[i].spin_l_info->node_id) = ci[i].spin_c_info->node_id;
                li[i].spin_l_info->server_id = connect_to_spin_server(cm_event_channel, &spin_sockaddr, li[i].spin_l_info->node_id, li[i].spin_l_info->buffer, li[i].spin_l_info->metadata);
                wait_on_data(li[i].spin_l_info->metadata, 1);
                *(li[i].spin_l_info->metadata) = 0;
                break;
            default:
                //Nothing
        }
    }

    start = clock();
    for (int i = 0; i < num_aquire; i++) {
        for(int n = 0; n < noncritical_section; n++) {
            noop(&n);
        }
        
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
			case 'b':
				lockBasic(basic);
				break;
            default:
                //Nothing
        }

        for (int l = 0; l < num_parents; l++) {
            switch(parent_types[l]) {
                case 'm':
                    acquire_mcs_lock(li[i].mcs_l_info->id_arr, li[i].mcs_l_info->node_id, li[i].mcs_l_info->buffer, li[i].mcs_l_info->metadata);
                    break;
                case 't':
                    *(li[l].ticket_l_info->ticket) = acquire_ticket_lock(li[l].ticket_l_info->server_id, li[l].ticket_l_info->buffer);
                    break;
                case 's':
                    acquire_spin_lock(li[l].spin_l_info->server_id, li[l].spin_l_info->buffer);
                    break;
                default:
                    //Nothing
            }
        }

        for (int c = 0; c < critical_section; c++) {
            noop(&c);
        }

        for (int l = num_parents - 1; l >= 0; l--) {
            switch(parent_types[l]) {
                case 'm':
                    release_mcs_lock(li[i].mcs_l_info->id_arr, li[i].mcs_l_info->node_id, li[i].mcs_l_info->buffer, li[i].mcs_l_info->metadata);
                    break;
                case 't':
                    release_ticket_lock(li[l].ticket_l_info->server_id, *(li[l].ticket_l_info->ticket), li[l].ticket_l_info->buffer);
                    break;
                case 's':
                    release_spin_lock(li[l].ticket_l_info->server_id, li[l].ticket_l_info->buffer);
                    break;
                default:
                    //Nothing
            }
        }

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
			case 'b':
				unlockBasic(basic);
				break;
            default:
                //Nothing
        }
    }
    end = clock();

    for(int i = num_parents; i >= 0; i--) {
        switch(parent_types[i]) {
            case 'm':
                disconnect_from_mcs(li[i].mcs_l_info->id_arr, cm_event_channel, li[i].mcs_l_info->node_id, li[i].mcs_l_info->num_conn);
                free(li[i].mcs_l_info->id_arr);
                free(li[i].mcs_l_info->node_id);
                free(li[i].mcs_l_info->buffer);
                free(li[i].mcs_l_info->num_conn);
                free((void *)li[i].mcs_l_info->metadata);
                break;
            case 't':
                disconnect_client(cm_event_channel, li[i].ticket_l_info->server_id);
                free((void *)li[i].ticket_l_info->metadata);
                free(li[i].ticket_l_info->buffer);
                free(li[i].ticket_l_info->node_id);
                free(li[i].ticket_l_info->ticket);
                free(li[i].ticket_l_info);
                break;
            case 's':
                disconnect_client(cm_event_channel, li[i].spin_l_info->server_id);
                free((void *)li[i].spin_l_info->metadata);
                free(li[i].spin_l_info->buffer);
                free(li[i].spin_l_info->node_id);
                free(li[i].spin_l_info);
                break;
            default:
                //Nothing
        }
    }
    free(li);
    free(node_id);
    rdma_destroy_event_channel(cm_event_channel);

    printf("%f\n", ((double)(num_aquire * critical_section))/((double)(end - start)/CLOCKS_PER_SEC));
    return NULL;
}
