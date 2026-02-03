# Execution Implementation Roadmap

## Current Status
✅ Phase 1-2: Tokenizer (Lexer) - Complete
✅ Phase 3-4: Parser - Complete
🔄 Phase 5-13: Executor - **Next**

## Phase 5: Execution Foundation

### 5.1 Header Setup
**File:** `includes/executor.h`

**Structures:**
```c
typedef struct s_exec_ctx
{
	t_env		*env;
	int			last_status;
	int			pipe_fds[2];
	int			stdin_backup;
	int			stdout_backup;
}	t_exec_ctx;
```

**Function Prototypes:**
```c
// Main execution
int		execute_ast(t_ast_node *ast, t_exec_ctx *ctx);
int		exec_command(t_cmd_node *cmd, t_exec_ctx *ctx);

// Utilities
char	*resolve_path(char *command, t_env *env);
char	**env_to_envp(t_env *env);
void	free_envp(char **envp);
t_exec_ctx	*init_exec_ctx(char **environ);
void	cleanup_exec_ctx(t_exec_ctx *ctx);
```

### 5.2 Execution Dispatcher
**File:** `src/executor/execute.c`

**Functions:**
- `execute_ast()` - Main dispatcher, routes based on node type
- Returns exit status (0-255)

**Logic:**
```c
int	execute_ast(t_ast_node *ast, t_exec_ctx *ctx)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_COMMAND)
		return (exec_command(&ast->data.cmd, ctx));
	else if (ast->type == NODE_PIPE)
		return (exec_pipe(&ast->data.binop, ctx));
	else if (ast->type == NODE_AND)
		return (exec_and(&ast->data.binop, ctx));
	else if (ast->type == NODE_OR)
		return (exec_or(&ast->data.binop, ctx));
	else if (ast->type == NODE_SUBSHELL)
		return (exec_subshell(&ast->data.subshell, ctx));
	return (1);
}
```

### 5.3 Simple Command Execution
**File:** `src/executor/exec_command.c`

**Functions:**
- `exec_command()` - Check built-in vs external, fork if needed
- `exec_external()` - Fork and execve
- `wait_child()` - Wait and return exit status

**Logic:**
```c
1. Check if built-in
   - If yes: exec_builtin() and return status
2. Fork process
3. In child:
   - Resolve command path
   - Convert env to envp
   - execve(path, argv, envp)
   - If execve fails: print error and exit(127)
4. In parent:
   - Wait for child
   - Return WEXITSTATUS
```

### 5.4 PATH Resolution
**File:** `src/executor/path_resolve.c`

**Functions:**
- `resolve_path()` - Find executable in PATH
- `is_executable()` - Check access and permissions
- `try_direct_path()` - Handle absolute/relative paths

**Logic:**
```c
1. If command contains '/': try direct path
2. Get PATH from environment
3. Split PATH by ':'
4. For each directory:
   - Join dir + "/" + command
   - Check if executable with access()
   - If yes: return full path
5. Return NULL if not found
```

### 5.5 Environment Utilities
**File:** `src/executor/exec_utils.c`

**Functions:**
- `env_to_envp()` - Convert linked list to char** array
- `free_envp()` - Free char** array
- `init_exec_ctx()` - Initialize execution context
- `cleanup_exec_ctx()` - Free execution context

**Test:** `tests/test_exec_simple.c`
```c
Test cases:
1. Execute /bin/echo hello
2. Execute ls (PATH resolution)
3. Execute ls -la /tmp (with arguments)
4. Execute nonexistent → error 127
5. Execute /tmp/noexec → error 126
```

**Completion Criteria:**
- ✅ Simple external commands work
- ✅ PATH resolution works
- ✅ Exit status correctly returned
- ✅ Errors handled (command not found, permission denied)
- ✅ No memory leaks (valgrind clean)
- ✅ All files pass norminette

---

## Phase 6: Environment Management

### 6.1 Environment Header
**File:** `includes/env.h`

**Structures:**
```c
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;
```

**Function Prototypes:**
```c
t_env	*init_env(char **environ);
char	*env_get(t_env *env, char *key);
int		env_set(t_env **env, char *key, char *value);
int		env_unset(t_env **env, char *key);
void	free_env(t_env *env);
t_env	*env_node_new(char *key, char *value);
t_env	*env_find(t_env *env, char *key);
```

### 6.2 Environment Initialization
**File:** `src/env/env_init.c`

**Functions:**
- `init_env()` - Parse environ and build linked list
- `env_node_new()` - Create new node
- `parse_env_line()` - Split "KEY=VALUE" into key and value

**Logic:**
```c
For each string in environ:
1. Find '=' position
2. Extract key (before '=')
3. Extract value (after '=')
4. Create new node
5. Add to linked list
```

### 6.3 Environment Get
**File:** `src/env/env_get.c`

**Functions:**
- `env_get()` - Find and return value by key
- `env_find()` - Find node by key (helper)

### 6.4 Environment Set
**File:** `src/env/env_set.c`

**Functions:**
- `env_set()` - Update existing or create new variable
- `env_update()` - Update existing node's value
- `env_append()` - Append new node to list

### 6.5 Environment Unset
**File:** `src/env/env_unset.c`

**Functions:**
- `env_unset()` - Remove variable from list
- `env_remove_node()` - Unlink and free node

### 6.6 Environment Conversion
**File:** `src/env/env_to_array.c`

**Functions:**
- `env_to_envp()` - Convert to char** for execve
- `env_count()` - Count variables
- `env_to_string()` - Convert node to "KEY=VALUE" string

**Test:** `tests/test_env.c`
```c
Test cases:
1. Initialize from environ
2. Get existing variable (PATH)
3. Get non-existent variable → NULL
4. Set new variable
5. Update existing variable
6. Unset variable
7. Convert to envp array
8. Memory leak check
```

**Completion Criteria:**
- ✅ Environment initialized from system
- ✅ Get/set/unset operations work
- ✅ Conversion to envp works
- ✅ No memory leaks
- ✅ All files pass norminette

---

## Phase 7: Built-in Commands

### 7.1 Built-in Header
**File:** `includes/builtins.h`

**Function Prototypes:**
```c
int		is_builtin(char *command);
int		exec_builtin(t_cmd_node *cmd, t_exec_ctx *ctx);
int		builtin_echo(char **argv, t_exec_ctx *ctx);
int		builtin_cd(char **argv, t_exec_ctx *ctx);
int		builtin_pwd(char **argv, t_exec_ctx *ctx);
int		builtin_env(char **argv, t_exec_ctx *ctx);
int		builtin_export(char **argv, t_exec_ctx *ctx);
int		builtin_unset(char **argv, t_exec_ctx *ctx);
int		builtin_exit(char **argv, t_exec_ctx *ctx);
```

### 7.2 Built-in Dispatcher
**File:** `src/builtins/builtin_dispatcher.c`

**Functions:**
- `is_builtin()` - Check if command is built-in
- `exec_builtin()` - Dispatch to appropriate built-in

**Logic:**
```c
if (!ft_strcmp(cmd->argv[0], "echo"))
	return (builtin_echo(cmd->argv, ctx));
else if (!ft_strcmp(cmd->argv[0], "cd"))
	return (builtin_cd(cmd->argv, ctx));
// ... etc
```

### 7.3 Echo Built-in
**File:** `src/builtins/builtin_echo.c`

**Behavior:**
- Print all arguments separated by space
- -n flag: suppress trailing newline
- Multiple -n flags: only first is significant

**Test:**
```bash
echo hello world    → "hello world\n"
echo -n hello       → "hello"
echo -n -n hello    → "hello"
```

### 7.4 CD Built-in
**File:** `src/builtins/builtin_cd.c`

**Behavior:**
- Change directory with chdir()
- Update PWD and OLDPWD environment variables
- Handle: relative paths, absolute paths, . and ..
- cd with no args: go to HOME
- cd -: go to OLDPWD

**Test:**
```bash
cd /tmp && pwd           → "/tmp"
cd .. && pwd             → "/"
cd && pwd                → "/Users/username"
cd - && pwd              → "/tmp"
cd /nonexistent          → error message
```

### 7.5 PWD Built-in
**File:** `src/builtins/builtin_pwd.c`

**Behavior:**
- Print current working directory using getcwd()
- No options supported

**Test:**
```bash
pwd    → "/current/directory"
```

### 7.6 Env Built-in
**File:** `src/builtins/builtin_env.c`

**Behavior:**
- Print all environment variables
- Format: KEY=VALUE (one per line)
- No options supported

**Test:**
```bash
env    → prints all variables
```

### 7.7 Export Built-in
**File:** `src/builtins/builtin_export.c`

**Behavior:**
- export VAR=value: set environment variable
- export without args: print all variables (like env)
- Multiple variables: export VAR1=a VAR2=b

**Test:**
```bash
export FOO=bar
env | grep FOO           → "FOO=bar"
export BAR=baz QUX=42
env | grep BAR           → "BAR=baz"
```

### 7.8 Unset Built-in
**File:** `src/builtins/builtin_unset.c`

**Behavior:**
- Remove environment variable
- Multiple variables: unset VAR1 VAR2
- No error if variable doesn't exist

**Test:**
```bash
export FOO=bar
unset FOO
env | grep FOO           → (no output)
```

### 7.9 Exit Built-in
**File:** `src/builtins/builtin_exit.c`

**Behavior:**
- exit: exit with last command's status
- exit N: exit with status N (0-255)
- exit with non-numeric: error and exit 2
- Only first argument considered

**Test:**
```bash
exit       → exit(last_status)
exit 42    → exit(42)
exit abc   → error "numeric argument required"
```

**Test:** `tests/test_builtins.c`
```c
Test each built-in individually
Test with various argument combinations
Test error cases
Test environment changes persist
```

**Completion Criteria:**
- ✅ All 7 built-ins implemented
- ✅ Built-ins execute without fork
- ✅ Environment changes persist
- ✅ All test cases pass
- ✅ No memory leaks
- ✅ All files pass norminette

---

## Phase 8: Redirections

### 8.1 Redirection Functions
**File:** `src/executor/exec_redirect.c`

**Functions:**
- `setup_redirects()` - Apply all redirections
- `restore_redirects()` - Restore original fds
- `open_redir_file()` - Open file with correct flags
- `apply_single_redir()` - Apply one redirection

**Logic:**
```c
setup_redirects():
1. Save stdin/stdout to ctx->stdin_backup, ctx->stdout_backup
2. For each redirect in list:
   - Open file
   - dup2 to appropriate fd
   - Close opened file
3. Return 0 on success, -1 on error

restore_redirects():
1. dup2(ctx->stdin_backup, STDIN_FILENO)
2. dup2(ctx->stdout_backup, STDOUT_FILENO)
3. close(ctx->stdin_backup)
4. close(ctx->stdout_backup)
```

**File Open Flags:**
- REDIR_IN: O_RDONLY
- REDIR_OUT: O_WRONLY | O_CREAT | O_TRUNC, mode 0644
- REDIR_APPEND: O_WRONLY | O_CREAT | O_APPEND, mode 0644
- REDIR_HEREDOC: special handling (next section)

### 8.2 Heredoc Processing
**File:** `src/executor/heredoc.c`

**Functions:**
- `handle_heredoc()` - Process heredoc, return fd
- `read_heredoc_lines()` - Read lines until delimiter
- `create_heredoc_fd()` - Create temp file or pipe

**Logic:**
```c
1. Create pipe for heredoc content
2. Read lines with readline until delimiter found
3. Write lines to pipe write end
4. Close write end
5. Return read end fd
6. In setup_redirects: dup2 heredoc fd to stdin
```

**Note:** Heredoc should be processed during parsing or before execution begins, not during command execution.

### 8.3 Update exec_command
**File:** `src/executor/exec_command.c`

**Modifications:**
```c
exec_command():
1. Setup redirections
2. Execute command (built-in or external)
3. Restore redirections
4. Return exit status
```

**Test:** `tests/test_redirect.c`
```c
Test cases:
1. cat < input.txt
2. echo hello > output.txt
3. echo world >> output.txt
4. cat << EOF (heredoc)
5. Multiple: cat < in > out
6. Error: cat < nonexistent
7. Error: echo > /nowrite/file
```

**Completion Criteria:**
- ✅ All 4 redirection types work
- ✅ Multiple redirections work
- ✅ Redirections with built-ins work
- ✅ Redirections with external commands work
- ✅ Error handling works
- ✅ File descriptors properly closed
- ✅ No memory leaks
- ✅ All files pass norminette

---

## Phase 9: Pipes

### 9.1 Pipe Execution
**File:** `src/executor/exec_pipe.c`

**Functions:**
- `exec_pipe()` - Execute pipeline
- `setup_pipe_child()` - Setup child fds for pipe
- `wait_pipeline()` - Wait for all children
- `close_pipe_fds()` - Close pipe in parent

**Pipeline Strategy:**
```
For "cmd1 | cmd2 | cmd3":

Execution:
1. Flatten pipeline to array of commands (left-to-right)
2. For each command except last:
   - Create pipe
   - Fork child
   - In child: setup stdin from prev pipe, stdout to new pipe, execute
3. For last command:
   - Fork child
   - In child: setup stdin from prev pipe, execute
4. In parent: close all pipes
5. Wait for all children
6. Return exit status of last command
```

**Detailed Logic:**
```c
exec_pipe(binop_node):
1. Collect all pipe commands in array (handle left-deep tree)
2. For i = 0 to n-1:
   a. If not last: pipe(new_fds)
   b. Fork child
   c. In child:
      - If not first: dup2(prev_pipe_read, STDIN)
      - If not last: dup2(new_pipe_write, STDOUT)
      - Close all pipes
      - Execute command
      - Exit
   d. In parent:
      - If not first: close(prev_pipe_read)
      - If not last: close(new_pipe_write), save new_pipe_read as prev
3. Wait for all children
4. Return last child's exit status
```

### 9.2 Pipeline Flattening
**File:** `src/executor/pipe_utils.c`

**Functions:**
- `flatten_pipeline()` - Convert tree to array of nodes
- `count_pipes()` - Count pipeline depth
- `free_pipeline()` - Free array

**Test:** `tests/test_pipe.c`
```c
Test cases:
1. echo hello | cat
2. cat file | grep pattern
3. ls | wc -l
4. cat file | grep a | wc -l (3-stage)
5. echo a | cat | cat | cat (4-stage)
6. With redirections: cat < in | grep x > out
7. Built-ins in pipe: export FOO=bar | cat
```

**Completion Criteria:**
- ✅ 2-stage pipes work
- ✅ Multi-stage pipes work
- ✅ Pipes with redirections work
- ✅ Pipes with built-ins work
- ✅ Exit status from last command
- ✅ No file descriptor leaks
- ✅ No memory leaks
- ✅ All files pass norminette

---

## Phase 10: Logical Operators

### 10.1 AND Operator
**File:** `src/executor/exec_logical.c`

**Function:** `exec_and()`

**Logic:**
```c
exec_and(binop_node):
1. status = execute_ast(left, ctx)
2. ctx->last_status = status
3. if (status == 0):
      status = execute_ast(right, ctx)
4. return status
```

### 10.2 OR Operator
**File:** `src/executor/exec_logical.c`

**Function:** `exec_or()`

**Logic:**
```c
exec_or(binop_node):
1. status = execute_ast(left, ctx)
2. ctx->last_status = status
3. if (status != 0):
      status = execute_ast(right, ctx)
4. return status
```

**Test:** `tests/test_logical.c`
```c
Test cases:
1. true && echo yes        → prints "yes"
2. false && echo no        → no output
3. false || echo yes       → prints "yes"
4. true || echo no         → no output
5. false || false || true  → short-circuits correctly
6. true && false && echo   → short-circuits correctly
7. Complex: (false || true) && echo yes  → "yes"
```

**Completion Criteria:**
- ✅ AND short-circuits correctly
- ✅ OR short-circuits correctly
- ✅ Exit status propagates correctly
- ✅ Combinations work correctly
- ✅ All test cases pass
- ✅ All files pass norminette

---

## Phase 11: Subshells

### 11.1 Subshell Execution
**File:** `src/executor/exec_subshell.c`

**Function:** `exec_subshell()`

**Logic:**
```c
exec_subshell(subshell_node):
1. Fork process
2. In child:
   - Setup redirections for subshell
   - Execute child AST
   - Exit with child's exit status
3. In parent:
   - Wait for child
   - Return child's exit status
```

**Subshell Properties:**
- Environment changes don't affect parent
- cd doesn't affect parent
- exit exits subshell only, not parent
- Redirections apply to entire subshell

**Test:** `tests/test_subshell.c`
```c
Test cases:
1. (echo hello)              → "hello"
2. (cd /tmp && pwd)          → "/tmp", parent still in original dir
3. (export FOO=bar && env)   → shows FOO, parent doesn't have FOO
4. (echo hello) > out        → redirect subshell output
5. (exit 42)                 → subshell exits, parent continues
6. ((echo nested))           → nested subshells
```

**Completion Criteria:**
- ✅ Subshells execute in child process
- ✅ Environment isolation works
- ✅ Redirections work
- ✅ Exit doesn't kill parent shell
- ✅ All test cases pass
- ✅ All files pass norminette

---

## Phase 12: Signal Handling

### 12.1 Signal Setup
**File:** `src/signals/signal_init.c`

**Functions:**
- `init_signals()` - Setup signal handlers
- `ignore_signals()` - Ignore signals (during child execution)
- `default_signals()` - Restore default handlers

**Signal Handlers:**
- SIGINT: Interactive new prompt / during command terminate
- SIGQUIT: Ignore interactive / during command terminate with message

### 12.2 Signal Handlers
**File:** `src/signals/signal_handlers.c`

**Functions:**
- `sigint_handler()` - Handle Ctrl+C
- `sigquit_handler()` - Handle Ctrl+\

**Behavior:**

**In Interactive Mode (waiting for input):**
- SIGINT: Write "\n" and display new prompt (using rl_on_new_line, rl_replace_line, rl_redisplay)
- SIGQUIT: Ignore

**During Command Execution:**
- SIGINT: Default behavior (terminates child), parent displays "\n"
- SIGQUIT: Default behavior (terminates child), parent displays "Quit: 3\n"

**Test:** Manual testing
```bash
1. [prompt] Ctrl+C → new prompt
2. sleep 10 [Ctrl+C] → terminates, new prompt
3. cat [Ctrl+C] → terminates
4. cat [Ctrl+\] → "Quit: 3"
5. Verify no weird behavior after signals
```

**Completion Criteria:**
- ✅ SIGINT works in interactive mode
- ✅ SIGINT works during command execution
- ✅ SIGQUIT ignored in interactive mode
- ✅ SIGQUIT works during command execution
- ✅ All files pass norminette

---

## Phase 13: Main Shell Loop

### 13.1 Main Entry Point
**File:** `src/main.c`

**Functions:**
- `main()` - Initialize and start shell loop
- `shell_loop()` - Main REPL loop
- `process_line()` - Process single input line

**Shell Loop:**
```c
while (1)
{
	1. line = readline("minishell> ")
	2. if (!line) break  // EOF (Ctrl+D)
	3. if (line[0] == '\0') continue  // Empty line
	4. add_history(line)
	5. tokens = tokenize(line)
	6. if (!tokens) continue
	7. if (!validate_syntax(tokens)) { free_tokens; continue }
	8. ast = parse_tokens(tokens)
	9. if (!ast) { free_tokens; continue }
	10. status = execute_ast(ast, ctx)
	11. ctx->last_status = status
	12. free_ast(ast)
	13. free_tokens(tokens)
	14. free(line)
}
```

### 13.2 Cleanup
**File:** `src/executor/exec_cleanup.c`

**Functions:**
- `cleanup_exec_ctx()` - Free execution context
- `cleanup_all()` - Full cleanup before exit

**Cleanup on Exit:**
1. Free AST if allocated
2. Free tokens if allocated
3. Free environment
4. Clear readline history
5. Close any open file descriptors
6. Free execution context

**Test:** Full integration testing
```bash
Run comprehensive test suite:
1. All previous test cases
2. Memory leak check (valgrind)
3. Multiple sequential commands
4. Error recovery
5. Complex nested structures
6. Signal handling during execution
```

**Completion Criteria:**
- ✅ Shell loop works
- ✅ Multiple commands can be executed
- ✅ Error recovery works
- ✅ Clean exit on EOF
- ✅ No memory leaks (valgrind)
- ✅ All files pass norminette

---

## Final Integration Testing

### Test Categories

1. **Basic Commands**
   - Simple external commands
   - Built-in commands
   - Commands with various arguments

2. **PATH Resolution**
   - Absolute paths
   - Relative paths
   - Commands in PATH
   - Non-existent commands

3. **Redirections**
   - All 4 types individually
   - Multiple redirections
   - Redirection with pipes
   - Error cases

4. **Pipes**
   - 2-stage pipes
   - Multi-stage pipes
   - Pipes with redirections
   - Pipes with built-ins

5. **Logical Operators**
   - AND operator
   - OR operator
   - Combinations
   - With other features

6. **Subshells**
   - Simple subshells
   - Subshells with redirections
   - Nested subshells
   - Subshells with pipes

7. **Complex Combinations**
   - `(cmd1 | cmd2) && cmd3 > out`
   - `cmd1 < in | cmd2 | cmd3 > out`
   - `(cd /tmp && ls) || echo fail`
   - Multiple levels of nesting

8. **Error Handling**
   - Command not found
   - Permission denied
   - File not found (redirections)
   - Syntax errors
   - Memory allocation failures

9. **Signal Handling**
   - Ctrl+C in various contexts
   - Ctrl+\ in various contexts
   - Signal during long-running commands

10. **Memory Management**
    - Valgrind clean (no leaks, no errors)
    - Stress test with many commands
    - Large command lines

### Comparison with Bash

For each test case, verify:
1. Output matches bash
2. Exit status matches bash
3. File contents match bash (for redirections)
4. Behavior matches bash (for signals, errors)

### Performance Testing

- Measure execution time for common operations
- Ensure no significant performance degradation
- Profile for bottlenecks if necessary

---

## Makefile Updates

### Update Source Files
```makefile
# Executor sources
EXECUTOR_DIR = src/executor
EXECUTOR_FILES = execute.c \
                 exec_command.c \
                 exec_redirect.c \
                 exec_pipe.c \
                 exec_logical.c \
                 exec_subshell.c \
                 exec_utils.c \
                 exec_cleanup.c \
                 path_resolve.c \
                 heredoc.c \
                 pipe_utils.c

# Environment sources
ENV_DIR = src/env
ENV_FILES = env_init.c \
            env_get.c \
            env_set.c \
            env_unset.c \
            env_to_array.c

# Built-in sources
BUILTINS_DIR = src/builtins
BUILTINS_FILES = builtin_dispatcher.c \
                 builtin_echo.c \
                 builtin_cd.c \
                 builtin_pwd.c \
                 builtin_env.c \
                 builtin_export.c \
                 builtin_unset.c \
                 builtin_exit.c

# Signal sources
SIGNALS_DIR = src/signals
SIGNALS_FILES = signal_init.c \
                signal_handlers.c

# Main source
MAIN_SRC = src/main.c
```

### Final Binary
```makefile
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) -lreadline
```

---

## Progress Tracking

### Phase Checklist

- [ ] Phase 5: Execution Foundation
- [ ] Phase 6: Environment Management
- [ ] Phase 7: Built-in Commands
- [ ] Phase 8: Redirections
- [ ] Phase 9: Pipes
- [ ] Phase 10: Logical Operators
- [ ] Phase 11: Subshells
- [ ] Phase 12: Signal Handling
- [ ] Phase 13: Main Shell Loop
- [ ] Final Integration Testing
- [ ] Norminette Compliance
- [ ] Memory Leak Testing
- [ ] Comparison with Bash

### Estimated File Count

**New Files:**
- Headers: 3 (executor.h, env.h, builtins.h)
- Executor: 11 files
- Environment: 5 files
- Built-ins: 8 files
- Signals: 2 files
- Main: 1 file
- Tests: 8 test files

**Total: ~38 new files**

---

## Ready to Begin

Once you're ready to start implementation:
1. Confirm the plan looks good
2. We'll begin with Phase 5 (Execution Foundation)
3. Create headers first
4. Implement functions one phase at a time
5. Test after each phase
6. Run norminette after each file
7. Progress through all phases systematically

The execution stage is the most complex part but we have a clear roadmap!
