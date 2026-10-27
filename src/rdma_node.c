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

char * peer_addresses[4][180] = {
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

long peer_ports[4][180] = {
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
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
        DEFAULT_RDMA_PORT + 16,
        DEFAULT_RDMA_PORT + 17,
        DEFAULT_RDMA_PORT + 18,
        DEFAULT_RDMA_PORT + 19,
        DEFAULT_RDMA_PORT + 20,
        DEFAULT_RDMA_PORT + 21,
        DEFAULT_RDMA_PORT + 22,
        DEFAULT_RDMA_PORT + 23,
    }
};

int peer_groups[3][2] = {
    { 1, 0 },
    { 2, 0 },
    { 3, 0 },
};

int num_peers[4] = {
    144,
    48,
    48,
    48,
};

uint64_t client_ids[180][3] = {
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
    { 33, 33, 33 },
    { 34, 34, 34 },
    { 35, 35, 35 },
    { 36, 36, 36 },
    { 37, 37, 37 },
    { 38, 38, 38 },
    { 39, 39, 39 },
    { 40, 40, 40 },
    { 41, 41, 41 },
    { 42, 42, 42 },
    { 43, 43, 43 },
    { 44, 44, 44 },
    { 45, 45, 45 },
    { 46, 46, 46 },
    { 47, 47, 47 },
    { 48, 48, 48 },
    { 49, 1, 49 },
    { 50, 2, 50 },
    { 51, 3, 51 },
    { 52, 4, 52 },
    { 53, 5, 53 },
    { 54, 6, 54 },
    { 55, 7, 55 },
    { 56, 8, 56 },
    { 57, 9, 57 },
    { 58, 10, 58 },
    { 59, 11, 59 },
    { 60, 12, 60 },
    { 61, 13, 61 },
    { 62, 14, 62 },
    { 63, 15, 63 },
    { 64, 16, 64 },
    { 65, 17, 65 },
    { 66, 18, 66 },
    { 67, 19, 67 },
    { 68, 20, 68 },
    { 69, 21, 69 },
    { 70, 22, 70 },
    { 71, 23, 71 },
    { 72, 24, 72 },
    { 73, 25, 73 },
    { 74, 26, 74 },
    { 75, 27, 75 },
    { 76, 28, 76 },
    { 77, 29, 77 },
    { 78, 30, 78 },
    { 79, 31, 79 },
    { 80, 32, 80 },
    { 81, 33, 81 },
    { 82, 34, 82 },
    { 83, 35, 83 },
    { 84, 36, 84 },
    { 85, 37, 85 },
    { 86, 38, 86 },
    { 87, 39, 87 },
    { 88, 40, 88 },
    { 89, 41, 89 },
    { 90, 42, 90 },
    { 91, 43, 91 },
    { 92, 44, 92 },
    { 93, 45, 93 },
    { 94, 46, 94 },
    { 95, 47, 95 },
    { 96, 48, 96 },
    { 97, 1, 97 },
    { 98, 2, 98 },
    { 99, 3, 99 },
    { 100, 4, 100 },
    { 101, 5, 101 },
    { 102, 6, 102 },
    { 103, 7, 103 },
    { 104, 8, 104 },
    { 105, 9, 105 },
    { 106, 10, 106 },
    { 107, 11, 107 },
    { 108, 12, 108 },
    { 109, 13, 109 },
    { 110, 14, 110 },
    { 111, 15, 111 },
    { 112, 16, 112 },
    { 113, 17, 113 },
    { 114, 18, 114 },
    { 115, 19, 115 },
    { 116, 20, 116 },
    { 117, 21, 117 },
    { 118, 22, 118 },
    { 119, 23, 119 },
    { 120, 24, 120 },
    { 121, 25, 121 },
    { 122, 26, 122 },
    { 123, 27, 123 },
    { 124, 28, 124 },
    { 125, 29, 125 },
    { 126, 30, 126 },
    { 127, 31, 127 },
    { 128, 32, 128 },
    { 129, 33, 129 },
    { 130, 34, 130 },
    { 131, 35, 131 },
    { 132, 36, 132 },
    { 133, 37, 133 },
    { 134, 38, 134 },
    { 135, 39, 135 },
    { 136, 40, 136 },
    { 137, 41, 137 },
    { 138, 42, 138 },
    { 139, 43, 139 },
    { 140, 44, 140 },
    { 141, 45, 141 },
    { 142, 46, 142 },
    { 143, 47, 143 },
    { 144, 48, 144 },
};

server_config servers[2][1] = {
    {
        { 's', 144, DEFAULT_RDMA_PORT },
    },
    {
        {'m', 48, DEFAULT_RDMA_PORT + 30}
    }
};

client_config clients[6][30] = {
    {
        { client_ids[0], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 0 },
        { client_ids[1], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 1 },
        { client_ids[2], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 2 },
        { client_ids[3], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 3 },
        { client_ids[4], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 4 },
        { client_ids[5], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 5 },
        { client_ids[6], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 6 },
        { client_ids[7], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 7 },
        { client_ids[8], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 8 },
        { client_ids[9], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 9 },
        { client_ids[10], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 10 },
        { client_ids[11], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 11 },
        { client_ids[12], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 12 },
        { client_ids[13], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 13 },
        { client_ids[14], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 14 },
        { client_ids[15], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 15 },
        { client_ids[16], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 16 },
        { client_ids[17], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 17 },
        { client_ids[18], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 18 },
        { client_ids[19], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 19 },
        { client_ids[20], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 20 },
        { client_ids[21], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 21 },
        { client_ids[22], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 22 },
        { client_ids[23], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 23 },
    },
    {
        { client_ids[24], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 24 },
        { client_ids[25], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 25 },
        { client_ids[26], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 26 },
        { client_ids[27], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 27 },
        { client_ids[28], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 28 },
        { client_ids[29], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 29 },
        { client_ids[30], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 30 },
        { client_ids[31], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 31 },
        { client_ids[32], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 32 },
        { client_ids[33], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 33 },
        { client_ids[34], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 34 },
        { client_ids[35], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 35 },
        { client_ids[36], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 36 },
        { client_ids[37], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 37 },
        { client_ids[38], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 38 },
        { client_ids[39], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 39 },
        { client_ids[40], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 40 },
        { client_ids[41], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 41 },
        { client_ids[42], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 42 },
        { client_ids[43], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 43 },
        { client_ids[44], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 44 },
        { client_ids[45], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 45 },
        { client_ids[46], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 46 },
        { client_ids[47], 2, "ms", parent_addresses[0], parent_ports[0], peer_groups[0], 47 },
    },
    {
        { client_ids[48], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 48 },
        { client_ids[49], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 49 },
        { client_ids[50], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 50 },
        { client_ids[51], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 51 },
        { client_ids[52], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 52 },
        { client_ids[53], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 53 },
        { client_ids[54], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 54 },
        { client_ids[55], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 55 },
        { client_ids[56], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 56 },
        { client_ids[57], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 57 },
        { client_ids[58], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 58 },
        { client_ids[59], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 59 },
        { client_ids[60], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 60 },
        { client_ids[61], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 61 },
        { client_ids[62], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 62 },
        { client_ids[63], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 63 },
        { client_ids[64], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 64 },
        { client_ids[65], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 65 },
        { client_ids[66], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 66 },
        { client_ids[67], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 67 },
        { client_ids[68], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 68 },
        { client_ids[69], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 69 },
        { client_ids[70], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 70 },
        { client_ids[71], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 71 },
    },
    {
        { client_ids[72], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 72 },
        { client_ids[73], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 73 },
        { client_ids[74], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 74 },
        { client_ids[75], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 75 },
        { client_ids[76], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 76 },
        { client_ids[77], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 77 },
        { client_ids[78], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 78 },
        { client_ids[79], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 79 },
        { client_ids[80], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 80 },
        { client_ids[81], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 81 },
        { client_ids[82], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 82 },
        { client_ids[83], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 83 },
        { client_ids[84], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 84 },
        { client_ids[85], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 85 },
        { client_ids[86], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 86 },
        { client_ids[87], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 87 },
        { client_ids[88], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 88 },
        { client_ids[89], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 89 },
        { client_ids[90], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 90 },
        { client_ids[91], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 91 },
        { client_ids[92], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 92 },
        { client_ids[93], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 93 },
        { client_ids[94], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 94 },
        { client_ids[95], 2, "ms", parent_addresses[1], parent_ports[0], peer_groups[1], 95 },
    },
    {
        { client_ids[96], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 96 },
        { client_ids[97], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 97 },
        { client_ids[98], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 98 },
        { client_ids[99], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 99 },
        { client_ids[100], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 100 },
        { client_ids[101], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 101 },
        { client_ids[102], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 102 },
        { client_ids[103], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 103 },
        { client_ids[104], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 104 },
        { client_ids[105], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 105 },
        { client_ids[106], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 106 },
        { client_ids[107], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 107 },
        { client_ids[108], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 108 },
        { client_ids[109], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 109 },
        { client_ids[110], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 110 },
        { client_ids[111], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 111 },
        { client_ids[112], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 112 },
        { client_ids[113], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 113 },
        { client_ids[114], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 114 },
        { client_ids[115], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 115 },
        { client_ids[116], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 116 },
        { client_ids[117], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 117 },
        { client_ids[118], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 118 },
        { client_ids[119], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 119 },
    },
    {
        { client_ids[120], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 120 },
        { client_ids[121], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 121 },
        { client_ids[122], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 122 },
        { client_ids[123], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 123 },
        { client_ids[124], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 124 },
        { client_ids[125], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 125 },
        { client_ids[126], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 126 },
        { client_ids[127], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 127 },
        { client_ids[128], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 128 },
        { client_ids[129], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 129 },
        { client_ids[130], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 130 },
        { client_ids[131], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 131 },
        { client_ids[132], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 132 },
        { client_ids[133], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 133 },
        { client_ids[134], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 134 },
        { client_ids[135], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 135 },
        { client_ids[136], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 136 },
        { client_ids[137], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 137 },
        { client_ids[138], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 138 },
        { client_ids[139], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 139 },
        { client_ids[140], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 140 },
        { client_ids[141], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 141 },
        { client_ids[142], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 142 },
        { client_ids[143], 2, "ms", parent_addresses[2], parent_ports[0], peer_groups[2], 143 },
    },
};

node_config configs[7] = {
    {
        1,
        1,
        0,
        144,
        "n",
        servers[0],
        NULL,
    },
    {
        25,
        1,
        24,
        144,
        "b",
        servers[1],
        clients[0],
    },
    {
        24,
        0,
        24,
        144,
        "b",
        NULL,
        clients[1],
    },
    {
        25,
        1,
        24,
        144,
        "b",
        servers[1],
        clients[2],
    },
    {
        24,
        0,
        24,
        144,
        "b",
        NULL,
        clients[3],
    },
    {
        25,
        1,
        24,
        144,
        "b",
        servers[1],
        clients[4],
    },
    {
        24,
        0,
        24,
        144,
        "b",
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
