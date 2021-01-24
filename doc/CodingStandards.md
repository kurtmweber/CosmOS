
# CosmOS coding standards

* Standard based on LLVM and defined in `.clang-format` in project root. Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) as your code formatter, and `make lint` to check for errors.
* CosmOS is spelled <i>CosmOS</i>
* CosmOS uses Camel Case for function names
* Do not include header files from /kernel/dev/. Instead, query the DeviceMgr for the appropriate device and use the abstract device API
