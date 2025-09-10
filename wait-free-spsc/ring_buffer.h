#ifndef RING_BUFFER_LIB_HH
#define RING_BUFFER_LIB_HH

#include <stddef.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct {
    size_t cap;
    atomic_size_t deq_ptr;
    atomic_size_t enq_ptr;
    void **buffer;
} ring_buffer_t;


typedef enum {
    E_OK,
    E_ERR,
    E_FULL,
    E_EMPTY,
    E_NOMEM,
} err_ring_buffer_t;


err_ring_buffer_t init_rb(ring_buffer_t** rb, size_t cap);
err_ring_buffer_t enqueue_rb(void*, ring_buffer_t*);
err_ring_buffer_t dequeue_rb(void**, ring_buffer_t*);
void free_rb(ring_buffer_t*);
 
#endif
