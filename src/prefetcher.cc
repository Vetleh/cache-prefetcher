#include "interface.hh"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define GHB_SIZE 256
#define IT_SIZE 256

#define WIDTH 4
#define DEPTH 4
#define DEGREE 4

#define STRIDED true

// Delta
struct delta {
    uint64_t value;
    bool     sign;
};

// GHB Entry
struct ghb_entry {
    Addr adr;
    int  prev;
    int  next;
    bool valid;
};

// IT Entry
struct it_entry {
    int ghb_index;
};

// GHB and IT
ghb_entry ghb[GHB_SIZE];
it_entry  it[IT_SIZE];

// GHB head pointer
int head_index = -1;

// Initializes datastructures used by the prefetcher
void prefetch_init(void) {

    // Initializes the GHB
    for (int i = 0; i < GHB_SIZE; ++i) {
        ghb[i].valid = false;
        ghb[i].prev  = -1;
        ghb[i].next  = -1;
    }

    // Initializes the IT
    for (int i = 0; i < IT_SIZE; ++i)
        it[i].ghb_index = -1;
}

// Accesses the IT
it_entry access_it(delta delta) { return it[(delta.value / 64) % IT_SIZE]; }

// Calculates a delta value from a GHB index
delta calculate_delta(int index) {
    delta delta;

    // Stops if one of the entries is the head
    if (index == head_index || index + 1 == head_index) {
        delta.value = 1;
        return delta;
    }

    // Gets the GHB entries
    ghb_entry a = ghb[(index) % GHB_SIZE];
    ghb_entry b = ghb[(index + 1) % GHB_SIZE];

    // Returns -1 if not valid entries
    if (!a.valid || !b.valid) {
        delta.value = 1;
        return delta;
    }

    // Return the delta value
    delta.value = abs(b.adr - a.adr);
    delta.sign  = b.adr > a.adr;
    return delta;
}

// Gets the previous GHB entry that has the same delta value
int get_prev_ghb_entry_with_same_delta(int index, delta d) {

    // Checks the index table
    int ghb_index = access_it(d).ghb_index;

    if (ghb_index > 0) {
        delta delta_candidate;
        delta_candidate.value = abs(ghb[ghb_index].adr - ghb[(ghb_index - 1) % GHB_SIZE].adr);
        delta_candidate.sign  = ghb[ghb_index].adr > ghb[(ghb_index - 1) % GHB_SIZE].adr;
        if (d.value == delta_candidate.value && d.sign == delta_candidate.sign) {
            return ghb_index;
        }
    }

    return -1;
}

// Adds an entry into the GHB
ghb_entry add_ghb_entry(Addr adr, delta d) {

    // Increments the head index
    head_index = (head_index + 1) % GHB_SIZE;

    // Removes the previous link
    int next       = ghb[head_index].next;
    ghb[next].prev = -1;

    // Gets the previous ghb entry with same delta
    int prev = get_prev_ghb_entry_with_same_delta(head_index, d);

    // Updates the head ghb entry
    ghb[head_index].adr   = adr;
    ghb[head_index].prev  = prev;
    ghb[head_index].next  = -1;
    ghb[head_index].valid = true;

    // Updates the previous ghb entry
    ghb[prev].next = head_index;

    // Updates the index table
    it[(d.value / 64) % IT_SIZE].ghb_index = head_index;

    // Returns the added GHB entry
    return ghb[head_index];
}

// Called when an address is accessed
void prefetch_access(AccessStat stat) {
    if (!stat.miss)
        return;

    // Calculates the delta value
    delta d;
    d.value = abs(stat.mem_addr - ghb[head_index].adr);
    d.sign  = stat.mem_addr > ghb[head_index].adr;

    // Adds the GHB entry (i.e. updates the GHB and IT)
    ghb_entry node = add_ghb_entry(stat.mem_addr, d);

    // Amount of prefetched addresses
    int prefetched_addresses = 0;

    for (int i = 0; i < WIDTH; ++i) {
        if (node.prev < 0)
            break;

        // Gets the next node
        int current_index = node.prev;
        node              = ghb[current_index];

        Addr pf_addr = stat.mem_addr;

        for (int j = 0; j < DEPTH; ++j) {

            // Calculates the delta value
            d = calculate_delta(current_index + j);

            // Stops if no more nodes (i.e. no delta)
            if (d.value == 1)
                break;

            // Positive delta
            if (d.sign && pf_addr + d.value < MAX_PHYS_MEM_ADDR)
                pf_addr += d.value;
            else if (!d.sign && pf_addr > d.value)
                pf_addr -= d.value;
            else
                continue;

            // Issues a prefetch request if the address is not already in cache
            if (!in_cache(pf_addr) && !in_mshr_queue(pf_addr)) {
                // printf("[x]");
                ++prefetched_addresses;
                issue_prefetch(pf_addr);
            }

            // Stops when prefetcher degree limit is met
            if (prefetched_addresses >= DEGREE)
                return;
        }
    }

    // Performs strided prefetching
    if (STRIDED) {
        for (int i = 1; i <= DEGREE - prefetched_addresses; ++i) {

            // Calculates the prefetch address
            Addr pf_addr = stat.mem_addr + BLOCK_SIZE * i;

            // Issues a prefetch request if the address is not already in cache
            if (!in_cache(pf_addr) && !in_mshr_queue(pf_addr))
                issue_prefetch(pf_addr);
        }
    }
}

// Called when a prefetch request completes
void prefetch_complete(Addr addr) {}
