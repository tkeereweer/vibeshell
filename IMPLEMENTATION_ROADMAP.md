# Minishell Implementation Roadmap

## Overview

This document provides a step-by-step implementation plan for building the minishell tokenizer and parser. All code will follow the 42 Norm (Version 4.1).

## Quick Reference: 42 Norm Constraints

- ✅ **25 lines max** per function (excluding braces)
- ✅ **5 variables max** per function
- ✅ **4 parameters max** per function
- ✅ **No for loops** (use while)
- ✅ **No ternary operators** (use if-else)
- ✅ **No semicolons** (except in forbidden for loops)
- ✅ **One global variable** (signal handling only)
- ✅ **Max 5 functions** per .c file

## Project Structure

```
vibeshell/
├── includes/
│   ├── minishell.h          # Main header
│   ├── tokens.h             # Token types and structures
│   ├── lexer.h              # Tokenizer prototypes
│   ├── ast.h                # AST node structures
│   └── parser.h             # Parser prototypes
├── src/
│   ├── lexer/               # Tokenizer implementation
│   │   ├── tokenize.c
│   │   ├── lexer_process.c
│   │   ├── state_handlers.c
│   │   ├── quote_handlers.c
│   │   ├── operator_handler.c
│   │   ├── char_classify.c
│   │   ├── operator_recognize.c
│   │   ├── token_utils.c
│   │   └── buffer_utils.c
│   ├── parser/              # Parser implementation
│   │   ├── parse.c
│   │   ├── parse_precedence.c
│   │   ├── parse_primary.c
│   │   ├── parse_command.c
│   │   ├── parse_redirect.c
│   │   ├── node_create.c
│   │   ├── node_free.c
│   │   ├── parse_helpers.c
│   │   ├── validate_syntax.c
│   │   └── parse_error.c
│   └── utils/
│       └── error.c
├── tests/                   # Test files
│   ├── test_tokenizer.c
│   └── test_parser.c
├── libft/                   # Your libft
└── Makefile
```

## Implementation Phases

### Phase 1: Setup & Foundation (Day 1)

**Goal:** Set up project structure and create all header files

**Tasks:**
1. Create directory structure
2. Write all header files with complete definitions
3. Set up Makefile with compilation rules
4. Ensure libft is available and has required functions

**Required libft functions:**
- `ft_strdup` - String duplication
- `ft_strlen` - String length
- `ft_putstr_fd` - Output to file descriptor
- `ft_strcmp` - String comparison (for testing)

**Deliverables:**
- [ ] All directories created
- [ ] All header files written
- [ ] Makefile compiles (even with empty .c files)
- [ ] Headers compile without errors

---

### Phase 2: Tokenizer - Foundation (Day 2-3)

**Goal:** Implement core token infrastructure

**Implementation order:**

1. **Error utilities** (`src/utils/error.c`):
   ```c
   void error_exit(char *msg, int code);
   void error_print(char *msg);
   ```

2. **Token utilities** (`src/lexer/token_utils.c`):
   ```c
   t_token *token_create(t_token_type type, char *value, int pos);
   void token_append(t_lexer *lex, t_token *token);
   void token_list_free(t_token *tokens);
   ```

3. **Character classification** (`src/lexer/char_classify.c`):
   ```c
   int is_whitespace(char c);
   int is_operator_char(char c);
   int is_quote(char c);
   int is_special_char(char c);
   ```

4. **Buffer utilities** (`src/lexer/buffer_utils.c`):
   ```c
   char *buffer_extract(t_lexer *lex, int start, int end);
   void buffer_add_word_token(t_lexer *lex);
   ```

**Testing:**
```c
// Test token creation
t_token *tok = token_create(TOKEN_WORD, "echo", 0);
assert(tok->type == TOKEN_WORD);
assert(strcmp(tok->value, "echo") == 0);
token_list_free(tok);
```

**Deliverables:**
- [ ] Token utilities work correctly
- [ ] Character classification functions return expected values
- [ ] Buffer extraction works
- [ ] No memory leaks (valgrind clean)

---

### Phase 3: Tokenizer - State Machine (Day 4-6)

**Goal:** Implement state machine for tokenization

**Implementation order:**

1. **Lexer initialization** (`src/lexer/tokenize.c`):
   ```c
   t_lexer *lexer_init(char *input);
   void lexer_free(t_lexer *lex);
   t_token *tokenize(char *input);  // Main entry point
   ```

2. **State handlers** (`src/lexer/state_handlers.c`):
   ```c
   void handle_initial_state(t_lexer *lex);
   void handle_word_state(t_lexer *lex);
   ```

3. **Quote handlers** (`src/lexer/quote_handlers.c`):
   ```c
   void handle_squote_state(t_lexer *lex);
   void handle_dquote_state(t_lexer *lex);
   ```

4. **Operator handler** (`src/lexer/operator_handler.c`):
   ```c
   void handle_operator_state(t_lexer *lex);
   ```

5. **Operator recognition** (`src/lexer/operator_recognize.c`):
   ```c
   int try_two_char_operator(t_lexer *lex);
   int try_one_char_operator(t_lexer *lex);
   ```

6. **Main processing loop** (`src/lexer/lexer_process.c`):
   ```c
   int lexer_process(t_lexer *lex);
   ```

**Testing at each step:**
```bash
# Test words
Input: "echo hello world"
Expected: [WORD:"echo"] [WORD:"hello"] [WORD:"world"] [EOF]

# Test quotes
Input: "echo 'hello world'"
Expected: [WORD:"echo"] [WORD:"'hello world'"] [EOF]

# Test operators
Input: "cat | grep"
Expected: [WORD:"cat"] [PIPE:"|"] [WORD:"grep"] [EOF]

# Test redirections
Input: "cat < in >> out"
Expected: [WORD:"cat"] [REDIR_IN:"<"] [WORD:"in"] [REDIR_APPEND:">>"] [WORD:"out"] [EOF]
```

**Deliverables:**
- [ ] Simple words tokenize correctly
- [ ] Quotes handled properly (single and double)
- [ ] All operators recognized (|, <, >, >>, <<, &&, ||, (, ))
- [ ] Unclosed quotes detected as errors
- [ ] No memory leaks
- [ ] All functions pass norminette

---

### Phase 4: Parser - Foundation (Day 7-8)

**Goal:** Set up parser infrastructure and node creation

**Implementation order:**

1. **Node creation** (`src/parser/node_create.c`):
   ```c
   t_ast_node *create_cmd_node(char **argv, t_redirect *redir);
   t_ast_node *create_binop_node(t_node_type type, t_ast_node *left, t_ast_node *right);
   t_ast_node *create_subshell_node(t_ast_node *child, t_redirect *redir);
   ```

2. **Node cleanup** (`src/parser/node_free.c`):
   ```c
   void free_argv(char **argv);
   void free_redirect_list(t_redirect *redir);
   void free_ast(t_ast_node *node);  // Recursive
   ```

3. **Parser helpers** (`src/parser/parse_helpers.c`):
   ```c
   int match_token(t_parse_ctx *ctx, t_token_type type);
   void advance_token(t_parse_ctx *ctx);
   t_token *peek_token(t_parse_ctx *ctx);
   int is_redir_token(t_token_type type);
   int is_operator_token(t_token_type type);
   ```

4. **Parser entry point** (`src/parser/parse.c`):
   ```c
   t_parse_ctx *init_parse_ctx(t_token *tokens);
   t_ast_node *parse_tokens(t_token *tokens);
   int is_at_end(t_parse_ctx *ctx);
   t_ast_node *make_binop(t_node_type type, t_ast_node *l, t_ast_node *r);
   void cleanup_parse(t_parse_ctx *ctx, t_ast_node *node);
   ```

**Testing:**
```c
// Test node creation
t_ast_node *node = create_cmd_node(argv, NULL);
assert(node->type == NODE_COMMAND);
free_ast(node);

// Test helpers
t_parse_ctx *ctx = init_parse_ctx(tokens);
assert(match_token(ctx, TOKEN_WORD));
advance_token(ctx);
assert(match_token(ctx, TOKEN_PIPE));
```

**Deliverables:**
- [ ] Can create all node types
- [ ] Memory cleanup works correctly
- [ ] Parser helpers navigate tokens correctly
- [ ] No memory leaks

---

### Phase 5: Parser - Simple Commands (Day 9-10)

**Goal:** Parse commands without operators

**Implementation order:**

1. **Argv building** (`src/parser/parse_command.c`):
   ```c
   int count_words(t_parse_ctx *ctx);
   char **alloc_argv_array(int count);
   void fill_argv_array(t_parse_ctx *ctx, char **argv, int count);
   char **build_argv(t_parse_ctx *ctx, int *out_count);
   t_ast_node *parse_command(t_parse_ctx *ctx);
   ```

2. **Error reporting** (`src/parser/parse_error.c`):
   ```c
   void parse_error(char *msg, char *token);
   void syntax_error(char *token);
   void unexpected_eof_error(void);
   void unmatched_paren_error(void);
   ```

**Testing:**
```bash
# Simple command
Input tokens: [WORD:"echo"] [WORD:"hello"] [EOF]
Expected AST: NODE_COMMAND with argv=["echo", "hello", NULL]

# Multiple arguments
Input tokens: [WORD:"ls"] [WORD:"-la"] [WORD:"/tmp"] [EOF]
Expected AST: NODE_COMMAND with argv=["ls", "-la", "/tmp", NULL]
```

**Deliverables:**
- [ ] Simple commands parse correctly
- [ ] Argv arrays built properly
- [ ] Error messages displayed for invalid syntax
- [ ] Norminette passes

---

### Phase 6: Parser - Redirections (Day 11-12)

**Goal:** Add redirection support to commands

**Implementation order:**

1. **Redirection parsing** (`src/parser/parse_redirect.c`):
   ```c
   t_redir_type token_to_redir(t_token_type type);
   t_redirect *create_redirect(t_token_type type, char *file);
   void append_redirect(t_redirect **head, t_redirect *new);
   t_redirect *parse_single_redir(t_parse_ctx *ctx);
   t_redirect *parse_redirections(t_parse_ctx *ctx);
   ```

2. **Integration with commands:**
   - Modify `parse_command()` to call `parse_redirections()`

**Testing:**
```bash
# Input redirection
Input: [WORD:"cat"] [REDIR_IN:"<"] [WORD:"input.txt"] [EOF]
Expected: NODE_COMMAND with redirects=[REDIR_IN: "input.txt"]

# Multiple redirections
Input: [WORD:"cat"] [REDIR_IN:"<"] [WORD:"in"] [REDIR_OUT:">"] [WORD:"out"] [EOF]
Expected: NODE_COMMAND with redirects=[REDIR_IN:"in", REDIR_OUT:"out"]

# Heredoc
Input: [WORD:"cat"] [REDIR_HEREDOC:"<<"] [WORD:"EOF"] [EOF]
Expected: NODE_COMMAND with redirects=[REDIR_HEREDOC:"EOF"]
```

**Deliverables:**
- [ ] All redirection types work (<, >, >>, <<)
- [ ] Multiple redirections per command supported
- [ ] Redirections can appear anywhere around command
- [ ] Error for missing filename after redirect operator

---

### Phase 7: Parser - Pipelines (Day 13-14)

**Goal:** Parse pipe operator

**Implementation order:**

1. **Primary parser** (`src/parser/parse_primary.c`):
   ```c
   int is_command_start(t_parse_ctx *ctx);
   int has_command_word(t_parse_ctx *ctx);
   t_ast_node *parse_primary(t_parse_ctx *ctx);
   // parse_subshell() will be added later for bonus
   ```

2. **Pipeline parser** (`src/parser/parse_precedence.c`):
   ```c
   t_ast_node *parse_pipe(t_parse_ctx *ctx);
   // parse_and() and parse_or() will be added later for bonus
   ```

3. **Update entry point:**
   - `parse_tokens()` should now call `parse_pipe()` as top-level

**Testing:**
```bash
# Simple pipe
Input: [WORD:"cat"] [PIPE:"|"] [WORD:"grep"] [EOF]
Expected: NODE_PIPE with left=NODE_COMMAND(cat), right=NODE_COMMAND(grep)

# Three-stage pipeline
Input: [WORD:"cat"] [PIPE] [WORD:"grep"] [PIPE] [WORD:"wc"] [EOF]
Expected: NODE_PIPE(NODE_PIPE(cat, grep), wc)

# Pipeline with redirections
Input: [WORD:"cat"] [REDIR_IN] [WORD:"in"] [PIPE] [WORD:"grep"] [REDIR_OUT] [WORD:"out"] [EOF]
Expected: NODE_PIPE with left=cat+redirect, right=grep+redirect
```

**Deliverables:**
- [ ] Simple pipelines work
- [ ] Multi-stage pipelines work (left-associative)
- [ ] Pipelines with redirections work
- [ ] Syntax error for pipe at start/end

---

### Phase 8: Parser - Validation (Day 15)

**Goal:** Pre-parse syntax validation

**Implementation order:**

1. **Syntax validation** (`src/parser/validate_syntax.c`):
   ```c
   int check_operator_position(t_token *tok);
   int check_redirect_has_file(t_token *tok);
   int check_balanced_parens(t_token *tokens);
   int validate_syntax(t_token *tokens);
   ```

2. **Integration:**
   - `parse_tokens()` calls `validate_syntax()` before parsing

**Testing:**
```bash
# Error: pipe at start
Input: [PIPE] [WORD:"cat"] [EOF]
Expected: Syntax error

# Error: missing filename
Input: [WORD:"cat"] [REDIR_IN] [EOF]
Expected: Syntax error

# Error: consecutive operators
Input: [WORD:"cat"] [PIPE] [PIPE] [WORD:"grep"] [EOF]
Expected: Syntax error
```

**Deliverables:**
- [ ] Operator position validation works
- [ ] Redirect filename validation works
- [ ] Parenthesis balance check works (for bonus prep)
- [ ] Clear error messages displayed

---

### Phase 9: BONUS - Logical Operators (Day 16-17)

**Goal:** Add && and || support

**Implementation order:**

1. **Logical parsers** (`src/parser/parse_precedence.c`):
   ```c
   t_ast_node *parse_and(t_parse_ctx *ctx);
   t_ast_node *parse_or(t_parse_ctx *ctx);
   ```

2. **Update entry point:**
   - `parse_tokens()` now calls `parse_or()` as top-level

**Testing:**
```bash
# AND operator
Input: [WORD:"make"] [AND:"&&"] [WORD:"./program"] [EOF]
Expected: NODE_AND with left=make, right=./program

# OR operator
Input: [WORD:"cmd1"] [OR:"||"] [WORD:"cmd2"] [EOF]
Expected: NODE_OR with left=cmd1, right=cmd2

# Precedence: || < &&
Input: [WORD:"a"] [AND] [WORD:"b"] [OR] [WORD:"c"] [EOF]
Expected: NODE_OR(NODE_AND(a, b), c)

# Precedence: && < |
Input: [WORD:"a"] [PIPE] [WORD:"b"] [AND] [WORD:"c"] [EOF]
Expected: NODE_AND(NODE_PIPE(a, b), c)
```

**Deliverables:**
- [ ] && operator works
- [ ] || operator works
- [ ] Correct precedence: || < && < |
- [ ] Left-associative for same precedence

---

### Phase 10: BONUS - Parentheses (Day 18-19)

**Goal:** Add subshell support

**Implementation order:**

1. **Subshell parser** (`src/parser/parse_primary.c`):
   ```c
   static t_ast_node *parse_subshell(t_parse_ctx *ctx);
   // Update parse_primary() to handle LPAREN
   ```

**Testing:**
```bash
# Simple subshell
Input: [LPAREN] [WORD:"echo"] [RPAREN] [EOF]
Expected: NODE_SUBSHELL with child=NODE_COMMAND(echo)

# Subshell in pipeline
Input: [LPAREN] [WORD:"cat"] [RPAREN] [PIPE] [WORD:"grep"] [EOF]
Expected: NODE_PIPE(NODE_SUBSHELL(cat), grep)

# Override precedence
Input: [LPAREN] [WORD:"a"] [PIPE] [WORD:"b"] [RPAREN] [AND] [WORD:"c"] [EOF]
Expected: NODE_AND(NODE_SUBSHELL(PIPE(a,b)), c)

# Nested
Input: [LPAREN] [LPAREN] [WORD:"cmd"] [RPAREN] [RPAREN] [EOF]
Expected: NODE_SUBSHELL(NODE_SUBSHELL(cmd))
```

**Deliverables:**
- [ ] Parentheses create subshells
- [ ] Subshells can have redirections
- [ ] Parentheses override operator precedence
- [ ] Nested parentheses work
- [ ] Error on unmatched parentheses

---

## Testing Strategy

### Unit Tests

Create test files for each component:

**`tests/test_tokenizer.c`:**
```c
void run_tokenizer_tests(void)
{
    test_simple_word();
    test_multiple_words();
    test_single_quotes();
    test_double_quotes();
    test_mixed_quotes();
    test_operators();
    test_redirections();
    test_unclosed_quote_error();
}
```

**`tests/test_parser.c`:**
```c
void run_parser_tests(void)
{
    test_simple_command();
    test_command_with_args();
    test_redirections();
    test_pipeline();
    test_logical_operators();
    test_parentheses();
    test_syntax_errors();
}
```

### Integration Tests

Compare with bash:
```bash
#!/bin/bash
# test_integration.sh

for cmd in \
    "echo hello" \
    "cat | grep" \
    "cat < in > out" \
    "cmd1 && cmd2 || cmd3" \
    "(cmd1 | cmd2) && cmd3"
do
    echo "Testing: $cmd"
    bash_out=$(bash -c "$cmd" 2>&1)
    mini_out=$(./minishell -c "$cmd" 2>&1)
    if [ "$bash_out" == "$mini_out" ]; then
        echo "✓ PASS"
    else
        echo "✗ FAIL"
        echo "  Bash: $bash_out"
        echo "  Mine: $mini_out"
    fi
done
```

### Memory Leak Testing

```bash
# Run with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./minishell

# Test each component
valgrind ./test_tokenizer
valgrind ./test_parser
```

### Norm Validation

```bash
# Check all source files
norminette includes/*.h
norminette src/lexer/*.c
norminette src/parser/*.c
norminette src/utils/*.c

# Should output: "No norm errors found"
```

---

## Debugging Tools

### Token Printer

```c
void print_tokens(t_token *tokens)
{
    int i = 0;
    while (tokens)
    {
        printf("[%d] ", i);
        print_token_type(tokens->type);
        printf(": '%s'\n", tokens->value ? tokens->value : "(null)");
        tokens = tokens->next;
        i++;
    }
}
```

### AST Printer

```c
void print_ast(t_ast_node *node, int depth)
{
    if (!node)
        return;
    print_indent(depth);
    print_node_type(node->type);
    if (node->type == NODE_COMMAND)
        print_command_node(&node->data.cmd);
    else if (node->type == NODE_PIPE || node->type == NODE_AND || node->type == NODE_OR)
    {
        print_ast(node->data.binop.left, depth + 1);
        print_ast(node->data.binop.right, depth + 1);
    }
    else if (node->type == NODE_SUBSHELL)
        print_ast(node->data.subshell.child, depth + 1);
}
```

---

## Common Issues & Solutions

### Issue 1: Function Too Long

**Problem:** Function exceeds 25 lines

**Solution:**
- Extract helper functions
- Each helper does ONE specific task
- Break complex conditions into separate checks

### Issue 2: Too Many Variables

**Problem:** Function needs more than 5 variables

**Solution:**
- Use structures to group related data
- Pass context structures instead of individual values
- Extract logic to helper function

### Issue 3: Memory Leaks

**Problem:** Valgrind reports leaks

**Solution:**
- Free in reverse order of allocation
- Use cleanup functions on error paths
- Check all early return paths have cleanup

### Issue 4: Segmentation Fault

**Problem:** Crash during parsing

**Solution:**
- Check for NULL before dereferencing
- Validate token list is not empty
- Ensure advance_token() doesn't go past EOF

---

## Checklist Before Moving Forward

### After Tokenizer:
- [ ] All token types recognized
- [ ] Quotes handled correctly
- [ ] Operators recognized (including multi-char)
- [ ] Unclosed quotes detected
- [ ] No memory leaks
- [ ] Norminette passes
- [ ] Unit tests pass

### After Parser:
- [ ] Simple commands parse
- [ ] Redirections work
- [ ] Pipelines work
- [ ] Logical operators work (bonus)
- [ ] Parentheses work (bonus)
- [ ] Syntax validation works
- [ ] No memory leaks
- [ ] Norminette passes
- [ ] Integration tests pass

### Before Submission:
- [ ] All mandatory features work
- [ ] Bonus features implemented (if attempting)
- [ ] No memory leaks (except readline)
- [ ] No segfaults
- [ ] Norminette clean
- [ ] Makefile correct (all, clean, fclean, re)
- [ ] Git repository clean

---

## Next Steps

1. **Review this roadmap** and the detailed plans in the agent outputs
2. **Set up the project structure** (directories, Makefile)
3. **Start with Phase 1**: Create all header files
4. **Implement incrementally**: Don't skip phases
5. **Test continuously**: After each function, test it
6. **Use version control**: Commit after each working phase
7. **Ask questions**: If something is unclear, ask before implementing

Ready to start implementation? I can help you:
- Write specific functions
- Debug issues
- Review code for Norm compliance
- Create test cases
- Optimize implementation
