# Advanced Features Implementation Plan

## Overview
This document outlines the implementation of advanced minishell features:
1. Signal handling (SIGINT/SIGQUIT)
2. Heredoc input (<<)
3. Quote processing (quote removal)
4. Variable expansion ($VAR, $?)
5. Wildcards (*)

---

## Feature 1: Signal Handling

### Requirements
- **SIGINT (Ctrl+C)**:
  - Interactive mode: Display new prompt on new line
  - During command: Terminate foreground process, display newline
- **SIGQUIT (Ctrl+\)**:
  - Interactive mode: Ignored
  - During command: Terminate with "Quit: 3" message

### Implementation

**Files to Create:**
- `src/signals/signal_init.c` - Setup signal handlers
- `src/signals/signal_handlers.c` - Handler functions
- `includes/signals.h` - Signal prototypes

**Key Functions:**
```c
void    init_signals(void);           // Setup for interactive mode
void    ignore_signals(void);         // Ignore during child execution
void    default_signals(void);        // Restore defaults in child
void    sigint_handler(int sig);      // Handle Ctrl+C
```

**Signal Handler Logic:**
```c
// In parent (interactive):
signal(SIGINT, sigint_handler);   // Custom handler
signal(SIGQUIT, SIG_IGN);         // Ignore

// In child (command execution):
signal(SIGINT, SIG_DFL);          // Default (terminate)
signal(SIGQUIT, SIG_DFL);         // Default (terminate + core dump)
```

**Integration Points:**
- `main.c`: Call `init_signals()` before shell loop
- `exec_command.c`: Call `default_signals()` in child process
- `exec_pipe.c`: Call `default_signals()` in each pipe child
- `exec_subshell.c`: Call `default_signals()` in subshell child

**Testing:**
```bash
# Interactive mode
minishell> [Ctrl+C]  # New prompt
minishell> sleep 10 [Ctrl+C]  # Terminates, new prompt
minishell> cat [Ctrl+\]  # "Quit: 3"
```

---

## Feature 2: Heredoc Input (<<)

### Requirements
- Read lines until delimiter is found
- Store content in temporary location
- Redirect as stdin to command
- Handle multiple heredocs
- Variable expansion in heredoc content

### Implementation

**Files to Modify:**
- `src/executor/heredoc.c` - Heredoc processing

**Key Functions:**
```c
int     process_heredoc(t_redirect *redir);
char    *read_heredoc_content(char *delimiter);
int     create_heredoc_pipe(char *content);
```

**Heredoc Processing Flow:**
```
1. During parsing, store delimiter in t_redirect
2. Before execution, process all heredocs:
   a. Read lines with readline until delimiter
   b. Apply variable expansion to lines
   c. Create pipe, write content to write end
   d. Store read end fd in redir->heredoc_fd
3. During execution, dup2 heredoc_fd to stdin
4. After execution, close heredoc_fd
```

**Storage Strategy:**
- Use pipes instead of temp files (more secure)
- Process all heredocs before executing command
- Keep heredoc fds open until command completes

**Testing:**
```bash
minishell> cat << EOF
> line 1
> line 2
> EOF
line 1
line 2

minishell> cat << END | wc -l
> a
> b
> c
> END
3
```

---

## Feature 3: Quote Processing

### Requirements
- Remove quotes after parsing
- Single quotes ('): No expansion, literal
- Double quotes ("): Variable expansion allowed
- Escape sequences (\): Next character literal
- Quotes can be mixed: `echo '$USER' "$HOME"`

### Implementation

**Files to Create:**
- `src/expander/quote_removal.c` - Remove quotes from tokens

**Key Functions:**
```c
char    *remove_quotes(char *str);
char    *process_quoted_str(char *str, int *i);
int     is_quote_char(char c);
```

**Quote Removal Logic:**
```c
For each character in token:
1. If single quote:
   - Copy all chars until closing quote
   - Skip quote characters
2. If double quote:
   - Copy chars, but expand variables
   - Skip quote characters
3. If backslash (outside quotes):
   - Copy next character literally
   - Skip backslash
4. Otherwise: copy character
```

**Integration Point:**
- After parsing, before execution
- Process all WORD tokens in argv array
- Store result back in token value

**Testing:**
```bash
minishell> echo "hello world"     # hello world
minishell> echo 'hello world'     # hello world
minishell> echo hello' 'world     # hello world
minishell> echo "$USER"           # username
minishell> echo '$USER'           # $USER
```

---

## Feature 4: Variable Expansion

### Requirements
- `$VAR` - Expand environment variable
- `$?` - Expand last exit status
- `$$` - Expand shell PID (optional)
- Expansion in double quotes only
- No expansion in single quotes
- Empty string if variable doesn't exist

### Implementation

**Files to Create:**
- `src/expander/var_expansion.c` - Variable expansion
- `includes/expander.h` - Expansion prototypes

**Key Functions:**
```c
char    *expand_variables(char *str, t_exec_ctx *ctx);
char    *expand_single_var(char *str, int *i, t_exec_ctx *ctx);
char    *get_var_name(char *str, int *len);
char    *get_var_value(char *name, t_exec_ctx *ctx);
```

**Expansion Logic:**
```c
For each character in string:
1. If '$' found:
   a. If next is '?': replace with last_status
   b. If next is digit/letter: extract var name
   c. Look up value in environment
   d. Replace $VAR with value
2. Otherwise: keep character
```

**Special Cases:**
- `$?` → Exit status of last command
- `$` at end of string → literal '$'
- `$123` → Variable named "123" (or empty if not exists)
- `${VAR}` → Not required (optional enhancement)

**Integration Point:**
- During quote processing, expand in double-quoted strings
- In heredoc content
- In unquoted words

**Testing:**
```bash
minishell> export FOO=bar
minishell> echo $FOO              # bar
minishell> echo "$FOO"            # bar
minishell> echo '$FOO'            # $FOO
minishell> echo $FOO$FOO          # barbar
minishell> false; echo $?         # 1
minishell> true; echo $?          # 0
```

---

## Feature 5: Wildcards (*)

### Requirements
- `*` matches any sequence of characters
- Expand in current directory
- Sort results alphabetically
- No match → keep literal `*`
- Don't match hidden files (starting with .)

### Implementation

**Files to Create:**
- `src/expander/wildcard.c` - Wildcard expansion

**Key Functions:**
```c
char    **expand_wildcard(char *pattern);
int     match_pattern(char *str, char *pattern);
char    **list_matching_files(char *pattern);
int     compare_strings(const void *a, const void *b);
```

**Wildcard Matching Algorithm:**
```c
match_pattern(str, pattern):
1. If pattern is '*':
   - Try matching rest of pattern at each position in str
2. If characters match:
   - Advance both str and pattern
3. Otherwise:
   - No match
```

**Expansion Flow:**
```
1. Check if token contains '*'
2. If yes:
   a. Get current directory contents (readdir)
   b. Filter hidden files
   c. Match each file against pattern
   d. Sort matches alphabetically
   e. Replace token with matched files
3. If no matches: keep literal '*'
```

**Integration Point:**
- After parsing, expand wildcards in WORD tokens
- Before variable expansion
- Each match becomes separate argv element

**Testing:**
```bash
minishell> echo *                 # Lists all files
minishell> echo *.c               # Lists .c files
minishell> echo test*             # Lists files starting with test
minishell> echo *nomatch*         # *nomatch* (literal)
```

---

## Implementation Order

### Phase 1: Signal Handling
1. Create signal handler files
2. Integrate with main loop
3. Test interactive and execution modes

### Phase 2: Variable Expansion
1. Create expansion infrastructure
2. Implement $VAR expansion
3. Implement $? expansion
4. Test various scenarios

### Phase 3: Quote Processing
1. Implement quote removal
2. Integrate with variable expansion
3. Test mixed quotes

### Phase 4: Heredoc
1. Implement heredoc reading
2. Integrate with variable expansion
3. Test with pipes and redirections

### Phase 5: Wildcards
1. Implement pattern matching
2. Implement file listing and sorting
3. Test various patterns

---

## Testing Strategy

### Unit Tests
- Test each feature independently
- Test edge cases (empty strings, special chars)
- Test error handling

### Integration Tests
- Test feature combinations
- Compare with bash behavior
- Test complex command lines

### Regression Tests
- Ensure existing features still work
- Run all previous tests

---

## Norminette Compliance

Each new file must:
- Pass norminette with 0 errors
- Functions < 25 lines
- Max 5 variables per function
- Max 4 parameters per function
- Max 5 functions per file

If violations occur:
- Extract helper functions
- Split large functions
- Split large files

---

## Expected Deliverables

**New Files (approx. 10):**
- signals/ (2 files)
- expander/ (4 files)
- heredoc.c improvements
- expander.h, signals.h

**Modified Files (approx. 5):**
- main.c (signal init)
- exec_command.c (signal reset)
- parser files (quote/expansion integration)
- Makefile (new sources)

**Tests:**
- test_signals.sh
- test_expansion.sh
- test_heredoc.sh
- test_wildcards.sh
- Final integration test

**Total: ~15 new/modified files + tests**
