# CosmOS coding standards

- Standard based on LLVM and defined in `.clang-format` in project root. Use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) as your code formatter, and `make lint` to check for errors.
- CosmOS is spelled <i>CosmOS</i>
- CosmOS uses [Snake Case](https://en.wikipedia.org/wiki/Snake_case) for function names
- Do not include header files from `/kernel/dev/`. Instead, query the DeviceMgr for the appropriate device and use the abstract device API from `/sys/deviceapi/`

# Configure your editor for code formatting

## VSCode

Configure VSCode to use the `cLangFormat` formatter, and `file` for style options, which will refer to `.clang-format`.

In `settings.json`:

```json
{
  // Coding style, currently supports: Visual Studio, LLVM, Google, Chromium, Mozilla, WebKit. Use "file" to load the style from a .clang-format file in the current or parent directory. Use {key: value, ...} to set specific parameters. For example, the "Visual Studio" style is similar to: { BasedOnStyle: LLVM, UseTab: Never, IndentWidth: 4, TabWidth: 4, BreakBeforeBraces: Allman, AllowShortIfStatementsOnASingleLine: false, IndentCaseLabels: false, ColumnLimit: 0, AccessModifierOffset: -4, NamespaceIndentation: All }
  "C_Cpp.clang_format_style": "file",

  // Configures the formatting engine
  //  - clangFormat: clang-format will be used to format code.
  "C_Cpp.formatting": "clangFormat",

  // Format a file on save. A formatter must be available, the file must not be saved after delay, and the editor must not be shutting down.
  "editor.formatOnSave": true
}
```
