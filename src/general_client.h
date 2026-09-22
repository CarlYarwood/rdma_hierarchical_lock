#ifndef GENERAL_CLIENT_H
#define GENERAL_CLIENT_H
#include "mcs_client.h"
#include "spin_client.h"
#include "ticket_client.h"

typedef struct {
    char * parent_address;
    long parent_port;
    char ** peer_addresses;
    long * peer_ports;
    int num_peers;
    uint64_t node_id;
} mcs_client_info;

typedef struct {
    char * parent_address;
    long parent_port;
    uint64_t node_id;
} ticket_client_info;

typedef struct {
    char * parent_address;
    long parent_port;
    uint64_t node_id;
} spin_client_info;

union client_info {
    mcs_client_info * mcs_c_info;
    ticket_client_info * ticket_c_info;
    spin_client_info * spin_c_info;
};

typedef struct {
    struct rdma_cm_id ** id_arr;
    uint64_t * node_id;
    uint64_t * buffer;
    volatile uint64_t * metadata;
} mcs_lock_info;

typedef struct {
    struct rdma_cm_id * server_id;
    uint64_t * buffer;
    uint64_t * node_id;
    uint64_t * ticket;
    volatile uint64_t * metadata;
} ticket_lock_info;

typedef struct {
    struct rdma_cm_id * server_id;
    uint64_t * buffer;
    uint64_t * node_id;
    volatile uint64_t * metadata;
} spin_lock_info;

union lock_info {
    mcs_lock_info * mcs_l_info;
    ticket_lock_info * ticket_l_info;
    spin_lock_info * spin_l_info;
};

typedef struct {
    int num_parents;
    char * parent_types;
    int critical_section;
    int noncritical_section;
    int num_aquire;
    char * machine_lock_type;
    uint64_t node_id;
    union machine_lock machine_lock;
    union client_info * client_info;
} general_client_in;

#endif