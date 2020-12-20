
CosmOS routes all interrupts via `/interrupts/interrupt_router.h`.

The ISR's in `interrupts/irq.c` all route to `routeInterrupt` which is defined in `/interrupts/interrupt_router.h`

`/interrupts/interrupt_router.h` provides the function `void register_interrupt_handler(int interruptNumber, interruptHandler func);
` which devices can use to establish a subscription to an interrupt.

The function prototype `typedef void (*interruptHandler)(stackFrame *frame);` must be implemented by devices wishing to subscribe to an interrupt.

 