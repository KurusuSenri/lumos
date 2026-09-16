#include "../tests/tests.h"
#include "../include/serial.h"
#include "../include/string.h"
#include "../include/memory.h"

static void test_page_allocator(void)
{
    /*
     * try to allocate a page
     */
    uint64_t a, b;

    if (alloc_page(&a) && alloc_page(&b)) {
        volatile uint64_t *pa = (volatile uint64_t *)(uintptr_t)a;
        volatile uint64_t *pb = (volatile uint64_t *)(uintptr_t)b;

        *pa = 123;
        *pb = 456;

        serial_puts("Page A: ");
        serial_put_hex64(a);
        serial_puts(" value: ");
        serial_put_uint64(*pa);

        serial_puts("\r\nPage B: ");
        serial_put_hex64(b);
        serial_puts(" value: ");
        serial_put_uint64(*pb);
        serial_puts("\r\n");

        memcpy((void *)(uintptr_t)b, (const void *)(uintptr_t)a, sizeof(uint64_t));
        serial_puts("After memcpy, Page B value: ");
        serial_put_uint64(*pb);
        serial_puts("\r\n");
    }
}

static void test_brk_exception(void)
{
    /* Trigger a synchronous exception. */
    __asm__ volatile("brk #0");

    serial_puts("This should not be printed.\r\n");
}

void test_all(){
    serial_puts("\r\nRunning tests...\r\n");
    test_page_allocator();
    test_brk_exception();
}