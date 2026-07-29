#include "rdma_server.h"
#include "local_lock.h"

int main(int argc, char ** argv){
    int option = 0;

    while ((option = getopt(argc, argv, "G:g:C:c:N:n:")) != -1) {
		switch (option) {
            case 'G':
                break;
            case 'g':
                break;
            case 'C':
                break;
            case 'c':
                break;
            case 'N':
                break;
            case 'n':
                break;
            default:
                printf("invalid option detected\n");
                return -1;
		}
	}
}

// node -> node Lock -> clusterLock -> Global Lock
// node -> node Lock -> Global Lock
// node -> clusterLock -> Global Lock
