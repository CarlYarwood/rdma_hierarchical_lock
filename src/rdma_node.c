#include "rdma_node.h"

// Valid Clusters
// Utah xl170, c6525-100g c6525-25g
// Wisconsin c220g5 c240g5
// Clemson c6420

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 1000;

char * parent_addresses[1][1] = {
    {"10.10.1.1"}
};

char * peer_addresses[1][31] = {
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
        "10.10.1.1"
    }
};

long parent_ports[1][1] = {
    {DEFAULT_RDMA_PORT}
};

long peer_ports[1][31] = {
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
        DEFAULT_RDMA_PORT + 19,
    }
};

int peer_groups[1][1] = {
    { 0 }
};

int num_peers[1] = {
    19
};

uint64_t client_ids[31][2] = {
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
    { 19, 19 }
};

server_config servers[2] = {
    { 'm', 19}
};

client_config clients[31] = {
    { client_ids[0], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[1], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[2], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[3], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[4], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[5], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[6], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[7], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[8], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[9], 1, "m", parent_addresses[0], parent_ports[0],  peer_groups[0]},
    { client_ids[10], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[11], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[12], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[13], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[14], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[15], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[16], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[17], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
    { client_ids[18], 1, "m", parent_addresses[0], parent_ports[0], peer_groups[0] },
};

node_config configs[1] = {
    {
        20,
        1,
        19,
        "s",
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

    while ((option = getopt(argc, argv, "c:p:")) != -1) {
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
        c_in[i].node_id = clients[i].node_id[0];
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
    destroyMcsLock(mcs);
    destroyTicketLock(ticket);
    destroySpinLock(spin);
    destroyBasicLock(basic);
    free(c_in);
    free(workers);
    return 0;
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
