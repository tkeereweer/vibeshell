# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 42 Norm Compliance (Version 4.1)

This repository follows the 42 School Norm (Version 4.1). ALL code written must strictly adhere to these rules. The norminette tool validates most rules automatically, but rules marked with (*) require manual verification during peer evaluation.

### Naming Conventions

- **Structures**: Must start with `s_` (e.g., `s_stack`, `s_node`)
- **Typedefs**: Must start with `t_` (e.g., `t_stack`, `t_node`)
- **Unions**: Must start with `u_` (e.g., `u_data`)
- **Enums**: Must start with `e_` (e.g., `e_state`)
- **Globals**: Must start with `g_` (e.g., `g_variable`)
- **All identifiers** (functions, variables, types): snake_case only (lowercase, digits, underscores)
- **Files and directories**: snake_case only
- **(*) Explicit names**: All identifiers must be explicit, readable in English, or mnemonic
- **Global variables**: Only allowed if marked `const` or `static` (unless project explicitly permits)
- **ASCII only**: Non-ASCII characters forbidden except in literal strings/chars

### Formatting Rules

- **Function length**: Maximum 25 lines (excluding function braces)
- **Line width**: Maximum 80 columns (tabs count as their space representation)
- **Indentation**: 4-character TABS (ASCII char 9, NOT 4 spaces)
- **Function separation**: Functions must be separated by an empty line
- **Braces**: Alone on their own line, except in struct/enum/union declarations
- **Empty lines**: Must be truly empty (no spaces/tabs)
- **No consecutive empty lines or spaces**
- **Variable declarations**: Must be at the beginning of function, before any code
- **Variable alignment**: All variable names in same scope indented on same column
- **Pointer asterisks**: Stuck to variable names (e.g., `char *str`, not `char* str`)
- **One variable per line**
- **Declaration and initialization**: Cannot be on same line (except globals, statics, constants)
- **Empty line**: Required between variable declarations and function body
- **One instruction per line**: No multiple assignments or operations on same line
- **Line splitting**: When needed, indent continuation lines, operators at start of new line
- **Spaces**: After commas and semicolons (unless end of line)
- **Operator spacing**: One space around all operators and operands
- **Keyword spacing**: Space after C keywords (except type keywords like int, char, float, sizeof)
- **Control structures**: Must use braces unless single instruction on single line
- **Return values**: Must be in parentheses (e.g., `return (value);`) unless void

### Function Rules

- **Parameter limit**: Maximum 4 named parameters
- **No-argument functions**: Must explicitly use `void` (e.g., `int func(void)`)
- **Prototype naming**: Parameters must be named in prototypes
- **Variable limit**: Maximum 5 variables per function
- **Return format**: `return (value);` for non-void, `return ;` for void
- **Function signature**: Single tab between return type and function name

### Typedef, Struct, Enum, Union

- **Space after keyword**: `struct s_name`, `enum e_name`, `union u_name`
- **Variable declarations**: Apply usual indentation rules
- **Internal formatting**: Regular indentation inside braces
- **Structure names**: Indented on same column within their scope
- **No structures in .c files**: Structures must be declared in header files only

### Headers (.h files)

- **Allowed content** (*): Includes, declarations, defines, prototypes, macros only
- **Include order**: All includes at beginning of file
- **No .c includes**: Cannot include .c files
- **Header guards**: Required format: file `ft_foo.h` → guard `FT_FOO_H`
- **(*) No unused includes**: Every include must be necessary
- **Include justification**: Can be documented with comments

Example header structure:
```c
#ifndef FT_HEADER_H
# define FT_HEADER_H

# include <stdlib.h>
# include <stdio.h>

# define FOO "bar"

int		g_variable;
struct	s_struct;

#endif
```

### 42 Header Requirement

- **(*) Every .c and .h file** must start with the standard 42 header
- **(*) Header content**: Creator login, student email (@student.campus), creation date, last update login/date
- **Auto-update**: Header should update automatically on save
- Configure your editor: `C-c C-h` (emacs), `:Stdheader` or `F1` (vim)

### Macros and Preprocessor

- **(*) #define usage**: Only for literal and constant values
- **(*) No norm bypass**: No #defines to obfuscate code or bypass norm
- **(*) Standard library macros**: Only if allowed by project scope
- **No multiline macros**
- **Uppercase names**: All macro names must be uppercase
- **Indent preprocessor**: Indent directives inside #if/#ifdef/#ifndef blocks
- **Global scope only**: Preprocessor instructions forbidden inside functions

### Forbidden Constructs

- **for** loops (use while instead)
- **do...while** loops
- **switch** statements
- **case** statements
- **goto** statements
- **Ternary operators** (`? :`)
- **Variable Length Arrays (VLAs)**: `char str[argc]` is forbidden
- **Implicit type declarations**

### Comments

- **Location**: Outside function bodies only (end of line or own line)
- **(*) Language**: English only
- **(*) Useful comments**: Must be meaningful and useful
- **(*) No comment abuse**: Comments cannot justify "carryall" or bad functions with unclear purposes

### File Organization

- **No .c includes**: Cannot include .c files in other .c files
- **Function limit**: Maximum 5 function definitions per .c file

### Makefile Requirements

Makefiles are checked during evaluation, not by norminette:

- **Mandatory rules**: `$(NAME)`, `clean`, `fclean`, `re`, `all`
- **Default rule**: `all` (executed with just `make`)
- **No relinking**: Must not relink unnecessarily
- **Multibinary projects**: Separate rule for each binary, `all` compiles all
- **Library compilation**: Makefile must compile non-system libraries (e.g., libft) automatically
- **Explicit sources**: All source files must be explicitly named (no `*.c`, `*.o` wildcards)

### Validation

- **Norminette**: Run `norminette` to validate code compliance
- Repository: https://github.com/42School/norminette
- Install: Follow official 42 installation instructions for your campus

### Critical Reminders

1. **Functions do ONE thing**: Each function should have a single, clear, logical purpose
2. **Meaningful names**: Avoid `f1`, `f2`, `a`, `b`, `c` - use explicit names
3. **No obfuscation**: No one-liners, no tricks to bypass the norm
4. **File must compile**: Non-compiling code automatically fails the norm
5. **25 lines maximum**: Keep functions simple and focused
6. **4 parameters maximum**: If you need more, reconsider your design
7. **5 variables maximum**: Simplify complex functions
