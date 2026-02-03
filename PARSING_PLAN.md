# Minishell Parsing Plan - AST Approach

## Overview

This document details the parsing strategy for minishell, focusing on building an Abstract Syntax Tree (AST) that can be easily traversed during execution.

## Why AST?

**Advantages:**
1. **Clean separation**: Parsing and execution are completely separate phases
2. **Easy to traverse**: Tree structure naturally represents command hierarchy
3. **Extensible**: Easy to add new features (bonus: &&, ||, parentheses)
4. **Debuggable**: Can visualize/print tree structure
5. **Norm-friendly**: Natural breakdown into small, focused functions

## Parsing Phases

### Phase 1: Tokenization (Lexical Analysis)

**Input:** Raw string from readline()
**Output:** Linked list of tokens

**Responsibilities:**
- Break input into meaningful units (tokens)
- Recognize operators, words, redirections
- Handle quote boundaries
- Detect unclosed quotes
- Mark positions for error reporting

**Does NOT:**
- Expand variables (done later in expansion phase)
- Validate syntax (done in parser)
- Execute anything

### Phase 2: Syntax Validation

**Input:** Token list
**Output:** Boolean (valid/invalid)

**Checks:**
- Operators not at start/end
- Redirections followed by filenames
- Balanced parentheses (bonus)
- No consecutive operators
- No empty commands

### Phase 3: Parsing (Syntax Analysis)

**Input:** Validated token list
**Output:** AST root node

**Responsibilities:**
- Build hierarchical tree structure
- Respect operator precedence
- Attach redirections to commands
- Create appropriate node types

**Uses:** Recursive descent parsing with precedence climbing

### Phase 4: Expansion (Semantic Analysis)

**Input:** AST
**Output:** Expanded AST

**Responsibilities:**
- Expand environment variables (`$VAR`, `$?`)
- Remove quote characters
- Expand wildcards (bonus: `*`)
- Split words on whitespace (if needed)

### Phase 5: Execution

**Input:** Expanded AST
**Output:** Exit status

**Responsibilities:**
- Traverse tree (post-order)
- Fork processes
- Setup pipes and redirections
- Execute commands
- Return exit status

## Detailed Parsing Algorithm

### Token Stream to AST

The parser uses **recursive descent with operator precedence climbing**. Each precedence level has its own function that calls the next higher precedence level.

```
parse_or()          # Lowest precedence (||)
  └─> parse_and()   # &&
        └─> parse_pipe()  # |
              └─> parse_primary()  # Commands, ()
                    └─> parse_command()  # Words + redirects
```

### Precedence Table

| Operator | Precedence | Associativity | Function |
|----------|------------|---------------|----------|
| `\|\|`   | 1 (lowest) | Left          | parse_or() |
| `&&`     | 2          | Left          | parse_and() |
| `\|`     | 3          | Left          | parse_pipe() |
| `<`,`>`,`>>`,`<<` | 4 (highest) | N/A | Attached to commands |
| `()`     | Overrides all | N/A       | parse_primary() |

### Parse Function Details

#### parse_or()

```
Algorithm:
1. left = parse_and()
2. While current token is OR (||):
   a. Consume OR token
   b. right = parse_and()
   c. Create NODE_OR with left and right
   d. left = new node (left-associative)
3. Return left

Example: cmd1 || cmd2 || cmd3
Result:
      OR
     /  \
   OR   cmd3
  /  \
cmd1 cmd2
```

#### parse_and()

```
Algorithm:
1. left = parse_pipe()
2. While current token is AND (&&):
   a. Consume AND token
   b. right = parse_pipe()
   c. Create NODE_AND with left and right
   d. left = new node
3. Return left

Example: cmd1 && cmd2 && cmd3
Result:
      AND
     /   \
   AND   cmd3
  /  \
cmd1 cmd2
```

#### parse_pipe()

```
Algorithm:
1. left = parse_primary()
2. While current token is PIPE (|):
   a. Consume PIPE token
   b. right = parse_primary()
   c. Create NODE_PIPE with left and right
   d. left = new node
3. Return left

Example: cmd1 | cmd2 | cmd3
Result:
      PIPE
     /    \
   PIPE   cmd3
  /   \
cmd1  cmd2
```

#### parse_primary()

```
Algorithm:
1. If token is LPAREN (:
   a. Consume LPAREN
   b. node = parse_or()  (restart from lowest precedence)
   c. Expect RPAREN )
   d. Consume RPAREN
   e. node = attach_redirections(node)
   f. Return NODE_SUBSHELL(node)
2. Else:
   a. Return parse_command()

Example: (cmd1 | cmd2) && cmd3
Step 1: parse_primary() sees LPAREN
Step 2: Recursively parse "cmd1 | cmd2" as NODE_PIPE
Step 3: Wrap in NODE_SUBSHELL
Step 4: parse_and() combines with cmd3
```

#### parse_command()

```
Algorithm:
1. If no WORD tokens → syntax error
2. Collect consecutive WORD tokens into argv array:
   - Stop at operators (|, &&, ||)
   - Stop at parentheses
   - Skip and collect redirections separately
3. Parse redirections:
   - For each redirection operator (<, >, >>, <<):
     a. Expect WORD token (filename/delimiter)
     b. Create redirect structure
     c. Add to redirect list
4. Create NODE_COMMAND with argv and redirects
5. Return node

Example: cat < input.txt | grep pattern > output.txt
First command parse:
  argv: [cat, NULL]
  redirects: [REDIR_IN: input.txt]
Second command parse:
  argv: [grep, pattern, NULL]
  redirects: [REDIR_OUT: output.txt]
```

### Handling Redirections

Redirections are **attached to the command they belong to**, not separate nodes in the tree.

**Rules:**
- Redirections can appear anywhere around a command
- Multiple redirections are allowed
- Last redirection of same type wins (bash behavior)

**Examples:**

1. `< input cat > output`
   ```
   NODE_COMMAND
   argv: [cat, NULL]
   redirects: [REDIR_IN: input, REDIR_OUT: output]
   ```

2. `cat > file1 > file2` (last one wins)
   ```
   NODE_COMMAND
   argv: [cat, NULL]
   redirects: [REDIR_OUT: file1, REDIR_OUT: file2]
   ```

3. Redirections on subshells: `(cmd1 | cmd2) > output`
   ```
   NODE_SUBSHELL
   child: NODE_PIPE(cmd1, cmd2)
   redirects: [REDIR_OUT: output]
   ```

### Heredoc Special Handling

Heredocs require reading multiple lines until delimiter:

```
Algorithm:
1. When << token encountered:
   a. Next token is delimiter
   b. Check if delimiter has quotes:
      - Quoted (<<'EOF' or <<"EOF") → no expansion
      - Unquoted (<<EOF) → expand variables
   c. Read lines from readline() until delimiter found
   d. Write content to temporary file
   e. Create REDIR_IN redirection pointing to temp file
2. Attach to command
```

**Example:**
```bash
cat << EOF
hello $USER
world
EOF
```

1. Delimiter: `EOF` (unquoted → expand)
2. Read lines: `hello $USER` and `world`
3. Expand: `hello john` (if USER=john)
4. Write to temp file: `/tmp/minishell_heredoc_123`
5. Redirect stdin to temp file

## AST Node Structure Reference

### Node Types

```c
typedef enum e_node_type
{
    NODE_COMMAND,        // Simple command: [argv, redirects]
    NODE_PIPE,           // Binary: [left | right]
    NODE_AND,            // Binary: [left && right]
    NODE_OR,             // Binary: [left || right]
    NODE_SUBSHELL        // Unary: [(child)]
}   t_node_type;
```

### Command Node

```c
typedef struct s_cmd_node
{
    char        **argv;         // [cmd, arg1, arg2, NULL]
    t_redirect  *redirects;     // Linked list
}   t_cmd_node;
```

### Binary Operator Node

```c
typedef struct s_binop_node
{
    t_ast_node  *left;
    t_ast_node  *right;
}   t_binop_node;
```

### Subshell Node

```c
typedef struct s_subshell_node
{
    t_ast_node  *child;         // Content
    t_redirect  *redirects;     // Applied to subshell
}   t_subshell_node;
```

### Redirection

```c
typedef struct s_redirect
{
    t_node_type         type;   // REDIR_IN/OUT/APPEND/HEREDOC
    char                *file;
    int                 heredoc_fd;
    struct s_redirect   *next;
}   t_redirect;
```

## Complete Examples

### Example 1: Simple Pipeline

**Input:** `cat file.txt | grep pattern | wc -l`

**Tokens:**
```
[WORD:cat] [WORD:file.txt] [PIPE] [WORD:grep] [WORD:pattern] [PIPE] [WORD:wc] [WORD:-l] [EOF]
```

**AST:**
```
           NODE_PIPE
          /         \
     NODE_PIPE    NODE_COMMAND
     /       \        |
NODE_CMD  NODE_CMD   argv: [wc, -l]
    |         |
argv:     argv:
[cat,     [grep,
 file.txt] pattern]
```

**Execution Order (Post-order traversal):**
1. Execute `cat file.txt`
2. Execute `grep pattern`
3. Connect with pipe
4. Execute `wc -l`
5. Connect with pipe
6. Wait for all, return exit status

### Example 2: Redirections with Pipe

**Input:** `cat < input.txt | grep pattern > output.txt`

**AST:**
```
         NODE_PIPE
        /         \
  NODE_COMMAND  NODE_COMMAND
  argv: [cat]   argv: [grep, pattern]
  redirects:    redirects:
    REDIR_IN:     REDIR_OUT:
    input.txt     output.txt
```

**Execution:**
1. Fork for `cat`:
   - Open input.txt for stdin
   - Setup pipe stdout
   - Exec cat
2. Fork for `grep`:
   - Setup pipe stdin
   - Open output.txt for stdout
   - Exec grep
3. Wait for both

### Example 3: Logical Operators

**Input:** `make && ./program || echo "failed"`

**AST:**
```
              NODE_OR
             /       \
        NODE_AND    NODE_CMD
        /      \       |
   NODE_CMD  NODE_CMD  argv: [echo, "failed"]
      |         |
   argv:     argv:
   [make]    [./program]
```

**Execution (Short-circuit):**
1. Execute `make`:
   - If fails (exit != 0) → skip `./program`, go to OR
   - If succeeds (exit == 0) → execute `./program`
2. AND result:
   - If succeeded → don't execute echo (OR already satisfied)
   - If failed → execute echo
3. Return final exit status

### Example 4: Complex with Parentheses (Bonus)

**Input:** `(cmd1 && cmd2) | cmd3 > out || cmd4`

**AST:**
```
                NODE_OR
               /       \
          NODE_PIPE   NODE_CMD
          /       \      |
   NODE_SUBSHELL NODE_CMD argv: [cmd4]
         |           |
     NODE_AND     argv: [cmd3]
     /      \     redirects:
NODE_CMD  NODE_CMD   REDIR_OUT: out
   |         |
argv:     argv:
[cmd1]    [cmd2]
```

**Execution:**
1. Fork subshell:
   - Execute AND: cmd1 && cmd2
   - Write to pipe
2. Fork cmd3:
   - Read from pipe
   - Redirect stdout to "out"
   - Execute
3. If pipeline fails:
   - Execute cmd4
4. Return exit status

## Error Handling in Parser

### Syntax Errors to Detect

1. **Missing operand:**
   - `| cmd` → pipe at start
   - `cmd |` → pipe at end
   - `cmd &&` → AND at end

2. **Missing filename:**
   - `cat <` → no file after redirect
   - `cmd > | other` → redirect without target

3. **Unmatched parentheses:**
   - `(cmd1 | cmd2` → missing )
   - `cmd1 | cmd2)` → extra )

4. **Empty commands:**
   - `cmd1 && && cmd2` → consecutive operators
   - `()` → empty subshell

### Error Reporting

```c
void    syntax_error(char *token)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    ft_putstr_fd(token, 2);
    ft_putstr_fd("'\n", 2);
}
```

**Examples:**
- `minishell: syntax error near unexpected token `|'`
- `minishell: syntax error: unexpected end of file`
- `minishell: syntax error: unmatched parenthesis`

## Memory Management

### Allocation Strategy

1. **Token list:** Linked list, each token allocated separately
2. **AST nodes:** Allocated recursively as tree is built
3. **Argv arrays:** NULL-terminated arrays for each command
4. **Redirections:** Linked list for each command

### Deallocation Strategy

Free in reverse order of construction:

```c
void free_ast(t_ast_node *node)
{
    if (!node)
        return;

    // Free children first (post-order)
    if (node->type == NODE_PIPE || node->type == NODE_AND || node->type == NODE_OR)
    {
        free_ast(node->data.binop.left);
        free_ast(node->data.binop.right);
    }
    else if (node->type == NODE_SUBSHELL)
    {
        free_ast(node->data.subshell.child);
        free_redirect_list(node->data.subshell.redirects);
    }
    else if (node->type == NODE_COMMAND)
    {
        free_argv(node->data.cmd.argv);
        free_redirect_list(node->data.cmd.redirects);
    }

    // Free node itself
    free(node);
}
```

### Error Cleanup

If parsing fails midway:

```c
void cleanup_parse_error(t_token *tokens, t_ast_node *partial_ast)
{
    free_tokens(tokens);
    free_ast(partial_ast);
}
```

## Implementation Checklist

### Tokenizer
- [ ] Token structure and types
- [ ] Basic tokenization (words, spaces)
- [ ] Quote handling (single and double)
- [ ] Operator recognition (all types)
- [ ] Unclosed quote detection
- [ ] Token list creation

### Parser
- [ ] AST node structures
- [ ] parse_or() - logical OR
- [ ] parse_and() - logical AND
- [ ] parse_pipe() - pipeline
- [ ] parse_command() - simple commands
- [ ] parse_primary() - parentheses
- [ ] Redirection parsing
- [ ] Syntax validation
- [ ] Error reporting

### Expander
- [ ] Variable expansion ($VAR, $?)
- [ ] Quote removal
- [ ] Wildcard expansion (bonus)
- [ ] AST traversal for expansion

### Executor
- [ ] Command execution
- [ ] Pipeline setup
- [ ] Redirection setup
- [ ] Logical operator execution
- [ ] Subshell execution (bonus)
- [ ] Built-in detection and execution

## Testing Plan

### Unit Tests

1. **Tokenizer:**
   - `echo hello` → [WORD, WORD, EOF]
   - `cat | grep` → [WORD, PIPE, WORD, EOF]
   - `"hello world"` → [WORD:"hello world", EOF]

2. **Parser:**
   - Simple: `echo test` → NODE_COMMAND
   - Pipe: `cat | grep` → NODE_PIPE
   - Redirect: `cat > file` → NODE_COMMAND with redirect
   - Logical: `cmd1 && cmd2` → NODE_AND

3. **Expander:**
   - `$HOME` → expanded value
   - `'$HOME'` → literal `$HOME`
   - `"$HOME"` → expanded value

### Integration Tests

Compare with bash output:
```bash
./minishell > my_output
bash > bash_output
diff my_output bash_output
```

Test cases:
- All examples in this document
- Edge cases (empty quotes, multiple spaces, etc.)
- Error cases (syntax errors)
- Signal handling (ctrl-C, ctrl-D)

## Questions to Clarify

Before implementation, consider:

1. **Heredoc delimiter quotes:** How to handle `<<'EOF'` vs `<<"EOF"` vs `<<EOF`?
   - **Answer:** All three should prevent expansion if quoted

2. **Multiple redirections:** `cat < f1 < f2` - which file wins?
   - **Answer:** Last one (f2) - bash behavior

3. **Subshell redirections:** Can redirections apply to entire subshell?
   - **Answer:** Yes, `(cmd1 | cmd2) > file` is valid

4. **Empty parentheses:** Is `()` valid?
   - **Answer:** Syntax error (no command)

5. **Wildcard in quotes:** Should `"*.txt"` expand?
   - **Answer:** No, only unquoted wildcards expand

When in doubt, **test with bash** and match its behavior!
