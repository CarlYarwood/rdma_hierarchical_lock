#include "mcs_client.h"
#include "mcs_server.h"
#include "ticket_client.h"
#include "ticket_server.h"
#include "spin_cluster_client.h"

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 1000;

char * addresses[3][20] = {
    {"10.10.1.1"},
    {"10.10.1.2"},
    {
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
        "10.10.1.3"
    }
};

long ports[3][20] = {
    {DEFAULT_RDMA_PORT},
    {DEFAULT_RDMA_PORT},
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
        DEFAULT_RDMA_PORT + 19
    }
};

int peer_group_sizes[3] = {
    1,
    1,
    20
};

int main(int argc, char ** argv){
    int option = 0;
    int parent_peer_group = -1;
    int parent_id = -1;
    int node_peer_group = -1;
    int node_offset = -1;
    int num_workers = -1;
    int child_peer_group = -1;
    int handover = 0;
    uint64_t node_id_start = 0;
    char * parent_lock_type = "none";
    char * local_lock_type = "none";
    char * machine_lock_type = "n";
    char * gated = "n";
    char * is_cluster = "n";
    pthread_t * workers;

    while ((option = getopt(argc, argv, "p:n:P:N:l:L:w:c:i:m:g:")) != -1) {
		switch (option) {
            case 'p':
                parent_peer_group = atoi(optarg);
                break;
            case 'n':
                node_peer_group = atoi(optarg);
                break;
            case 'P':
                parent_id = atoi(optarg);
                break;
            case 'N':
                node_offset = atoi(optarg);
                break;
            case 'l':
                parent_lock_type = optarg;
                break;
            case 'L':
                local_lock_type = optarg;
                break;
            case 'w':
                num_workers = atoi(optarg);
                break;
            case 'c':
                child_peer_group = atoi(optarg);
                break;
            case 'C':
                is_cluster = optarg;
                break;
            case 'i':
                node_id_start = strtoul(optarg, NULL, 0);
                break;
            case 'm':
                machine_lock_type = optarg;
                break;
            case 'g':
                gated = optarg;
                break;
            case 'h':
                handover = atoi(optarg);
                break;
            default:
                printf("invalid option detected\n");
                return -1;
		}
	}

    if(strcmp(is_cluster, "y") == 0){
        spin_cluster_client_in * client_in = (spin_cluster_client_in *)malloc(sizeof(spin_cluster_client_in));
        spin_server_in * server_in = (spin_server_in *)malloc(sizeof(spin_server_in));

        pthread_t * server = (pthread_t *)malloc(sizeof(pthread_t));
        pthread_t * client = (pthread_t *)malloc(sizeof(pthread_t));

        volatile uint64_t * lock = (volatile uint64_t *)malloc(sizeof(uint64_t));
        *lock = 0;
        volatile uint64_t * buffer = (volatile uint64_t *)malloc(sizeof(uint64_t));
        *buffer = 0;
        volatile int * num_conn = (volatile int *)malloc(sizeof(int));
        *num_conn = 0;
        volatile struct rdma_cm_id ** id_arr = (volatile struct rdma_cm_id **)malloc(sizeof(struct rdma_cm_id *) * peer_group_sizes[child_peer_group]);
        for (int i = 0; i < peer_group_sizes[child_peer_group]; i++) {
            (server_in->id_arr)[i] = NULL;
        }

        volatile int * ready = malloc(sizeof(int));
        *ready = 0;

        server_in->lock = lock;
        server_in->buffer = buffer;
        server_in->num_conn = num_conn;
        server_in->id_arr = id_arr;
        server_in->ready = ready;
        server_in->num_children = peer_group_sizes[child_peer_group];

        client_in->sync = (volatile uint64_t *)malloc(sizeof(uint64_t));
        *(client_in->sync) = 0;
        client_in->node_id = node_id_start;
        client_in->parent_address = addresses[parent_peer_group][parent_id];
        client_in->parent_port = ports[parent_peer_group][parent_id];
        client_in->lock = lock;
        client_in->buffer = buffer;
        client_in->num_conn = num_conn;
        client_in->handover = handover;

        pthread_create(server, NULL, spin_server, (void *)server_in);
        pthread_create(client, NULL, spin_cluster_client, (void *)client_in);
        pthread_join(*server, NULL);
        pthread_join(*client, NULL);

        free((void *)lock);
        free((void *)buffer);
        free((void *)num_conn);
        free((void *)id_arr);
        free((void *)ready);
        free(server_in);
        free(client_in);
        free(server);
        free(client);
        return 0;
    }

    if (strcmp(parent_lock_type, "mcs") == 0){
        mcsLock* mcs = buildMcsLock();
        ticketLock* ticket = buildTicketLock();
        spinLock* spin = buildSpinLock();
        printf("in client mcs\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
        mcs_client_in * in = (mcs_client_in *)malloc(sizeof(mcs_client_in) * num_workers);
        for (int i = 0; i < num_workers; i++) {
            in[i].metadata = (volatile uint64_t *)malloc(sizeof(uint64_t) * 3);
            in[i].metadata[NEXT] = 0;
            in[i].metadata[NOTIFY] = 0;
            in[i].metadata[MCS_SYNC] = 0;
            in[i].node_id = node_id_start + (uint64_t) i;
            in[i].critical_section = critical_section;
            in[i].noncritical_section = noncritical_section;
            in[i].num_aquire = num_aquire;
            in[i].parent_address = addresses[parent_peer_group][parent_id];
            in[i].parent_port = ports[parent_peer_group][parent_id];
            in[i].peer_addresses = addresses[node_peer_group];
            in[i].peer_ports = ports[node_peer_group];
            in[i].num_peers = peer_group_sizes[node_peer_group];
            in[i].machine_lock_type = machine_lock_type;
            in[i].gated = gated;
            switch (*machine_lock_type) {
                case 'm':
                    in[i].machine_lock.mcs = mcs;
                    break;
                case 't':
                    in[i].machine_lock.ticket = ticket;
                    break;
                case 's':
                    in[i].machine_lock.spin = spin;
                    break;
                default:
                    // do nothing
            }
            pthread_create(&workers[i], NULL, mcs_client, (void *)&in[i]);
        }

        for(int i = 0; i < num_workers; i++) {
            pthread_join(workers[i], NULL);
            free((void *)in[i].metadata);
        }
        destroyMcsLock(mcs);
        destroyTicketLock(ticket);
        destroySpinLock(spin);
        free(in);
        free(workers);
        return 0;
    }else if(strcmp(parent_lock_type, "ticket") == 0){
        mcsLock* mcs = buildMcsLock();
        ticketLock* ticket = buildTicketLock();
        spinLock* spin = buildSpinLock();
        printf("in client ticket\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
        ticket_client_in * in = (ticket_client_in *)malloc(sizeof(ticket_client_in) * num_workers);
        for(int i = 0; i < num_workers; i++) {
            in[i].sync = (volatile uint64_t *)malloc(sizeof(uint64_t));
            *in[i].sync = 0;
            in[i].node_id = node_id_start + (uint64_t) i;
            in[i].parent_address = addresses[parent_peer_group][parent_id];
            in[i].parent_port = ports[parent_peer_group][parent_id];
            in[i].critical_section = critical_section;
            in[i].noncritical_section = noncritical_section;
            in[i].num_aquire = num_aquire;
            in[i].machine_lock_type = machine_lock_type;
            in[i].gated = gated;
            switch (*machine_lock_type) {
                case 'm':
                    in[i].machine_lock.mcs = mcs;
                    break;
                case 't':
                    in[i].machine_lock.ticket = ticket;
                    break;
                case 's':
                    in[i].machine_lock.spin = spin;
                    break;
                default:
                    // do nothing
            }
            pthread_create(&workers[i], NULL, ticket_client, (void *)&in[i]);
        }

        for(int i = 0; i < num_workers; i++) {
            pthread_join(workers[i], NULL);
            free((void *)in[i].sync);
        }
        destroyMcsLock(mcs);
        destroyTicketLock(ticket);
        destroySpinLock(spin);
        free(in);
        free(workers);
        return 0;
    }else if(strcmp(parent_lock_type, "spin") == 0){
        mcsLock* mcs = buildMcsLock();
        ticketLock* ticket = buildTicketLock();
        spinLock* spin = buildSpinLock();
        printf("in client spin\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
        spin_client_in * in = (spin_client_in *)malloc(sizeof(spin_client_in) * num_workers);
        for (int i = 0; i < num_workers ; i++) {
            in[i].sync = (volatile uint64_t *)malloc(sizeof(uint64_t));
            *in[i].sync = 0;
            in[i].machine_lock_type = machine_lock_type;
            in[i].parent_address = addresses[parent_peer_group][parent_id];
            in[i].parent_port = ports[parent_peer_group][parent_id];
            in[i].node_id = node_id_start + (uint64_t) i;
            in[i].critical_section = critical_section;
            in[i].noncritical_section = noncritical_section;
            in[i].num_aquire = num_aquire;
            in[i].gated = gated;
            switch (*machine_lock_type) {
                case 'm':
                    in[i].machine_lock.mcs = mcs;
                    break;
                case 't':
                    in[i].machine_lock.ticket = ticket;
                    break;
                case 's':
                    in[i].machine_lock.spin = spin;
                    break;
                default:
                    // do nothing
            }
            pthread_create(&workers[i], NULL, spin_client, (void *)&in[i]);
        }

        for(int i = 0; i < num_workers; i++) {
            pthread_join(workers[i], NULL);
            free((void *)in[i].sync);
        }
        destroyMcsLock(mcs);
        destroyTicketLock(ticket);
        destroySpinLock(spin);
        free(in);
        free(workers);
        return 0;
    }
 
    if (strcmp(local_lock_type, "mcs") == 0){
        printf("in server mcs\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t));
        mcs_server_in * in = (mcs_server_in *)malloc(sizeof(mcs_server_in));
        in->lock = (volatile uint64_t *)malloc(sizeof(uint64_t) * 2);
        (in->lock)[LOCK] = 0;
        (in->lock)[READY] = 0;
        (in->id_arr) = (struct rdma_cm_id **)malloc(sizeof(struct rdma_cm_id *) * peer_group_sizes[child_peer_group]);
        (in->ready) = (volatile int *)malloc(sizeof(int));
        *(in->ready) = 1;
        for (int i = 0; i < peer_group_sizes[child_peer_group]; i++) {
            (in->id_arr)[i] = NULL;
        }
        in->num_children = peer_group_sizes[child_peer_group];
        pthread_create(workers, NULL, mcs_server, (void *)in);
        pthread_join(*workers, NULL);
        free((void *)in->lock);
        free(in->id_arr);
        free((void *)in->ready);
        free(in);
        free(workers);
        return 0;
    }else if(strcmp(local_lock_type, "ticket") == 0){
        printf("in server ticket\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t));
        ticket_server_in * in = (ticket_server_in *)malloc(sizeof(ticket_server_in));
        in->lock = (uint64_t *)malloc(sizeof(uint64_t) * 2);
        (in->lock)[NEXT] = 0;
        (in->lock)[NOW] = 0;
        (in->id_arr) = (struct rdma_cm_id **)malloc(sizeof(struct rdma_cm_id *) * peer_group_sizes[child_peer_group]);
        (in->ready) = (volatile int *)malloc(sizeof(int));
        *(in->ready) = 1;
        for (int i = 0; i < peer_group_sizes[child_peer_group]; i++) {
            (in->id_arr)[i] = NULL;
        }
        in->num_children = peer_group_sizes[child_peer_group];
        pthread_create(workers, NULL, ticket_server, (void *)in);
        pthread_join(*workers, NULL);
        free(in->lock);
        free(in->id_arr);
        free((void *)in->ready);
        free(in);
        free(workers);
        return 0;
    }else if(strcmp(local_lock_type, "spin") == 0){
        printf("in server spin\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t));
        spin_server_in * in = (spin_server_in *)malloc(sizeof(spin_server_in));
        in->lock = (volatile uint64_t *)malloc(sizeof(uint64_t));
        *(in->lock) = 0;
        in->buffer = (volatile uint64_t *)malloc(sizeof(uint64_t));
        *(in->buffer) = 0;
        in->num_conn = (volatile int *)malloc(sizeof(int));
        *(in->num_conn) = 0;
        (in->id_arr) = (volatile struct rdma_cm_id **)malloc(sizeof(struct rdma_cm_id *) * peer_group_sizes[child_peer_group]);
        (in->ready) = (volatile int *)malloc(sizeof(int));
        *(in->ready) = 1;
        for (int i = 0; i < peer_group_sizes[child_peer_group]; i++) {
            (in->id_arr)[i] = NULL;
        }
        in->num_children = peer_group_sizes[child_peer_group];
        pthread_create(workers, NULL, spin_server, (void *)in);
        pthread_join(*workers, NULL);
        free((void *)in->lock);
        free((void *)in->buffer);
        free((void *)in->num_conn);
        free((void *)in->id_arr);
        free((void *)in->ready);
        free(in);
        free(workers);
        return 0;
    }
    return 0;
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
