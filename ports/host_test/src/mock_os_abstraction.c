#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "os_abstraction.h"
#include "state_manager.h"
#include "display_manager.h"

// Simple ring buffer to simulate RTOS queues on the host
typedef struct {
    uint8_t buffer[256]; // Generously sized for testing small payloads
    uint32_t item_size;
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    uint32_t max_items;
} MockQueue_t;

// Static allocation for our mock queues
static MockQueue_t mock_state_q;
static MockQueue_t mock_display_q;

// Map them to the global handles expected by the application
RtosQueueHandle_t state_message_queue = &mock_state_q;
RtosQueueHandle_t display_message_queue = &mock_display_q;

static uint32_t mock_time_ms = 0;

// Helper to set up the mock ring buffers
static void init_mock_queue(MockQueue_t* q, uint32_t item_size, uint32_t max_items) {
    memset(q, 0, sizeof(MockQueue_t));
    q->item_size = item_size;
    q->max_items = max_items;
}

/* ========================================================================= *
 * Mock OS API Implementations
 * ========================================================================= */

void rtos_system_init(void) {
    // Initialize our fake queues
    init_mock_queue(&mock_state_q, sizeof(StateMsg_t), 10);
    init_mock_queue(&mock_display_q, sizeof(DisplayMsg_t), 10);
    mock_time_ms = 0;
}

bool rtos_queue_send(RtosQueueHandle_t queue, void* data) {
    MockQueue_t* q = (MockQueue_t*)queue;
    
    // Hard assertion: If the test overflows the queue, fail the test immediately
    assert(q->count < q->max_items); 

    uint32_t offset = q->tail * q->item_size;
    memcpy(&q->buffer[offset], data, q->item_size);
    
    q->tail = (q->tail + 1) % q->max_items;
    q->count++;
    return true;
}

bool rtos_queue_receive(RtosQueueHandle_t queue, void* buffer, uint32_t timeout_ms) {
    MockQueue_t* q = (MockQueue_t*)queue;
    
    if (q->count == 0) return false; // Queue is empty

    uint32_t offset = q->head * q->item_size;
    memcpy(buffer, &q->buffer[offset], q->item_size);
    
    q->head = (q->head + 1) % q->max_items;
    q->count--;
    return true;
}

// Mock time functions (Tests can manually advance mock_time_ms if needed)
uint32_t rtos_get_time_ms(void) { return mock_time_ms; }
void rtos_delay_ms(uint32_t ms) { mock_time_ms += ms; }

// NOP for pure logic tests
void rtos_semaphore_wait(RtosSemaphoreHandle_t semaphore, uint32_t timeout_ms) {}
