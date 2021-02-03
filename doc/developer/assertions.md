
# CosmOS assertions

CosmOS provides the macros `ASSERT(condition, message)` and `ASSERT_NOT_NULL(term, message)` in the header file `/kernel/panic/panic.h` which can be used to define runtime assertions.  In release builds these macros will expand to nothing in order to reduce performance impact.