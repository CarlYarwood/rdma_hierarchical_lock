#include "mcs_client.h"
#include "mcs_server.h"
#include "spin_client.h"
#include "spin_server.h"
#include "ticket_client.h"
#include "ticket_server.h"

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 1000;

char * addresses[2][40] = {
    {"10.10.1.1"},
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

long ports[2][40] = {
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
        DEFAULT_RDMA_PORT + 19,
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

int peer_group_sizes[2] = {
    1,
    40
};

int main(int argc, char ** argv){
    int option = 0;
    int parent_peer_group = -1;
    int parent_id = -1;
    int node_peer_group = -1;
    int node_offset = -1;
    int num_workers = -1;
    int child_peer_group = -1;
    uint64_t node_id_start = 0;
    char * parent_lock_type = "none";
    char * local_lock_type = "none";
    char * machine_lock_type = "none";
    pthread_t * workers;

    while ((option = getopt(argc, argv, "p:n:P:N:l:L:w:c:i:m:")) != -1) {
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
            case 'i':
                node_id_start = strtoul(optarg, NULL, 0);
                break;
            case 'm':
                machine_lock_type = optarg;
                break;
            default:
                printf("invalid option detected\n");
                return -1;
		}
	}

    if (strcmp(parent_lock_type, "mcs") == 0){
        mcsLock* mcs = buildMcsLock();
        ticketLock* ticket = buildTicketLock();
        spinLock* spin = buildSpinLock();
        printf("in client mcs\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
        mcs_client_in * in = (mcs_client_in *)malloc(sizeof(mcs_client_in) * num_workers);
        for (int i = 0; i < num_workers; i++) {
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
            if(strcmp(machine_lock_type, "none") != 0) {
                if(strcmp(machine_lock_type, "mcs") == 0) {
                    in[i].machine_lock.mcs = mcs;
                }else if(strcmp(machine_lock_type, "ticket") == 0) {
                    in[i].machine_lock.ticket = ticket;
                }else if(strcmp(machine_lock_type, "spin") == 0) {
                    in[i].machine_lock.spin = spin;
                }
            }
            pthread_create(&workers[i], NULL, mcs_client, (void *)&in[i]);
        }

        for(int i = 0; i < num_workers; i++) {
            pthread_join(workers[i], NULL);
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
            in[i].node_id = node_id_start + (uint64_t) i;
            in[i].parent_address = addresses[parent_peer_group][parent_id];
            in[i].parent_port = ports[parent_peer_group][parent_id];
            in[i].critical_section = critical_section;
            in[i].noncritical_section = noncritical_section;
            in[i].num_aquire = num_aquire;
            in[i].machine_lock_type = machine_lock_type;
            if(strcmp(machine_lock_type, "none") != 0) {
                if(strcmp(machine_lock_type, "mcs") == 0) {
                    in[i].machine_lock.mcs = mcs;
                }else if(strcmp(machine_lock_type, "ticket") == 0) {
                    in[i].machine_lock.ticket = ticket;
                }else if(strcmp(machine_lock_type, "spin") == 0) {
                    in[i].machine_lock.spin = spin;
                }
            }
            pthread_create(&workers[i], NULL, ticket_client, (void *)&in[i]);
        }

        for(int i = 0; i < num_workers; i++) {
            pthread_join(workers[i], NULL);
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
            in[i].machine_lock_type = machine_lock_type;
            in[i].parent_address = addresses[parent_peer_group][parent_id];
            in[i].parent_port = ports[parent_peer_group][parent_id];
            in[i].node_id = node_id_start + (uint64_t) i;
            in[i].critical_section = critical_section;
            in[i].noncritical_section = noncritical_section;
            in[i].num_aquire = num_aquire;
            if(strcmp(machine_lock_type, "none") != 0) {
                if(strcmp(machine_lock_type, "mcs") == 0) {
                    in[i].machine_lock.mcs = mcs;
                }else if(strcmp(machine_lock_type, "ticket") == 0) {
                    in[i].machine_lock.ticket = ticket;
                }else if(strcmp(machine_lock_type, "spin") == 0) {
                    in[i].machine_lock.spin = spin;
                }
            }
            pthread_create(&workers[i], NULL, spin_client, (void *)&in[i]);
        }

        for(int i = 0; i < num_workers; i++) {
            pthread_join(workers[i], NULL);
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
        in->num_children = peer_group_sizes[child_peer_group];
        pthread_create(workers, NULL, mcs_server, (void *)in);
        pthread_join(*workers, NULL);
        free(in);
        free(workers);
        return 0;
    }else if(strcmp(local_lock_type, "ticket") == 0){
        printf("in server ticket\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t));
        ticket_server_in * in = (ticket_server_in *)malloc(sizeof(ticket_server_in));
        in->num_children = peer_group_sizes[child_peer_group];
        pthread_create(workers, NULL, ticket_server, (void *)in);
        pthread_join(*workers, NULL);
        free(in);
        free(workers);
        return 0;
    }else if(strcmp(local_lock_type, "spin") == 0){
        printf("in server spin\n");
        workers = (pthread_t *) malloc(sizeof(pthread_t));
        spin_server_in * in = (spin_server_in *)malloc(sizeof(spin_server_in));
        in->num_children = peer_group_sizes[child_peer_group];
        pthread_create(workers, NULL, spin_server, (void *)in);
        pthread_join(*workers, NULL);
        free(in);
        free(workers);
        return 0;
    }
    return 0;
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
