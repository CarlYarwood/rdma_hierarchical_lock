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
    uint64_t * node_id;
    int num_parents;
    char * parent_types;
    char ** parent_addresses;
    long * parent_ports;
    char ** peer_addresses[31];
    long * peer_ports[31];
    int * num_peers;
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