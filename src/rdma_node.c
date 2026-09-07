#include "mcs_client.h"
#include "mcs_server.h"
#include "ticket_client.h"
#include "ticket_server.h"
#include "spin_client.h"
#include "spin_server.h"

int critical_section = 1;
int noncritical_section = 1;
int num_aquire = 1000;

char * addresses[2][20] = {
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
        "10.10.1.2"
    }
};

long ports[2][20] = {
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

int peer_group_sizes[2] = {
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
    uint64_t node_id_start = 0;
    char * parent_lock_type = "n";
    char * local_lock_type = "n";
    char * machine_lock_type = "n";
    pthread_t * workers;

    while ((option = getopt(argc, argv, "p:n:P:N:l:L:w:c:i:m:")) != -1) {
		switch (option) {
            case 'p':
                parent_peer_group = atoi(optarg);
                break;
            case 'P':
                parent_id = atoi(optarg);
                break;
            case 'n':
                node_peer_group = atoi(optarg);
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

    mcsLock* mcs = buildMcsLock();
    ticketLock* ticket = buildTicketLock();
    spinLock* spin = buildSpinLock();

    switch(*parent_lock_type) {
        case 'm':
            printf("in client mcs\n");
            workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
            mcs_client_in * mcs_in = (mcs_client_in *)malloc(sizeof(mcs_client_in) * num_workers);
            for (int i = 0; i < num_workers; i++) {
                mcs_in[i].node_id = node_id_start + (uint64_t) i;
                mcs_in[i].critical_section = critical_section;
                mcs_in[i].noncritical_section = noncritical_section;
                mcs_in[i].num_aquire = num_aquire;
                mcs_in[i].parent_address = addresses[parent_peer_group][parent_id];
                mcs_in[i].parent_port = ports[parent_peer_group][parent_id];
                mcs_in[i].peer_addresses = addresses[node_peer_group];
                mcs_in[i].peer_ports = ports[node_peer_group];
                mcs_in[i].num_peers = peer_group_sizes[node_peer_group];
                mcs_in[i].machine_lock_type = machine_lock_type;
                switch (*machine_lock_type) {
                    case 'm':
                        mcs_in[i].machine_lock.mcs = mcs;
                        break;
                    case 't':
                        mcs_in[i].machine_lock.ticket = ticket;
                        break;
                    case 's':
                        mcs_in[i].machine_lock.spin = spin;
                        break;
                    default:
                        // do nothing
                }
                pthread_create(&workers[i], NULL, mcs_client, (void *)&mcs_in[i]);
            }

            for(int i = 0; i < num_workers; i++) {
                pthread_join(workers[i], NULL);
            }
            destroyMcsLock(mcs);
            destroyTicketLock(ticket);
            destroySpinLock(spin);
            free(mcs_in);
            free(workers);
            return 0;
        case 't':
            printf("in client ticket\n");
            workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
            ticket_client_in * ticket_in = (ticket_client_in *)malloc(sizeof(ticket_client_in) * num_workers);
            for(int i = 0; i < num_workers; i++) {
                ticket_in[i].node_id = node_id_start + (uint64_t) i;
                ticket_in[i].parent_address = addresses[parent_peer_group][parent_id];
                ticket_in[i].parent_port = ports[parent_peer_group][parent_id];
                ticket_in[i].critical_section = critical_section;
                ticket_in[i].noncritical_section = noncritical_section;
                ticket_in[i].num_aquire = num_aquire;
                ticket_in[i].machine_lock_type = machine_lock_type;
                switch (*machine_lock_type) {
                    case 'm':
                        ticket_in[i].machine_lock.mcs = mcs;
                        break;
                    case 't':
                        ticket_in[i].machine_lock.ticket = ticket;
                        break;
                    case 's':
                        ticket_in[i].machine_lock.spin = spin;
                        break;
                    default:
                        // do nothing
                }
                pthread_create(&workers[i], NULL, ticket_client, (void *)&ticket_in[i]);
            }

            for(int i = 0; i < num_workers; i++) {
                pthread_join(workers[i], NULL);
            }
            destroyMcsLock(mcs);
            destroyTicketLock(ticket);
            destroySpinLock(spin);
            free(ticket_in);
            free(workers);
            return 0;
        case 's':
            printf("in client spin\n");
            workers = (pthread_t *) malloc(sizeof(pthread_t) * num_workers);
            spin_client_in * spin_in = (spin_client_in *)malloc(sizeof(spin_client_in) * num_workers);
            for (int i = 0; i < num_workers ; i++) {
                spin_in[i].machine_lock_type = machine_lock_type;
                spin_in[i].parent_address = addresses[parent_peer_group][parent_id];
                spin_in[i].parent_port = ports[parent_peer_group][parent_id];
                spin_in[i].node_id = node_id_start + (uint64_t) i;
                spin_in[i].critical_section = critical_section;
                spin_in[i].noncritical_section = noncritical_section;
                spin_in[i].num_aquire = num_aquire;
                switch (*machine_lock_type) {
                    case 'm':
                        spin_in[i].machine_lock.mcs = mcs;
                        break;
                    case 't':
                        spin_in[i].machine_lock.ticket = ticket;
                        break;
                    case 's':
                        spin_in[i].machine_lock.spin = spin;
                        break;
                    default:
                        // do nothing
                }
                pthread_create(&workers[i], NULL, spin_client, (void *)&spin_in[i]);
            }

            for(int i = 0; i < num_workers; i++) {
                pthread_join(workers[i], NULL);
            }
            destroyMcsLock(mcs);
            destroyTicketLock(ticket);
            destroySpinLock(spin);
            free(spin_in);
            free(workers);
            return 0;
        default:
            destroyMcsLock(mcs);
            destroyTicketLock(ticket);
            destroySpinLock(spin);
    }

    server_in * in = (server_in *)malloc(sizeof(server_in));
    switch(*local_lock_type) {
        case 'm':
            printf("in server mcs\n");
            workers = (pthread_t *) malloc(sizeof(pthread_t));
            in->num_children = peer_group_sizes[child_peer_group];
            pthread_create(workers, NULL, mcs_server, (void *)in);
            pthread_join(*workers, NULL);
            free(in);
            free(workers);
            return 0;
        case 't':
            printf("in server ticket\n");
            workers = (pthread_t *) malloc(sizeof(pthread_t));
            in->num_children = peer_group_sizes[child_peer_group];
            pthread_create(workers, NULL, ticket_server, (void *)in);
            pthread_join(*workers, NULL);
            free(in);
            free(workers);
            return 0;
        case 's':
            printf("in server spin\n");
            workers = (pthread_t *) malloc(sizeof(pthread_t));
            in->num_children = peer_group_sizes[child_peer_group];
            pthread_create(workers, NULL, spin_server, (void *)in);
            pthread_join(*workers, NULL);
            free(in);
            free(workers);
            return 0;
        default:
            free(in);
    }
    return 0;
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
