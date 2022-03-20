/*
 * A sample prefetcher which does sequential one-block lookahead.
 * This means that the prefetcher fetches the next block _after_ the one that
 * was just accessed. It also ignores requests to blocks already in the cache.
 */

// Debug command (TODO REMOVE)
// make && clear && ./build/ALPHA_SE/m5.opt /opt/prefetcher/m5/configs/example/se.py --detailed --caches --l2cache --l2size=1MB
// --prefetcher=policy=proxy --prefetcher=on_access=True

#include "interface.hh"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GHB_SIZE 512
#define IT_SIZE 512

#define WIDTH 16
#define DEPTH 16
#define DEGREE 32

// Global History Buffer (GHB) Entry
struct ghb_entry {
    Addr adr;
    int  prev;
    int  next;
    bool valid;
};

// Index Table (IT) Entry
struct it_entry {
    int ghb_index;
};

ghb_entry ghb[GHB_SIZE];
it_entry  it[IT_SIZE];

// Points to head of GHB
int head_index = -1;

/* Called before any calls to prefetch_access. */
/* This is the place to initialize data structures. */
void prefetch_init(void) {

    // Initializes the GHB
    for (int i = 0; i < GHB_SIZE; ++i) {
        ghb[i].valid = false;
        ghb[i].prev  = -1;
        ghb[i].next  = -1;
    }

    // Initializes the IT
    for (int i = 0; i < IT_SIZE; ++i) {
        it[i].ghb_index = -1;
    }
}

// Calculates a delta value
int calculate_delta(int index) {

    // Stops if one of the entries is the head
    if (index == head_index || index + 1 == head_index)
        return -1;

    // Gets the GHB entries
    ghb_entry a = ghb[(index) % GHB_SIZE];
    ghb_entry b = ghb[(index + 1) % GHB_SIZE];

    // Returns -1 if not valid entries
    if (!a.valid || !b.valid)
        return -1;

    // Return the delta value
    return b.adr - a.adr;
}

it_entry access_it(int delta) { return it[abs((delta / 64) % IT_SIZE)]; }

int get_prev_ghb_entry_with_same_delta(int index, int delta) {

    // Checks the index table
    int ghb_index = access_it(delta).ghb_index;
    if (ghb_index > 0) {
        int delta_candidate = ghb[ghb_index].adr - ghb[(ghb_index - 1) % GHB_SIZE].adr;
        if (delta == delta_candidate)
            return ghb_index;
    }

    // TODO: Add for loop

    return -1;
}

ghb_entry add_ghb_entry(Addr adr, int delta) {
    // Increments the head index
    head_index = (head_index + 1) % GHB_SIZE;

    // Removes the previous link
    int next       = ghb[head_index].next;
    ghb[next].prev = -1;

    // Gets the previous ghb entry with same delta
    int prev = get_prev_ghb_entry_with_same_delta(head_index, delta);

    // Updates the head ghb entry
    ghb[head_index].adr   = adr;
    ghb[head_index].prev  = prev;
    ghb[head_index].next  = -1;
    ghb[head_index].valid = true;

    // Updates the previous ghb entry
    ghb[prev].next = head_index;

    // Updates the index table
    it[abs((delta / 64) % IT_SIZE)].ghb_index = head_index;

    return ghb[head_index];
}

void prefetch_access(AccessStat stat) {
    // printf("Hello, I am done");
    if (!stat.miss)
        return;

    // Calculates the delta value
    int delta = ghb[head_index].adr - stat.mem_addr;

    // Adds the ghb entry (updates the GHB and IT)
    ghb_entry node = add_ghb_entry(stat.mem_addr, delta);

    int prefetched_addresses = 0;

    for (int i = 0; i < WIDTH; ++i) {
        if (node.prev < 0)
            continue;

        // Gets the next node
        int current_index = node.prev;
        node              = ghb[current_index];

        Addr pf_addr = stat.mem_addr;

        for (int j = 0; j < DEPTH; ++j) {

            // Calculates the delta value
            delta = calculate_delta(current_index + j);

            // Stops if no more nodes
            if (delta == -1)
                break;

            // Continues if address is too large
            if (pf_addr + delta > MAX_PHYS_MEM_ADDR || pf_addr + delta < 0)
                continue;

            // Adds the delta to the miss address
            pf_addr += delta;

            // printf("%d\n", current_index + j);

            // Issue a prefetch request if the address is not already in cache
            if (!in_cache(pf_addr)) {
                ++prefetched_addresses;
                issue_prefetch(pf_addr);
            }

            // Gets the next node
            node = ghb[(current_index + j) % GHB_SIZE];
        }
        // printf("\n");
    }

    for (int i = 1; i <= DEGREE - prefetched_addresses; ++i) {

        Addr pf_addr = stat.mem_addr + BLOCK_SIZE * i;

        if (!in_cache(pf_addr))
            issue_prefetch(pf_addr);
    }
}

void prefetch_complete(Addr addr) {
    /*
     * Called when a block requested by the prefetcher has been loaded.
     */
}
