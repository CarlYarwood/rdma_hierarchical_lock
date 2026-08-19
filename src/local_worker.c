#include "local_worker.h"

void* local_worker(void * in) {
    uint64_t * worker_id = (uint64_t *)malloc(sizeof(uint64_t));
    *worker_id = ((local_worker_in *)in)->worker_id;
    volatile uint64_t * sync = ((local_worker_in *)in)->sync;
    char * machine_lock_type = ((local_worker_in *)in)->machine_lock_type;
    int critical_section = ((spin_client_in *) in)->critical_section;
	int noncritical_section = ((spin_client_in *) in)->noncritical_section;
	int num_aquire = ((spin_client_in *) in)->num_aquire;
    clock_t start, end;

    switch(*machine_lock_type) {
        case 'm':
            mcs = ((mcs_client_in *) in)->machine_lock.mcs;
            break;
        case 't':
            ticket = ((mcs_cleint_in *)in)->machine_lock.ticket;
            break;
        case 's':
            spin = ((mcs_client_in *)in)->machine_lock.spin;
            break;
        default:
            //Nothing
    }

    wait_on_sync(sync);
    *sync = 0;

    start = clock();
	for (int i = 0; i < num_aquire; i++) {
		for (int n = 0; n < noncritical_section; n++) {
			noop(&n);
		}
		//lock
        switch(*machine_lock_type) {
            case 'm':
                lockMcs(mcs, *worker_id);
                break;
            case 't':
                lockTicket(ticket , *worker_id);
                break;
            case 's':
                lockSpin(spin, *worker_id);
                break;
            default:
                //Nothing
        }

		wait_on_sync(sync);
		*sync = 0;

		//work
		for (int c = 0; c < critical_section; c++) {
			noop(&c);
		}
		//unlock
		release_spin_lock(ctx, node_id, response);
		switch(*machine_lock_type) {
            case 'm':
                unlockMcs(mcs);
                break;
            case 't':
                unlockTicket(ticket);
                break;
            case 's':
                unlockSpin(spin);
                break;
            default:
                //Nothing
        }
	}
	end = clock();

    printf("%f\n",((double)(num_aquire * critical_section))/((double)(end-start)/CLOCKS_PER_SEC));
    return NULL;
}
