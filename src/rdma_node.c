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

char * peer_addresses[4][96] = {
    {
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
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
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
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
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
        "10.10.1.2",
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
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
        "10.10.1.3",
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
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
        "10.10.1.4",
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
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
        "10.10.1.5",
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
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
        "10.10.1.6",
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
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
        "10.10.1.7",
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

long peer_ports[4][96] = {
    {
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
    },
    {
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
    },
    {
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
    },
    {
        DEFAULT_RDMA_PORT,
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
        DEFAULT_RDMA_PORT,
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
    }
};

int peer_groups[3][2] = {
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
};

int num_peers[4] = {
    96,
    32,
    32,
    32,
};

uint64_t client_ids[96][3] = {
    { 1, 1, 1 },
    { 2, 2, 2 },
    { 3, 3, 3 },
    { 4, 4, 4 },
    { 5, 5, 5 },
    { 6, 6, 6 },
    { 7, 7, 7 },
    { 8, 8, 8 },
    { 9, 9, 9 },
    { 10, 10, 10 },
    { 11, 11, 11 },
    { 12, 12, 12 },
    { 13, 13, 13 },
    { 14, 14, 14 },
    { 15, 15, 15 },
    { 16, 16, 16 },
    { 17, 17, 17 },
    { 18, 18, 18 },
    { 19, 19, 19 },
    { 20, 20, 20 },
    { 21, 21, 21 },
    { 22, 22, 22 },
    { 23, 23, 23 },
    { 24, 24, 24 },
    { 25, 25, 25 },
    { 26, 26, 26 },
    { 27, 27, 27 },
    { 28, 28, 28 },
    { 29, 29, 29 },
    { 30, 30, 30 },
    { 31, 31, 31 },
    { 32, 32, 32 },
    { 33, 1, 33 },
    { 34, 2, 34 },
    { 35, 3, 35 },
    { 36, 4, 36 },
    { 37, 5, 37 },
    { 38, 6, 38 },
    { 39, 7, 39 },
    { 40, 8, 40 },
    { 41, 9, 41 },
    { 42, 10, 42 },
    { 43, 11, 43 },
    { 44, 12, 44 },
    { 45, 13, 45 },
    { 46, 14, 46 },
    { 47, 15, 47 },
    { 48, 16, 48 },
    { 49, 17, 49 },
    { 50, 18, 50 },
    { 51, 19, 51 },
    { 52, 20, 52 },
    { 53, 21, 53 },
    { 54, 22, 54 },
    { 55, 23, 55 },
    { 56, 24, 56 },
    { 57, 25, 57 },
    { 58, 26, 58 },
    { 59, 27, 59 },
    { 60, 28, 60 },
    { 61, 29, 61 },
    { 62, 30, 62 },
    { 63, 31, 63 },
    { 64, 32, 64 },
    { 65, 1, 65 },
    { 66, 2, 66 },
    { 67, 3, 67 },
    { 68, 4, 68 },
    { 69, 5, 69 },
    { 70, 6, 70 },
    { 71, 7, 71 },
    { 72, 8, 72 },
    { 73, 9, 73 },
    { 74, 10, 74 },
    { 75, 11, 75 },
    { 76, 12, 76 },
    { 77, 13, 77 },
    { 78, 14, 78 },
    { 79, 15, 79 },
    { 80, 16, 80 },
    { 81, 17, 81 },
    { 82, 18, 82 },
    { 83, 19, 83 },
    { 84, 20, 84 },
    { 85, 21, 85 },
    { 86, 22, 86 },
    { 87, 23, 87 },
    { 88, 24, 88 },
    { 89, 25, 89 },
    { 90, 26, 90 },
    { 91, 27, 91 },
    { 92, 28, 92 },
    { 93, 29, 93 },
    { 94, 30, 94 },
    { 95, 31, 95 },
    { 96, 32, 96 },
};

server_config servers[2][1] = {
    {
        { 't', 96, DEFAULT_RDMA_PORT },
    },
    {
        {'t', 32, DEFAULT_RDMA_PORT + 30}
    }
};

client_config clients[6][24] = {
    {
        { client_ids[0], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[1], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 1 },
        { client_ids[2], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 2 },
        { client_ids[3], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 3 },
        { client_ids[4], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 4 },
        { client_ids[5], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 5 },
        { client_ids[6], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 6 },
        { client_ids[7], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 7 },
        { client_ids[8], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 8 },
        { client_ids[9], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 9 },
        { client_ids[10], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 10 },
        { client_ids[11], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 11 },
        { client_ids[12], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 12 },
        { client_ids[13], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 13 },
        { client_ids[14], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 14 },
        { client_ids[15], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 15 },
    },
    {
        { client_ids[16], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 16 },
        { client_ids[17], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 17 },
        { client_ids[18], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 18 },
        { client_ids[19], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 19 },
        { client_ids[20], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 20 },
        { client_ids[21], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 21 },
        { client_ids[22], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 22 },
        { client_ids[23], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 23 },
        { client_ids[24], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 24 },
        { client_ids[25], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 25 },
        { client_ids[26], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 26 },
        { client_ids[27], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 27 },
        { client_ids[28], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 28 },
        { client_ids[29], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 29 },
        { client_ids[30], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 30 },
        { client_ids[31], 2, "tt", parent_addresses[0], parent_ports[0], peer_groups[0], 31 },
    },
    {
        { client_ids[32], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 32 },
        { client_ids[33], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 33 },
        { client_ids[34], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 34 },
        { client_ids[35], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 35 },
        { client_ids[36], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 36 },
        { client_ids[37], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 37 },
        { client_ids[38], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 38 },
        { client_ids[39], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 39 },
        { client_ids[40], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 40 },
        { client_ids[41], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 41 },
        { client_ids[42], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 42 },
        { client_ids[43], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 43 },
        { client_ids[44], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 44 },
        { client_ids[45], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 45 },
        { client_ids[46], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 46 },
        { client_ids[47], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 47 },
    },
    {
        { client_ids[48], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 48 },
        { client_ids[49], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 49 },
        { client_ids[50], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 50 },
        { client_ids[51], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 51 },
        { client_ids[52], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 52 },
        { client_ids[53], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 53 },
        { client_ids[54], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 54 },
        { client_ids[55], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 55 },
        { client_ids[56], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 56 },
        { client_ids[57], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 57 },
        { client_ids[58], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 58 },
        { client_ids[59], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 59 },
        { client_ids[60], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 60 },
        { client_ids[61], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 61 },
        { client_ids[62], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 62 },
        { client_ids[63], 2, "tt", parent_addresses[1], parent_ports[0], peer_groups[1], 63 },   
    },
    {
        { client_ids[64], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 64 },
        { client_ids[65], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 65 },
        { client_ids[66], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 66 },
        { client_ids[67], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 67 },
        { client_ids[68], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 68 },
        { client_ids[69], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 69 },
        { client_ids[70], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 70 },
        { client_ids[71], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 71 },
        { client_ids[72], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 72 },
        { client_ids[73], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 73 },
        { client_ids[74], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 74 },
        { client_ids[75], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 75 },
        { client_ids[76], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 76 },
        { client_ids[77], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 77 },
        { client_ids[78], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 78 },
        { client_ids[79], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 79 },
    },
    {
        { client_ids[80], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 80 },
        { client_ids[81], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 81 },
        { client_ids[82], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 82 },
        { client_ids[83], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 83 },
        { client_ids[84], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 84 },
        { client_ids[85], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 85 },
        { client_ids[86], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 86 },
        { client_ids[87], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 87 },
        { client_ids[88], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 88 },
        { client_ids[89], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 89 },
        { client_ids[90], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 90 },
        { client_ids[91], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 91 },
        { client_ids[92], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 92 },
        { client_ids[93], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 93 },
        { client_ids[94], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 94 },
        { client_ids[95], 2, "tt", parent_addresses[2], parent_ports[0], peer_groups[2], 95 },
    },
};

node_config configs[7] = {
    {
        1,
        1,
        0,
        96,
        "n",
        servers[0],
        NULL,
    },
    {
        17,
        1,
        16,
        96,
        "n",
        servers[1],
        clients[0],
    },
    {
        16,
        0,
        16,
        96,
        "n",
        NULL,
        clients[1],
    },
    {
        17,
        1,
        16,
        96,
        "n",
        servers[1],
        clients[2],
    },
    {
        16,
        0,
        16,
        96,
        "n",
        NULL,
        clients[3],
    },
    {
        17,
        1,
        16,
        96,
        "n",
        servers[1],
        clients[4],
    },
    {
        16,
        0,
        16,
        96,
        "n",
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
