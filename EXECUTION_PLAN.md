# Execution Plan

## Overview

This document outlines the execution stage implementation for the minishell project. The executor takes the Abstract Syntax Tree (AST) produced by the parser and executes the commands according to shell semantics.

## Architecture

### Core Components

1. **Executor Core** (`src/executor/`)
   - Main execution dispatcher based on AST node type
   - Process management (fork, wait, exit status)
   - Error handling and cleanup

2. **Command Execution** (`src/executor/`)
   - External command execution via execve
   - PATH resolution
   - Built-in command dispatch

3. **Redirection Handler** (`src/executor/`)
   - File descriptor management
   - Input/output/append redirections
   - Heredoc processing

4. **Pipeline Handler** (`src/executor/`)
   - Pipe creation and management
   - Multiple command coordination
   - File descriptor passing

5. **Logical Operators** (`src/executor/`)
   - && (AND) execution with short-circuit
   - || (OR) execution with short-circuit
   - Exit status propagation

6. **Subshell Handler** (`src/executor/`)
   - Fork and execute in isolated context
   - Redirection support for subshells

7. **Built-in Commands** (`src/builtins/`)
   - echo (with -n flag)
   - cd (with relative/absolute paths)
   - pwd (no options)
   - export (variable=value format)
   - unset (remove variables)
   - env (display environment)
   - exit (with optional exit code)

8. **Environment Manager** (`src/env/`)
   - Environment variable storage
   - Variable lookup, set, unset operations
   - Export functionality

9. **Signal Handler** (`src/signals/`)
   - SIGINT (Ctrl+C) handling
   - SIGQUIT (Ctrl+\) handling
   - Signal behavior in interactive vs non-interactive mode

## Data Structures

### Execution Context
```c
typedef struct s_exec_ctx
{
	t_env		*env;           // Environment variables
	int			last_status;    // Last command exit status
	int			pipe_fds[2];    // Current pipe file descriptors
	int			stdin_backup;   // Original stdin for restoration
	int			stdout_backup;  // Original stdout for restoration
}	t_exec_ctx;
```

### Environment Variable
```c
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;
```

## Execution Flow

### 1. Simple Command
```
Input: "ls -la"
AST: NODE_COMMAND(argv=["ls", "-la"], redirects=NULL)

Execution:
1. Check if command is built-in
2. If built-in: execute_builtin()
3. If external:
   a. Fork process
   b. In child: resolve PATH, execve
   c. In parent: wait for child, capture exit status
4. Return exit status
```

### 2. Pipeline
```
Input: "cat file | grep pattern | wc -l"
AST: NODE_PIPE
       /        \
    NODE_PIPE  wc
    /      \
  cat     grep

Execution (right-to-left):
1. Create pipe between grep and wc
2. Fork wc process with stdin from pipe
3. Create pipe between cat and grep
4. Fork grep process with stdin from pipe[0], stdout to wc's pipe[1]
5. Fork cat process with stdout to grep's pipe[1]
6. Close all pipe fds in parent
7. Wait for all children
8. Return exit status of rightmost command
```

### 3. Logical AND
```
Input: "make && ./program"
AST: NODE_AND
      /       \
    make    program

Execution:
1. Execute left subtree (make)
2. If exit status == 0: execute right subtree (program)
3. If exit status != 0: skip right subtree
4. Return final exit status
```

### 4. Logical OR
```
Input: "test -f file || touch file"
AST: NODE_OR
      /         \
    test       touch

Execution:
1. Execute left subtree (test)
2. If exit status != 0: execute right subtree (touch)
3. If exit status == 0: skip right subtree
4. Return final exit status
```

### 5. Redirections
```
Input: "cat < input > output 2>&1"
Redirects: [REDIR_IN("input"), REDIR_OUT("output")]

Execution:
1. Save original stdin/stdout
2. For each redirect:
   - Open file with appropriate flags
   - dup2 to replace stdin/stdout/stderr
3. Execute command
4. Restore original file descriptors
5. Close opened files
```

### 6. Subshell
```
Input: "(cd /tmp && ls) > output"
AST: NODE_SUBSHELL
       child: NODE_AND(cd, ls)
       redirects: [REDIR_OUT("output")]

Execution:
1. Fork subshell process
2. In child:
   a. Apply redirections
   b. Execute child AST
   c. Exit with child's exit status
3. In parent: wait for subshell
4. Return subshell exit status
```

## Implementation Phases

### Phase 1: Foundation (Simple Commands)
**Files to Create:**
- `src/executor/execute.c` - Main execution dispatcher
- `src/executor/exec_command.c` - Simple command execution
- `src/executor/exec_utils.c` - Helper functions
- `src/executor/path_resolve.c` - PATH resolution
- `includes/executor.h` - Execution types and prototypes

**Functions:**
```c
int		execute_ast(t_ast_node *ast, t_exec_ctx *ctx);
int		exec_command(t_cmd_node *cmd, t_exec_ctx *ctx);
char	*resolve_path(char *command, t_env *env);
char	**env_to_envp(t_env *env);
void	free_envp(char **envp);
```

**Test Cases:**
- Execute `/bin/ls`
- Execute `ls` (PATH resolution)
- Execute with arguments `ls -la /tmp`
- Non-existent command error handling
- Permission denied handling

### Phase 2: Environment Management
**Files to Create:**
- `src/env/env_init.c` - Initialize from system environ
- `src/env/env_get.c` - Get variable value
- `src/env/env_set.c` - Set variable value
- `src/env/env_unset.c` - Remove variable
- `src/env/env_to_array.c` - Convert to char** for execve
- `includes/env.h` - Environment types and prototypes

**Functions:**
```c
t_env	*init_env(char **environ);
char	*env_get(t_env *env, char *key);
int		env_set(t_env **env, char *key, char *value);
int		env_unset(t_env **env, char *key);
void	free_env(t_env *env);
```

**Test Cases:**
- Initialize environment from system
- Get PATH variable
- Set new variable
- Unset variable
- Convert to envp array

### Phase 3: Built-in Commands
**Files to Create:**
- `src/builtins/builtin_dispatcher.c` - Check and dispatch built-ins
- `src/builtins/builtin_echo.c` - echo implementation
- `src/builtins/builtin_cd.c` - cd implementation
- `src/builtins/builtin_pwd.c` - pwd implementation
- `src/builtins/builtin_env.c` - env implementation
- `src/builtins/builtin_export.c` - export implementation
- `src/builtins/builtin_unset.c` - unset implementation
- `src/builtins/builtin_exit.c` - exit implementation
- `includes/builtins.h` - Built-in prototypes

**Functions:**
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

**Built-in Specifications:**
- **echo**: Print arguments with space separator, -n flag suppresses newline
- **cd**: Change directory, update PWD/OLDPWD, handle relative/absolute paths
- **pwd**: Print current working directory
- **env**: Print all environment variables (key=value format)
- **export**: Set environment variable (export VAR=value)
- **unset**: Remove environment variable
- **exit**: Exit shell with optional exit code (numeric)

**Test Cases:**
- `echo hello world` → "hello world\n"
- `echo -n hello` → "hello" (no newline)
- `cd /tmp && pwd` → "/tmp"
- `export FOO=bar && env | grep FOO` → "FOO=bar"
- `exit 42` → exit with status 42

### Phase 4: Redirections
**Files to Create:**
- `src/executor/exec_redirect.c` - Redirection setup and cleanup
- `src/executor/heredoc.c` - Heredoc processing

**Functions:**
```c
int		setup_redirects(t_redirect *redir, t_exec_ctx *ctx);
void	restore_redirects(t_exec_ctx *ctx);
int		handle_heredoc(t_redirect *redir);
int		open_redir_file(t_redirect *redir);
```

**Redirection Types:**
- `<` Input redirection (O_RDONLY)
- `>` Output redirection (O_WRONLY | O_CREAT | O_TRUNC, 0644)
- `>>` Append redirection (O_WRONLY | O_CREAT | O_APPEND, 0644)
- `<<` Heredoc (create temp file, read until delimiter)

**Test Cases:**
- `cat < input.txt`
- `echo hello > output.txt`
- `echo world >> output.txt`
- `cat << EOF` (heredoc)
- Multiple redirections: `cat < in > out`
- Redirection errors: Permission denied, file not found

### Phase 5: Pipes
**Files to Create:**
- `src/executor/exec_pipe.c` - Pipeline execution

**Functions:**
```c
int		exec_pipe(t_binop_node *pipe, t_exec_ctx *ctx);
int		fork_and_exec(t_ast_node *node, int *in_fd, int *out_fd, t_exec_ctx *ctx);
void	close_pipe(int fds[2]);
```

**Pipe Execution Strategy:**
```
For "cmd1 | cmd2 | cmd3":

1. Create pipe1 for cmd1 → cmd2
2. Fork cmd1: stdout → pipe1[1]
3. Create pipe2 for cmd2 → cmd3
4. Fork cmd2: stdin → pipe1[0], stdout → pipe2[1]
5. Fork cmd3: stdin → pipe2[0]
6. Close all pipe fds in parent
7. Wait for all children (in order)
8. Return exit status of last command
```

**Test Cases:**
- `echo hello | cat`
- `cat file | grep pattern`
- `ls | wc -l`
- Three-stage pipe: `cat file | grep a | wc -l`
- Pipe with redirections: `cat < in | grep pattern > out`

### Phase 6: Logical Operators
**Files to Create:**
- `src/executor/exec_logical.c` - AND/OR execution

**Functions:**
```c
int		exec_and(t_binop_node *and_node, t_exec_ctx *ctx);
int		exec_or(t_binop_node *or_node, t_exec_ctx *ctx);
```

**AND Operator (&&):**
- Execute left subtree
- If exit status == 0: execute right subtree
- If exit status != 0: skip right subtree, return left status
- Return final exit status

**OR Operator (||):**
- Execute left subtree
- If exit status != 0: execute right subtree
- If exit status == 0: skip right subtree, return left status
- Return final exit status

**Test Cases:**
- `true && echo success` → prints "success"
- `false && echo fail` → no output
- `false || echo fallback` → prints "fallback"
- `true || echo skip` → no output
- Complex: `false || true && echo yes` → prints "yes"

### Phase 7: Subshells
**Files to Create:**
- `src/executor/exec_subshell.c` - Subshell execution

**Functions:**
```c
int		exec_subshell(t_subshell_node *subshell, t_exec_ctx *ctx);
```

**Subshell Execution:**
1. Fork subshell process
2. In child process:
   - Apply subshell redirections
   - Execute child AST
   - Exit with child's exit status
3. In parent: wait and return status

**Subshell Properties:**
- Environment changes don't affect parent
- Exit in subshell doesn't exit parent shell
- Redirections can be applied to entire subshell

**Test Cases:**
- `(echo hello)` → "hello"
- `(cd /tmp && pwd)` → "/tmp" (doesn't change parent's pwd)
- `(echo hello) > output` → redirects subshell output
- `(exit 42)` → subshell exits but parent continues

### Phase 8: Signal Handling
**Files to Create:**
- `src/signals/signal_init.c` - Setup signal handlers
- `src/signals/signal_handlers.c` - Signal handler functions

**Functions:**
```c
void	init_signals(void);
void	ignore_signals(void);
void	default_signals(void);
void	sigint_handler(int sig);
```

**Signal Behavior:**

**Interactive Mode (waiting for input):**
- SIGINT (Ctrl+C): Display new prompt on new line
- SIGQUIT (Ctrl+\): Ignored

**During Command Execution:**
- SIGINT: Terminate foreground process, display newline
- SIGQUIT: Terminate foreground process, print "Quit: 3"

**Test Cases:**
- Press Ctrl+C at prompt → new prompt
- `sleep 10` then Ctrl+C → terminates, new prompt
- `cat` then Ctrl+C → terminates cat
- `cat` then Ctrl+\ → terminates with "Quit: 3"

### Phase 9: Main Shell Loop
**Files to Create:**
- `src/main.c` - Main shell loop
- `src/executor/exec_cleanup.c` - Cleanup functions

**Functions:**
```c
int		main(int argc, char **argv, char **environ);
void	shell_loop(t_exec_ctx *ctx);
void	cleanup_ctx(t_exec_ctx *ctx);
```

**Shell Loop:**
```c
while (1)
{
	1. Display prompt
	2. Read input with readline
	3. If NULL (EOF): break
	4. If empty: continue
	5. Add to history
	6. Tokenize input
	7. Validate syntax
	8. Parse tokens → AST
	9. Execute AST
	10. Free AST and tokens
}
```

## Error Handling

### Command Not Found
- Errno check after execve failure
- Print: "minishell: command: command not found"
- Exit status: 127

### Permission Denied
- Check file permissions before execve
- Print: "minishell: command: Permission denied"
- Exit status: 126

### Redirection Errors
- File open failure: "minishell: file: No such file or directory"
- Permission denied: "minishell: file: Permission denied"
- Exit status: 1

### Fork Failure
- Print: "minishell: fork: Resource temporarily unavailable"
- Exit status: 1

### Memory Allocation Failure
- Print error message
- Cleanup and exit

## Exit Status Rules

- Last executed command's exit status stored in `$?`
- 0: Success
- 1-255: Command-specific error codes
- 126: Command found but not executable
- 127: Command not found
- 128+N: Command terminated by signal N (e.g., 130 for SIGINT)

## Testing Strategy

### Unit Tests
- Test each execution type independently
- Mock environment and file system where possible
- Verify exit status propagation

### Integration Tests
1. Simple commands: `ls`, `echo`, `cat`
2. Commands with arguments: `ls -la`, `echo hello world`
3. PATH resolution: `ls` vs `/bin/ls`
4. Built-ins: All 7 built-in commands
5. Redirections: All 4 types, multiple redirections
6. Pipes: 2-stage, 3-stage, with redirections
7. Logical operators: &&, ||, combinations
8. Subshells: Simple, with redirections, nested
9. Complex combinations: `(cmd1 | cmd2) && cmd3 || cmd4 > out`
10. Error cases: Invalid commands, permission errors, etc.

### Comparison with Bash
- Run same input in both shells
- Compare output and exit status
- Verify behavior matches bash for:
  - Command execution
  - Operator precedence
  - Redirection order
  - Signal handling
  - Exit status

## 42 Norm Compliance Considerations

### Function Length
- Keep functions under 25 lines
- Extract helper functions for complex logic
- Use function pointers for dispatch tables if needed

### Variable Count
- Maximum 5 variables per function
- Use structures to group related data
- Pass context structures instead of many parameters

### Parameter Count
- Maximum 4 parameters per function
- Use context structures for execution state

### Forbidden Constructs
- No for loops (use while)
- No ternary operators
- No switch statements (use if-else or function pointers)

## Memory Management

### Allocation Points
1. Environment variables (linked list nodes)
2. envp array (for execve)
3. PATH resolution (string allocation)
4. File descriptor backups
5. Pipe file descriptors

### Cleanup Requirements
- Free all allocated memory on exit
- Close all file descriptors
- Restore terminal state
- Clean up readline history

### Leak Prevention
- Free on normal execution path
- Free on error paths
- Use valgrind for verification
- Implement cleanup_ctx function

## Dependencies

### Allowed Functions
- readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history
- printf, malloc, free, write, access, open, read, close
- fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill
- exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe
- opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl
- getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

### Libft Functions
- All previously implemented libft functions available
- Focus on: ft_split, ft_strjoin, ft_strlen, ft_strcmp, ft_strdup, etc.

## Next Steps

After completing this plan:
1. Review with user for any adjustments
2. Begin implementation in phases
3. Test each phase before moving to next
4. Run norminette after each file
5. Integration test after each phase
6. Final comprehensive testing

## Notes

- Built-ins must execute in the parent process (no fork) to affect shell state
- External commands must fork to preserve shell state
- Pipe commands must all fork simultaneously to avoid deadlock
- File descriptors must be carefully managed to prevent leaks
- Exit status must be properly propagated through all execution paths
