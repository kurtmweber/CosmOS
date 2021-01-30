# CosmOS coding standards

- Standard based on LLVM and defined in `.clang-format` in project root. Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) as your code formatter, and `make lint` to check for errors.
- CosmOS is spelled <i>CosmOS</i>
- CosmOS uses [Snake Case](https://en.wikipedia.org/wiki/Snake_case) for function names
- Do not include header files from `/kernel/dev/`. Instead, query the DeviceMgr for the appropriate device and use the abstract device API from `/sys/deviceapi/`

# Configure your editor for code formatting

## VSCode

Configure VSCode to use the `cLangFormat` formatter, and `file` for style options, which will refer to `.clang-format`.

In `settings.json`:

```json5
{
  // Coding style. Use "file" to load the style from a .clang-format file in the current or parent directory.
  'C_Cpp.clang_format_style': 'file',

  // Configures the formatting engine for clangFormat.
  'C_Cpp.formatting': 'clangFormat',

  // Format a file on save. Formatter must be available.
  'editor.formatOnSave': true,
}
```
