#include "rdma_node.h"

// Valid Clusters
// Utah xl170, c6525-100g c6525-25g
// Wisconsin c220g5 c240g5
// Clemson c6420

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 10000;

char * parent_addresses[1][1] = {
    {
        "10.10.1.1",
    }
};

char * peer_addresses[1][48] = {
    {
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
    }
};

long parent_ports[1][1] = {
    {
        DEFAULT_RDMA_PORT,
    }
};

long peer_ports[1][48] = {
    {
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
        DEFAULT_RDMA_PORT,
        DEFAULT_RDMA_PORT + 1,
        DEFAULT_RDMA_PORT + 2,
        DEFAULT_RDMA_PORT + 3,
        DEFAULT_RDMA_PORT + 4,
        DEFAULT_RDMA_PORT + 5,
        DEFAULT_RDMA_PORT + 6,
        DEFAULT_RDMA_PORT + 7,
    }
};

int peer_groups[1][1] = {
    { 0 },
};

int num_peers[1] = {
    48,
};

uint64_t client_ids[48][2] = {
    { 1, 1 },
    { 2, 2 },
    { 3, 3 },
    { 4, 4 },
    { 5, 5 },
    { 6, 6 },
    { 7, 7 },
    { 8, 8 },
    { 9, 9 },
    { 10, 10 },
    { 11, 11 },
    { 12, 12 },
    { 13, 13 },
    { 14, 14 },
    { 15, 15 },
    { 16, 16 },
    { 17, 17 },
    { 18, 18 },
    { 19, 19 },
    { 20, 20 },
    { 21, 21 },
    { 22, 22 },
    { 23, 23 },
    { 24, 24 },
    { 25, 25 },
    { 26, 26 },
    { 27, 27 },
    { 28, 28 },
    { 29, 29 },
    { 30, 30 },
    { 31, 31 },
    { 32, 32 },
    { 33, 33 },
    { 34, 34 },
    { 35, 35 },
    { 36, 36 },
    { 37, 37 },
    { 38, 38 },
    { 39, 39 },
    { 40, 40 },
    { 41, 41 },
    { 42, 42 },
    { 43, 43 },
    { 44, 44 },
    { 45, 45 },
    { 46, 46 },
    { 47, 47 },
    { 48, 48 },
};

server_config servers[1] = {
    { 's', 48, DEFAULT_RDMA_PORT },
};

client_config clients[6][8] = {
    {
        { client_ids[0], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[1], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[2], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[3], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[4], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[5], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[6], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[7], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
    },
    {
        { client_ids[8], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[9], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[10], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[11], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[12], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[13], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[14], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[15], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
    },
    {
        { client_ids[16], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[17], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[18], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[19], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[20], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[21], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[22], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[23], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
    },
    {
        { client_ids[24], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[25], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[26], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[27], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[28], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[29], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[30], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[31], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
    },
    {
        { client_ids[32], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[33], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[34], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[35], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[36], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[37], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[38], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[39], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
    },
    {
        { client_ids[40], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[41], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[42], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[43], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[44], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[45], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[46], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[47], 1, "s", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
    },
};

node_config configs[7] = {
    {
        1,
        1,
        0,
        1,
        "n",
        servers,
        NULL,
    },
    {
        8,
        0,
        8,
        1,
        "m",
        NULL,
        clients[0],
    },
    {
        8,
        0,
        8,
        1,
        "m",
        NULL,
        clients[1],
    },
    {
        8,
        0,
        8,
        1,
        "m",
        NULL,
        clients[2],
    },
    {
        8,
        0,
        8,
        1,
        "m",
        NULL,
        clients[3],
    },
    {
        8,
        0,
        8,
        1,
        "m",
        NULL,
        clients[4],
    },
    {
        8,
        0,
        8,
        1,
        "m",
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
