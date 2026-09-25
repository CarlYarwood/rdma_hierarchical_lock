#ifndef RDMA_NODE_H_
#define RDMA_NODE_H_
#include "general_client.h"
#include "mcs_server.h"
#include "ticket_server.h"
#include "spin_server.h"

typedef struct {
    char server_type;
    int num_children;
} server_config;

typedef struct {
    uint64_t node_id[2];
    int num_parents;
    char * parent_types;
    char * parent_addresses[1];
    long parent_ports[1];
    char * peer_addresses[1][31];
    long * peer_ports[1][31];
    int num_peers[1];
} client_config;

typedef struct {
    int total_workers;
    int num_servers;
    int num_clients;
    char * machine_lock_type;
    server_config * servers;
    client_config * clients;
} node_config;
#endif