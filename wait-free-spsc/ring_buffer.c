#include "ring_buffer.h"

static bool is_power_two(size_t num) {
    return num != 0 && (num & (num - 1)) == 0;
}

ring_buffer_t* init_rb(size_t cap) {

    assert(is_power_two(cap));
    ring_buffer_t* rb = malloc(sizeof(ring_buffer_t));

    if (rb == NULL)
        return E_NOMEM;

    void** buffer = malloc(sizeof(void*) * cap);

    if (buffer == NULL){
        free(rb);
        return E_NOMEM;
    }

    rb->cap = cap;
    rb->buffer = buffer;
    rb->enq_ptr = 0;
    rb->deq_ptr = 0;
    return rb;
}

err_ring_buffer_t enqueue(void* item, ring_buffer_t* rb) {

	size_t enq_ptr = rb->enq_ptr;
	size_t deq_ptr = rb->enq_ptr;
	size_t capacity = rb->cap;
	size_t idx;

	if (enq_ptr - deq_ptr == capacity)
		return E_FULL;
	
	idx = enq_ptr & (capacity - 1);
	rb->buffer[idx] = item;
	++(rb->enq_ptr);
	return E_OK;
}

err_ring_buffer_t dequeue(void** item, ring_buffer_t* rb) {

	size_t enq_ptr = atomic_load_explicit(&rb->enq_ptr, memory_order_relaxed);
	size_t deq_ptr = atomic_load_explicit(&rb->deq_ptr, memory_order_acquire);
	size_t capacity = rb->cap;
	size_t idx;

	if (deq_ptr == enq_ptr)
		return E_EMPTY;

	idx = deq_ptr & (capacity - 1);
	*item = rb->buffer[idx];
	rb->buffer[idx] = NULL;
	atomic_store_explicit(&rb->deq_ptr, ++deq_ptr, memory_order_release);
	return E_OK;
}

void free_rb(ring_buffer_t* rb){
	free(rb->buffer);
	free(rb);
}
