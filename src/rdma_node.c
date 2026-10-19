#include "rdma_node.h"

// Valid Clusters
// Utah xl170, c6525-100g c6525-25g
// Wisconsin c220g5 c240g5
// Clemson c6420

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 10000;

char * parent_addresses[3][2] = {
    {
        "10.10.1.2",
        "10.10.1.1",
    },
    {
        "10.10.1.4",
        "10.10.1.1",
    },
    {
        "10.10.1.6",
        "10.10.1.1",
    }
};

char * peer_addresses[4][24] = {
    {
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
    },
    {
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
    },
    {
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
    },
    {
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
    },
};

long parent_ports[1][2] = {
    {
        DEFAULT_RDMA_PORT + 30,
        DEFAULT_RDMA_PORT,
    }
};

long peer_ports[4][24] = {
    {
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
    },
    {
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
    },
    {
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
    },
    {
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
    }
};

int peer_groups[3][2] = {
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
};

int num_peers[4] = {
    24,
    8,
    8,
    8,
};

uint64_t client_ids[24][3] = {
    { 1, 1, 1 },
    { 2, 2, 2 },
    { 3, 3, 3 },
    { 4, 4, 4 },
    { 5, 5, 5 },
    { 6, 6, 6 },
    { 7, 7, 7 },
    { 8, 8, 8 },
    { 9, 1, 9 },
    { 10, 2, 10 },
    { 11, 3, 11 },
    { 12, 4, 12 },
    { 13, 5, 13 },
    { 14, 6, 14 },
    { 15, 7, 15 },
    { 16, 8, 16 },
    { 17, 1, 17 },
    { 18, 2, 18 },
    { 19, 3, 19 },
    { 20, 4, 20 },
    { 21, 5, 21 },
    { 22, 6, 22 },
    { 23, 7, 23 },
    { 24, 8, 24 },
};

server_config servers[2][1] = {
    {
        { 's', 24, DEFAULT_RDMA_PORT },
    },
    {
        {'t', 8, DEFAULT_RDMA_PORT + 30}
    }
};

client_config clients[6][4] = {
    {
        { client_ids[0], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[1], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 1 },
        { client_ids[2], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 2 },
        { client_ids[3], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 3 },
    },
    {
        { client_ids[4], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 4 },
        { client_ids[5], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 5 },
        { client_ids[6], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 6 },
        { client_ids[7], 2, "ts", parent_addresses[0], parent_ports[0], peer_groups[0], 7 },
    },
    {
        { client_ids[8], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 8 },
        { client_ids[9], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 9 },
        { client_ids[10], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 10 },
        { client_ids[11], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 11 },
    },
    {
        { client_ids[12], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 12 },
        { client_ids[13], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 13 },
        { client_ids[14], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 14 },
        { client_ids[15], 2, "ts", parent_addresses[1], parent_ports[0], peer_groups[1], 15 },
    },
    {
        { client_ids[16], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 16 },
        { client_ids[17], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 17 },
        { client_ids[18], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 18 },
        { client_ids[19], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 19 },
    },
    {
        { client_ids[20], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 20 },
        { client_ids[21], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 21 },
        { client_ids[22], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 22 },
        { client_ids[23], 2, "ts", parent_addresses[2], parent_ports[0], peer_groups[2], 23 },
    },
};

node_config configs[7] = {
    {
        1,
        1,
        0,
        24,
        "n",
        servers[0],
        NULL,
    },
    {
        5,
        1,
        4,
        24,
        "s",
        servers[1],
        clients[0],
    },
    {
        4,
        0,
        4,
        24,
        "s",
        NULL,
        clients[1],
    },
    {
        5,
        1,
        4,
        24,
        "s",
        servers[1],
        clients[2],
    },
    {
        4,
        0,
        4,
        24,
        "s",
        NULL,
        clients[3],
    },
    {
        5,
        1,
        4,
        24,
        "s",
        servers[1],
        clients[4],
    },
    {
        4,
        0,
        4,
        24,
        "s",
        NULL,
        clients[5],
    },
};

int main(int argc, char ** argv){
    int option = 0;
    int choice = -1;
    int index = 0;
    pthread_t * workers;
    node_config * config;

    while ((option = getopt(argc, argv, "c:")) != -1) {
		switch (option) {
            case 'c':
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
        s_in[i].num_locks = config->num_locks;
        s_in[i].port = servers[i].port;
        switch(servers[i].server_type) {
            case 'm':
                pthread_create(&workers[i], NULL, mcs_server, (void *) &s_in[i]);
                break;
            case 't':
                pthread_create(&workers[i], NULL, ticket_server, (void *) &s_in[i]);
                break;
            case 's':
                pthread_create(&workers[i], NULL, spin_server, (void *) &s_in[i]);
                break;
            default:
                // Nothing
        }
        index++;
    }

    mcsLock ** mcs_arr = NULL;
    ticketLock ** ticket_arr = NULL;
    spinLock ** spin_arr = NULL;
    basicLock ** basic_arr = NULL;\
    switch(*(config->machine_lock_type)) {
        case 'm':
            mcs_arr = (mcsLock **)malloc(sizeof(mcsLock *) * config->num_locks);
            for (int i = 0; i < config->num_locks; i++) {
                mcs_arr[i] = buildMcsLock();
            }
            break;
        case 't':
            ticket_arr = (ticketLock **)malloc(sizeof(ticketLock *) * config->num_locks);
            for (int i = 0; i < config->num_locks; i++) {
                ticket_arr[i] = buildTicketLock();
            }
            break;
        case 's':
            spin_arr = (spinLock **)malloc(sizeof(spinLock *) * config->num_locks);
            for (int i = 0; i < config->num_locks; i++) {
                spin_arr[i] = buildSpinLock();
            }
            break;
        case 'b':
            basic_arr = (basicLock **)malloc(sizeof(basicLock *) * config->num_locks);
            for (int i = 0; i < config->num_locks; i++) {
                basic_arr[i] = buildBasicLock();
            }
            break;
        default:
            //Nothing
    }
    client_config * clients = config->clients;
    general_client_in * c_in = malloc(sizeof(general_client_in) * config->num_clients);
    if(config->num_clients > 0) {
        printf("node, Throughput (ops/sec), Average Latency(sec)\n");
    }
    for (int i = 0; i < config->num_clients; i++) {
        c_in[i].num_parents = clients[i].num_parents;
        c_in[i].parent_types = clients[i].parent_types;
        c_in[i].critical_section = critical_section;
        c_in[i].noncritical_section = noncritical_section;
        c_in[i].num_aquire = num_aquire;
        c_in[i].node_id = clients[i].node_id[0];
        c_in[i].target_lock = clients[i].target_lock;
        c_in[i].machine_lock_type = config->machine_lock_type;
        switch(*(config->machine_lock_type)) {
            case 'm':
                c_in[i].machine_lock.mcs_arr = mcs_arr;
                break;
            case 't':
                c_in[i].machine_lock.ticket_arr = ticket_arr;
                break;
            case 's':
                c_in[i].machine_lock.spin_arr = spin_arr;
                break;
            case 'b':
                c_in[i].machine_lock.basic_arr = basic_arr;
                break;
            default:
                //Nothing
        }
        c_in[i].client_info = (union client_info *)malloc(sizeof(union client_info) * clients[i].num_parents);
        for(int p = 0; p < clients[i].num_parents ; p++) {
            switch(clients[i].parent_types[p]) {
                case 'm':
                    c_in[i].client_info[p].mcs_c_info = (mcs_client_info *)malloc(sizeof(mcs_client_info));
                    c_in[i].client_info[p].mcs_c_info->parent_address = clients[i].parent_addresses[p];
                    c_in[i].client_info[p].mcs_c_info->parent_port = clients[i].parent_ports[p];
                    c_in[i].client_info[p].mcs_c_info->peer_addresses = peer_addresses[clients[i].peer_groups[p]];
                    c_in[i].client_info[p].mcs_c_info->peer_ports = peer_ports[clients[i].peer_groups[p]];
                    c_in[i].client_info[p].mcs_c_info->num_peers = num_peers[clients[i].peer_groups[p]];
                    c_in[i].client_info[p].mcs_c_info->node_id = clients[i].node_id[p + 1];
                    break;
                case 't':
                    c_in[i].client_info[p].ticket_c_info = (ticket_client_info *)malloc(sizeof(ticket_client_info));
                    c_in[i].client_info[p].ticket_c_info->parent_address = clients[i].parent_addresses[p];
                    c_in[i].client_info[p].ticket_c_info->parent_port = clients[i].parent_ports[p];
                    c_in[i].client_info[p].ticket_c_info->node_id = clients[i].node_id[p + 1];
                    break;
                case 's':
                    c_in[i].client_info[p].spin_c_info = (spin_client_info *)malloc(sizeof(spin_client_info));
                    c_in[i].client_info[p].spin_c_info->parent_address = clients[i].parent_addresses[p];
                    c_in[i].client_info[p].spin_c_info->parent_port = clients[i].parent_ports[p];
                    c_in[i].client_info[p].spin_c_info->node_id = clients[i].node_id[p + 1];
                    break;
                default:
                    //Nothing
            }
        }
        pthread_create(&workers[index], NULL, general_client, (void *) &c_in[i]);
        index++;
    }

    for(int i = 0; i < config->total_workers; i++) {
        pthread_join(workers[i], NULL);
    }

    free(s_in);
    for(int i = 0; i < config->num_clients; i++) {
        for(int p = 0; p < clients[i].num_parents; p++) {
            switch(clients[i].parent_types[p]) {
                case 'm':
                    free(c_in[i].client_info[p].mcs_c_info);
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
    switch(*(config->machine_lock_type)) {
        case 'm' :
            for(int i = 0; i < config->num_locks; i++) {
                destroyMcsLock(mcs_arr[i]);
            }
            free(mcs_arr);
            break;
        case 't' :
            for(int i = 0; i < config->num_locks; i++) {
                destroyTicketLock(ticket_arr[i]);
            }
            free(ticket_arr);
            break;
        case 's' :
            for(int i = 0; i < config->num_locks; i++) {
                destroySpinLock(spin_arr[i]);
            }
            free(spin_arr);
            break;
        case 'b' :
            for(int i = 0; i < config->num_locks; i++) {
                destroyBasicLock(basic_arr[i]);
            }
            free(basic_arr);
            break;
        default:
            //Nothing
    }
    free(c_in);
    free(workers);
    return 0;
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
