#include "rdma_node.h"

// Valid Clusters
// Utah xl170, c6525-100g c6525-25g
// Wisconsin c220g5 c240g5
// Clemson c6420

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 1000;

char * addresses[2][31] = {
    {"10.10.1.1"},
    {
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1",
        "10.10.1.1"
    }
};

long ports[2][31] = {
    {DEFAULT_RDMA_PORT},
    {
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
        DEFAULT_RDMA_PORT + 8,
        DEFAULT_RDMA_PORT + 9,
        DEFAULT_RDMA_PORT + 10,
        DEFAULT_RDMA_PORT + 11,
        DEFAULT_RDMA_PORT + 12,
        DEFAULT_RDMA_PORT + 13,
        DEFAULT_RDMA_PORT + 14,
        DEFAULT_RDMA_PORT + 15,
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
        DEFAULT_RDMA_PORT + 24,
        DEFAULT_RDMA_PORT + 25,
        DEFAULT_RDMA_PORT + 26,
        DEFAULT_RDMA_PORT + 27,
        DEFAULT_RDMA_PORT + 28,
        DEFAULT_RDMA_PORT + 29,
        DEFAULT_RDMA_PORT + 30,
        DEFAULT_RDMA_PORT + 31,
    }
};

int peer_group_sizes[2] = {
    1,
    31
};

server_config servers[1] = {
    't',
    31
};

client_config clients[31] = {
    { 1, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 2, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 3, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 4, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 5, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 6, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 7, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 8, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 9, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 10, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 11, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 12, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 13, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 14, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 15, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 16, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 17, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 18, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 19, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 20, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 21, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 22, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 23, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 24, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 25, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 26, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 27, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 28, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 29, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 30, 1, "t", addresses[0], ports[0], NULL, NULL },
    { 31, 1, "t", addresses[0], ports[0], NULL, NULL }
};

node_config configs[1] = {
    {
        32,
        1,
        31,
        "b",
        servers,
        clients
    }
};

int main(int argc, char ** argv){
    int option = 0;
    int choice = -1;
    int index = 0;
    pthread_t * workers;
    node_config * config;

    while ((option = getopt(argc, argv, "n:")) != -1) {
		switch (option) {
            case 'n':
                choice = atoi(optarg);
                break;
            default:
                printf("invalid option detected\n");
                return -1;
		}
	}
    config = &configs[choice];

    workers = (pthread_t *)malloc(sizeof(pthread_t) * config->total_workers);
    server_config * servers = config->servers;
    server_in * s_in = (server_in *)malloc(sizeof(server_in) * config->num_servers);
    for (int i = 0; i < config->num_servers; i++) {
        s_in[i].num_children = servers[i].num_children;
        switch(servers[i].server_type) {
            case 'm':
                // Nothing for Now
                break;
            case 't':
                pthread_create(&workers[i], NULL, ticket_server, (void*) &s_in[i]);
                break;
            case 's':
                pthread_create(&workers[i], NULL, spin_server, (void *) &s_in[i]);
                break;
            default:
                // Nothing
        }
        index++;
    }

    mcsLock* mcs = buildMcsLock();
    ticketLock* ticket = buildTicketLock();
    spinLock* spin = buildSpinLock();
    basicLock * basic = buildBasicLock();
    client_config * clients = config->clients;
    general_client_in * c_in = malloc(sizeof(general_client_in) * config->num_clients);
    for (int i = 0; i < config->num_clients; i++) {
        c_in[i].num_parents = clients[i].num_parents;
        c_in[i].parent_types = clients[i].parent_types;
        c_in[i].critical_section = critical_section;
        c_in[i].noncritical_section = noncritical_section;
        c_in[i].num_aquire = num_aquire;
        c_in[i].node_id = clients[i].node_id;
        c_in[i].machine_lock_type = config->machine_lock_type;
        switch(*(config->machine_lock_type)) {
            case 'm':
                c_in[i].machine_lock.mcs = mcs;
                break;
            case 't':
                c_in[i].machine_lock.ticket = ticket;
                break;
            case 's':
                c_in[i].machine_lock.spin = spin;
                break;
            case 'b':
                c_in[i].machine_lock.basic = basic;
                break;
            default:
                //Nothing
        }
        c_in[i].client_info = (union client_info *)malloc(sizeof(union client_info) * clients[i].num_parents);
        for(int p = 0; p < clients[i].num_parents ; p++) {
            switch(clients[i].parent_types[p]) {
                case 'm':
                    //Do Nothing for now
                    break;
                case 't':
                    c_in[i].client_info[p].ticket_c_info = (ticket_client_info *)malloc(sizeof(ticket_client_info));
                    c_in[i].client_info[p].ticket_c_info->parent_address = clients[i].parent_addresses[p];
                    c_in[i].client_info[p].ticket_c_info->parent_port = clients[i].parent_ports[p];
                    c_in[i].client_info[p].ticket_c_info->node_id = clients[i].node_id;
                    break;
                case 's':
                    c_in[i].client_info[p].spin_c_info = (spin_client_info *)malloc(sizeof(spin_client_info));
                    c_in[i].client_info[p].spin_c_info->parent_address = clients[i].parent_addresses[p];
                    c_in[i].client_info[p].spin_c_info->parent_port = clients[i].parent_ports[p];
                    c_in[i].client_info[p].spin_c_info->node_id = clients[i].node_id;
                    break;
                default:
                    //Nothing
            }
        }
        pthread_create(&workers[index], NULL, general_client, (void *) &c_in[i]);
        index++;
    }

    for(int i = (config->total_workers) - 1; i >= 0; i--) {
        pthread_join(workers[i], NULL);
    }

    free(s_in);
    for(int i = 0; i < config->num_clients; i++) {
        for(int p = 0; p < clients[i].num_parents; p++) {
            switch(clients[i].parent_types[p]) {
                case 'm':
                    //Do Nothing for now
                    break;
                case 't':
                    free(c_in[i].client_info[p].ticket_c_info);
                    break;
                case 's':
                    free(c_in[i].client_info[p].spin_c_info);
                    break;
                default:
                    //Nothing
            }
        }
        free(c_in[i].client_info);
    }
    free(c_in);
    free(workers);

    // mcsLock* mcs = buildMcsLock();
    // ticketLock* ticket = buildTicketLock();
    // spinLock* spin = buildSpinLock();
    // basicLock* basic = buildBasicLock();

    // switch(*parent_lock_type) {
    //     case 'm':
    //         printf("in client mcs\n");
    //         workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
    //         mcs_client_in * mcs_in = (mcs_client_in *)malloc(sizeof(mcs_client_in) * num_workers);
    //         for (int i = 0; i < num_workers; i++) {
    //             mcs_in[i].node_id = node_id_start + (uint64_t) i;
    //             mcs_in[i].critical_section = critical_section;
    //             mcs_in[i].noncritical_section = noncritical_section;
    //             mcs_in[i].num_aquire = num_aquire;
    //             mcs_in[i].parent_address = addresses[parent_peer_group][parent_id];
    //             mcs_in[i].parent_port = ports[parent_peer_group][parent_id];
    //             mcs_in[i].peer_addresses = addresses[node_peer_group];
    //             mcs_in[i].peer_ports = ports[node_peer_group];
    //             mcs_in[i].num_peers = peer_group_sizes[node_peer_group];
    //             mcs_in[i].machine_lock_type = machine_lock_type;
    //             switch (*machine_lock_type) {
    //                 case 'm':
    //                     mcs_in[i].machine_lock.mcs = mcs;
    //                     break;
    //                 case 't':
    //                     mcs_in[i].machine_lock.ticket = ticket;
    //                     break;
    //                 case 's':
    //                     mcs_in[i].machine_lock.spin = spin;
    //                     break;
    //                 case 'b':
    //                     mcs_in[i].machine_lock.basic = basic;
    //                     break;
    //                 default:
    //                     // do nothing
    //             }
    //             pthread_create(&workers[i], NULL, mcs_client, (void *)&mcs_in[i]);
    //         }

    //         for(int i = 0; i < num_workers; i++) {
    //             pthread_join(workers[i], NULL);
    //         }
    //         destroyMcsLock(mcs);
    //         destroyTicketLock(ticket);
    //         destroySpinLock(spin);
    //         destroyBasicLock(basic);
    //         free(mcs_in);
    //         free(workers);
    //         return 0;
    //     case 't':
    //         printf("in client ticket\n");
    //         workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
    //         ticket_client_in * ticket_in = (ticket_client_in *)malloc(sizeof(ticket_client_in) * num_workers);
    //         for(int i = 0; i < num_workers; i++) {
    //             ticket_in[i].node_id = node_id_start + (uint64_t) i;
    //             ticket_in[i].parent_address = addresses[parent_peer_group][parent_id];
    //             ticket_in[i].parent_port = ports[parent_peer_group][parent_id];
    //             ticket_in[i].critical_section = critical_section;
    //             ticket_in[i].noncritical_section = noncritical_section;
    //             ticket_in[i].num_aquire = num_aquire;
    //             ticket_in[i].machine_lock_type = machine_lock_type;
    //             switch (*machine_lock_type) {
    //                 case 'm':
    //                     ticket_in[i].machine_lock.mcs = mcs;
    //                     break;
    //                 case 't':
    //                     ticket_in[i].machine_lock.ticket = ticket;
    //                     break;
    //                 case 's':
    //                     ticket_in[i].machine_lock.spin = spin;
    //                     break;
    //                 case 'b':
    //                     ticket_in[i].machine_lock.basic = basic;
    //                     break;
    //                 default:
    //                     // do nothing
    //             }
    //             pthread_create(&workers[i], NULL, ticket_client, (void *)&ticket_in[i]);
    //         }

    //         for(int i = 0; i < num_workers; i++) {
    //             pthread_join(workers[i], NULL);
    //         }
    //         destroyMcsLock(mcs);
    //         destroyTicketLock(ticket);
    //         destroySpinLock(spin);
    //         destroyBasicLock(basic);
    //         free(ticket_in);
    //         free(workers);
    //         return 0;
    //     case 's':
    //         printf("in client spin\n");
    //         workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
    //         spin_client_in * spin_in = (spin_client_in *)malloc(sizeof(spin_client_in) * num_workers);
    //         for (int i = 0; i < num_workers ; i++) {
    //             spin_in[i].machine_lock_type = machine_lock_type;
    //             spin_in[i].parent_address = addresses[parent_peer_group][parent_id];
    //             spin_in[i].parent_port = ports[parent_peer_group][parent_id];
    //             spin_in[i].node_id = node_id_start + (uint64_t) i;
    //             spin_in[i].critical_section = critical_section;
    //             spin_in[i].noncritical_section = noncritical_section;
    //             spin_in[i].num_aquire = num_aquire;
    //             switch (*machine_lock_type) {
    //                 case 'm':
    //                     spin_in[i].machine_lock.mcs = mcs;
    //                     break;
    //                 case 't':
    //                     spin_in[i].machine_lock.ticket = ticket;
    //                     break;
    //                 case 's':
    //                     spin_in[i].machine_lock.spin = spin;
    //                     break;
    //                 case 'b':
    //                     spin_in[i].machine_lock.basic = basic;
    //                     break;
    //                 default:
    //                     // do nothing
    //             }
    //             pthread_create(&workers[i], NULL, spin_client, (void *)&spin_in[i]);
    //         }

    //         for(int i = 0; i < num_workers; i++) {
    //             pthread_join(workers[i], NULL);
    //         }
    //         destroyMcsLock(mcs);
    //         destroyTicketLock(ticket);
    //         destroySpinLock(spin);
    //         destroyBasicLock(basic);
    //         free(spin_in);
    //         free(workers);
    //         return 0;
    //     default:
    //         destroyMcsLock(mcs);
    //         destroyTicketLock(ticket);
    //         destroySpinLock(spin);
    //         destroyBasicLock(basic);
    // }

    // server_in * in = (server_in *)malloc(sizeof(server_in));
    // switch(*local_lock_type) {
    //     case 'm':
    //         printf("in server mcs\n");
    //         workers = (pthread_t *) malloc(sizeof(pthread_t));
    //         in->num_children = peer_group_sizes[child_peer_group];
    //         pthread_create(workers, NULL, mcs_server, (void *)in);
    //         pthread_join(*workers, NULL);
    //         free(in);
    //         free(workers);
    //         return 0;
    //     case 't':
    //         printf("in server ticket\n");
    //         workers = (pthread_t *) malloc(sizeof(pthread_t));
    //         in->num_children = peer_group_sizes[child_peer_group];
    //         pthread_create(workers, NULL, ticket_server, (void *)in);
    //         pthread_join(*workers, NULL);
    //         free(in);
    //         free(workers);
    //         return 0;
    //     case 's':
    //         printf("in server spin\n");
    //         workers = (pthread_t *) malloc(sizeof(pthread_t));
    //         in->num_children = peer_group_sizes[child_peer_group];
    //         pthread_create(workers, NULL, spin_server, (void *)in);
    //         pthread_join(*workers, NULL);
    //         free(in);
    //         free(workers);
    //         return 0;
    //     default:
    //         free(in);
    // }
    return 0;
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
