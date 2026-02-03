# Minishell Architecture Plan

## Project Overview

This minishell implementation follows an **AST-based parsing approach** with clean separation between:
1. **Lexical Analysis (Tokenization)** - Convert input to tokens
2. **Syntax Analysis (Parsing)** - Build Abstract Syntax Tree
3. **Semantic Analysis (Expansion)** - Expand variables, remove quotes
4. **Execution** - Traverse AST and execute commands

All code must comply with the 42 Norm (Version 4.1).

## High-Level Pipeline

```
Input String (from readline)
    ↓
[TOKENIZER] - Lexical Analysis
    ↓
Token Stream [TOKEN_WORD, TOKEN_PIPE, TOKEN_WORD, ...]
    ↓
[VALIDATOR] - Syntax Validation
    ↓
[PARSER] - Build AST
    ↓
Abstract Syntax Tree (hierarchical structure)
    ↓
[EXPANDER] - Variable expansion, quote removal
    ↓
Expanded AST (ready for execution)
    ↓
[EXECUTOR] - Execute commands
    ↓
Exit Status
```

## Operator Precedence

From lowest to highest precedence:
1. **Logical OR** (`||`) - Left-associative, short-circuit evaluation
2. **Logical AND** (`&&`) - Left-associative, short-circuit evaluation
3. **Pipe** (`|`) - Left-associative
4. **Redirections** (`<`, `>`, `>>`, `<<`) - Attached to commands

Parentheses `()` can override precedence (bonus feature).

## Phase 1: Tokenization (Lexer)

### Token Types

```c
typedef enum e_token_type
{
    TOKEN_WORD,          // Commands, arguments, filenames
    TOKEN_PIPE,          // |
    TOKEN_REDIR_IN,      // <
    TOKEN_REDIR_OUT,     // >
    TOKEN_REDIR_APPEND,  // >>
    TOKEN_REDIR_HEREDOC, // <<
    TOKEN_AND,           // && (bonus)
    TOKEN_OR,            // || (bonus)
    TOKEN_LPAREN,        // ( (bonus)
    TOKEN_RPAREN,        // ) (bonus)
    TOKEN_EOF            // End of input
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    int             pos;         // Position in input (for errors)
    struct s_token  *next;
}   t_token;
```

### Tokenization Strategy: State Machine

**States:**
- `STATE_INITIAL` - Whitespace skipping, token start
- `STATE_WORD` - Building word tokens
- `STATE_SQUOTE` - Inside single quotes (no expansion)
- `STATE_DQUOTE` - Inside double quotes ($ expansion only)
- `STATE_OPERATOR` - Processing operators

**Key Functions:**
```c
t_token *tokenize(char *input);                    // Main entry
int      lexer_process(t_lexer *lexer);           // Character loop
void     handle_initial_state(t_lexer *lex, char c);
void     handle_word_state(t_lexer *lex, char c);
void     handle_squote_state(t_lexer *lex, char c);
void     handle_dquote_state(t_lexer *lex, char c);
void     handle_operator_state(t_lexer *lex, char c);
```

**Quote Handling:**
- Single quotes `'...'`: Everything literal, no expansion
- Double quotes `"..."`: Allow `$` expansion, everything else literal
- Track quote boundaries for later expansion phase
- Detect unclosed quotes as errors

**Operator Recognition:**
- Lookahead to distinguish: `<` vs `<<`, `>` vs `>>`, `|` vs `||`, `&` vs `&&`
- Create appropriate token type
- Advance position by 1 or 2 characters

### Example Tokenization

Input: `echo "hello world" | cat > file`

Tokens:
```
[TOKEN_WORD:"echo"]
[TOKEN_WORD:"hello world"]  (quotes preserved in value)
[TOKEN_PIPE:"|"]
[TOKEN_WORD:"cat"]
[TOKEN_REDIR_OUT:">"]
[TOKEN_WORD:"file"]
[TOKEN_EOF]
```

## Phase 2: Parsing (Building AST)

### AST Node Structure

```c
typedef enum e_node_type
{
    NODE_COMMAND,        // Simple command
    NODE_PIPE,           // Pipeline |
    NODE_AND,            // Logical AND &&
    NODE_OR,             // Logical OR ||
    NODE_SUBSHELL        // Parentheses () - bonus
}   t_node_type;

typedef struct s_redirect
{
    t_node_type         type;        // REDIR_IN/OUT/APPEND/HEREDOC
    char                *file;       // Filename or delimiter
    int                 heredoc_fd;  // Temp file fd for heredoc
    struct s_redirect   *next;
}   t_redirect;

typedef struct s_cmd_node
{
    char        **argv;         // NULL-terminated argument array
    t_redirect  *redirects;     // Linked list of redirections
}   t_cmd_node;

typedef struct s_binop_node
{
    struct s_ast_node   *left;
    struct s_ast_node   *right;
}   t_binop_node;

typedef struct s_subshell_node
{
    struct s_ast_node   *child;
    t_redirect          *redirects;  // Redirects for entire subshell
}   t_subshell_node;

typedef union u_node_data
{
    t_cmd_node          cmd;
    t_binop_node        binop;
    t_subshell_node     subshell;
}   t_node_data;

typedef struct s_ast_node
{
    t_node_type type;
    t_node_data data;
}   t_ast_node;
```

### Recursive Descent Parser

Parser respects operator precedence with separate functions for each level:

```c
t_ast_node *parse_tokens(t_token *tokens);    // Entry point
t_ast_node *parse_or(t_parse_ctx *ctx);       // || operator
t_ast_node *parse_and(t_parse_ctx *ctx);      // && operator
t_ast_node *parse_pipe(t_parse_ctx *ctx);     // | operator
t_ast_node *parse_primary(t_parse_ctx *ctx);  // Commands, ()
t_ast_node *parse_command(t_parse_ctx *ctx);  // Simple commands
t_redirect *parse_redirections(t_parse_ctx *ctx);
```

**Parsing Algorithm (Recursive Descent):**

1. `parse_or()`: Parse left with `parse_and()`, loop while `||` token
2. `parse_and()`: Parse left with `parse_pipe()`, loop while `&&` token
3. `parse_pipe()`: Parse left with `parse_primary()`, loop while `|` token
4. `parse_primary()`: Check for `(`, else parse command
5. `parse_command()`: Collect words for argv, parse redirections

### Example Parse Trees

**Example 1:** `cat file | grep text`
```
     NODE_PIPE
     /       \
NODE_CMD    NODE_CMD
(cat,file) (grep,text)
```

**Example 2:** `cat < in > out`
```
NODE_COMMAND
argv: [cat, NULL]
redirects:
  - REDIR_IN: "in"
  - REDIR_OUT: "out"
```

**Example 3:** `cmd1 && cmd2 || cmd3`
```
       NODE_OR
      /       \
  NODE_AND   NODE_CMD
  /      \    (cmd3)
CMD1    CMD2
```

**Example 4 (Bonus):** `(cmd1 | cmd2) && cmd3`
```
        NODE_AND
       /         \
NODE_SUBSHELL   NODE_CMD
     |           (cmd3)
 NODE_PIPE
 /       \
CMD1    CMD2
```

## Phase 3: Expansion

After parsing, before execution:

### Variable Expansion

- `$VAR` → Replace with environment variable value
- `$?` → Replace with last exit status
- **In single quotes**: No expansion
- **In double quotes**: Expand `$` only
- **Unquoted**: Expand `$`

### Quote Removal

After expansion, remove quote characters (`'` and `"`).

### Wildcard Expansion (Bonus)

- `*` expands to matching files in current directory
- Only in unquoted contexts

**Functions:**
```c
void expand_ast(t_ast_node *node, t_expand_ctx *ctx);
char *expand_string(char *str, t_expand_ctx *ctx);
char *remove_quotes(char *str);
char **expand_wildcard(char *pattern);
```

## Phase 4: Execution

### Execution Entry Point

```c
int execute_ast(t_ast_node *node, t_shell_ctx *ctx);
```

Dispatches based on node type:
- `NODE_COMMAND` → Execute simple command
- `NODE_PIPE` → Setup pipe, fork children
- `NODE_AND` → Execute left, if success execute right
- `NODE_OR` → Execute left, if failure execute right
- `NODE_SUBSHELL` → Fork, execute child in subshell

### Command Execution

1. Check if built-in → execute directly
2. Otherwise:
   - Fork process
   - Setup redirections
   - Execute with `execve()`
   - Wait for child, return exit status

### Pipeline Execution

For `cmd1 | cmd2 | cmd3`:
1. Create pipes between each command
2. Fork for each command
3. Redirect stdin/stdout to appropriate pipe ends
4. Close unused pipe ends
5. Wait for all children
6. Return exit status of last command

### Built-in Commands

Executed in parent process (no fork):
- `echo` - with `-n` option
- `cd` - change directory
- `pwd` - print working directory
- `export` - set environment variable
- `unset` - unset environment variable
- `env` - print environment
- `exit` - exit shell

## File Structure

```
vibeshell/
├── Makefile
├── CLAUDE.md              # 42 Norm reference
├── ARCHITECTURE.md        # This file
├── includes/
│   ├── minishell.h       # Main header
│   ├── tokens.h          # Token structures
│   ├── ast.h             # AST structures
│   ├── parser.h          # Parser prototypes
│   ├── executor.h        # Executor prototypes
│   └── builtins.h        # Built-in prototypes
├── src/
│   ├── main.c            # Main loop with readline
│   ├── lexer/
│   │   ├── tokenize.c
│   │   ├── lexer_states.c
│   │   ├── operator_utils.c
│   │   └── token_utils.c
│   ├── parser/
│   │   ├── parse.c
│   │   ├── parse_operators.c
│   │   ├── parse_command.c
│   │   ├── parse_redirect.c
│   │   └── node_create.c
│   ├── expander/
│   │   ├── expand_vars.c
│   │   ├── expand_quotes.c
│   │   └── expand_wildcard.c  (bonus)
│   ├── executor/
│   │   ├── execute.c
│   │   ├── exec_command.c
│   │   ├── exec_pipeline.c
│   │   ├── exec_redirect.c
│   │   └── exec_logical.c     (bonus)
│   ├── builtins/
│   │   ├── builtin_echo.c
│   │   ├── builtin_cd.c
│   │   ├── builtin_pwd.c
│   │   ├── builtin_export.c
│   │   ├── builtin_unset.c
│   │   ├── builtin_env.c
│   │   └── builtin_exit.c
│   ├── signals/
│   │   └── signal_handler.c
│   └── utils/
│       ├── error.c
│       ├── env_utils.c
│       └── memory.c
└── libft/                 # Your libft
```

## 42 Norm Compliance

### Critical Constraints

- **25 lines max** per function (excluding braces)
- **5 variables max** per function
- **4 parameters max** per function
- **No for loops** (use while)
- **No ternary operators** (use if-else)
- **No semicolons** (except in for, which is forbidden)
- **One global variable** (for signal handling only)

### Strategy

- Break complex logic into small helper functions
- Use structures to pass related data (reduces parameter count)
- Each function has ONE clear purpose
- Use while loops with explicit index incrementation

### Global Variable

```c
// Only allowed global - for signal handling
volatile sig_atomic_t g_signal = 0;
```

Must only store signal number, nothing else.

## Implementation Priority

### Week 1: Foundation
1. Project structure and Makefile
2. Basic token structures
3. Simple tokenizer (no quotes, no operators)
4. Basic AST node creation

### Week 2: Tokenization Complete
1. Quote handling in tokenizer
2. Operator recognition (all types)
3. Token validation
4. Unit tests for tokenizer

### Week 3: Parser
1. Parse simple commands
2. Parse redirections
3. Parse pipelines
4. Syntax validation

### Week 4: Execution
1. Basic command execution
2. Redirection setup
3. Pipeline execution
4. Variable expansion

### Week 5: Built-ins
1. Implement all 7 built-ins
2. Environment variable management
3. Exit status tracking

### Week 6: Polish
1. Signal handling (ctrl-C, ctrl-D, ctrl-\)
2. Memory leak checking (valgrind)
3. Edge case testing
4. Norm validation

### Week 7 (Optional): Bonus
1. Logical operators (&&, ||)
2. Parentheses/subshells
3. Wildcard expansion

## Critical Edge Cases

### Quotes
- Empty quotes: `echo ""` → valid empty argument
- Mixed quotes: `echo "a'b'c"` → `a'b'c`
- Unclosed: `echo "hello` → syntax error
- Variables: `echo '$HOME'` vs `echo "$HOME"`

### Redirections
- Multiple: `cat < in1 < in2` → last wins
- Order: `>out cat <in` → valid (order doesn't matter)
- Heredoc quotes: `<<EOF` (expand) vs `<<'EOF'` (no expand)

### Pipes
- Empty commands: `| cat` or `cat |` → syntax error
- Built-ins in pipeline: Run in subshell (don't affect parent)

### Logical Operators
- Short-circuit: `false && echo never` → echo not executed
- Precedence: `false || true && echo yes` → echo executes

## Signal Handling

### Interactive Mode

- **ctrl-C**: Display new prompt on new line
- **ctrl-D**: Exit shell (if input is empty)
- **ctrl-\**: Do nothing

### During Command Execution

- **ctrl-C**: Interrupt current command
- Set `g_signal` to indicate interrupt
- Return to prompt after cleanup

## Testing Strategy

### Unit Tests
- Tokenizer: Test each token type, quotes, operators
- Parser: Test precedence, syntax errors, tree structure
- Expander: Test variable expansion, quote removal
- Executor: Test simple commands, pipes, redirections

### Integration Tests
- Compare output with bash for same inputs
- Test all mandatory features
- Test edge cases
- Test error handling

### Norm Validation
```bash
norminette includes/ src/
```

### Memory Leaks
```bash
valgrind --leak-check=full ./minishell
```

Note: readline() may leak - acceptable per subject.

## Reference Implementation Behavior

When in doubt, use bash as reference:
```bash
bash$ echo "test" | cat > file && cat file
bash$ echo $?
```

Match bash's:
- Exit status behavior
- Error messages (similar format)
- Redirection semantics
- Pipeline execution
- Quote handling
