# Minishell Comprehensive Test Suite

## Overview
This test suite provides comprehensive testing for the minishell implementation, comparing behavior against bash.

## Test Categories

### 1. Basic Commands (`test_basic.sh`)
- Simple commands (echo, pwd, env)
- Built-in commands (export, unset, cd)
- External commands
- Empty/whitespace input

### 2. Redirections (`test_redirects.sh`)
- Output redirection (>)
- Input redirection (<)
- Append (>>)
- Multiple redirects
- Redirect order and precedence
- Error cases

### 3. Variable Expansion (`test_expansion.sh`)
- Simple variable expansion ($VAR)
- Exit status expansion ($?)
- Expansion in quotes
- Undefined variables
- Empty variables
- Nested expansion scenarios
- Variable in commands and redirects

### 4. Heredoc (`test_heredoc.sh`)
- Simple heredocs (<<)
- Variable expansion in heredocs
- Empty heredocs
- Heredocs with pipes
- Multiple line heredocs

### 5. Wildcards (`test_wildcards.sh`)
- Star wildcard (*)
- Wildcard by extension (*.txt)
- Wildcard by prefix/suffix
- No match scenarios (literal)
- Hidden file exclusion
- Wildcards in subdirectories

### 6. Quote Processing (`test_quotes.sh`)
- Double quotes
- Single quotes
- Mixed quotes
- Empty quotes
- Quote concatenation
- Expansion in quotes
- Special characters in quotes

### 7. Pipes (`test_pipes.sh`)
- Simple pipes
- Multiple pipes
- Pipes with redirects
- Empty pipes
- Complex pipelines

### 8. Logical Operators (`test_logical.sh`)
- AND operator (&&)
- OR operator (||)
- Chained logical operators
- Mixed AND/OR
- Precedence testing

### 9. Exit Status (`test_exit_status.sh`)
- Exit status of built-ins
- Exit status of external commands
- Exit status with pipes
- Exit status with redirects
- Exit status with logical operators
- Exit status persistence

### 10. Edge Cases (`test_edge_cases.sh`)
- Complex redirect combinations
- Variable expands to variable name
- Ambiguous redirects
- Wildcard in quotes
- Heredoc in pipes
- All features combined
- Long command lines
- Special character handling

### 11. Signals (`test_signals_manual.sh`)
**Note: These tests must be run MANUALLY**
- SIGINT (Ctrl+C) in interactive mode
- SIGINT during command execution
- SIGQUIT (Ctrl+\) handling
- Exit status after signals
- Signals in heredocs
- Signals in pipelines

## Running the Tests

### Automated Tests
```bash
# Make test runner executable
chmod +x tests/test_runner.sh

# Run all automated tests
./tests/test_runner.sh
```

### Manual Signal Tests
```bash
# Read the signal testing guide
cat tests/test_signals_manual.sh

# Or run it to display the guide
bash tests/test_signals_manual.sh
```

## Test Output

The test runner will:
1. Create a temporary test environment
2. Run each test in both bash and minishell
3. Compare outputs and exit statuses
4. Display colored results:
   - 🟢 **GREEN**: Test passed
   - 🔴 **RED**: Test failed (with diff)
   - 🟡 **YELLOW**: Test skipped

## Expected Results

All automated tests should pass if minishell correctly implements:
- ✅ Lexer and parser
- ✅ Command execution
- ✅ Built-in commands
- ✅ Redirections (>, <, >>)
- ✅ Heredocs (<<)
- ✅ Pipes (|)
- ✅ Logical operators (&&, ||)
- ✅ Variable expansion ($VAR, $?)
- ✅ Quote processing (", ')
- ✅ Wildcard expansion (*)

## Troubleshooting

### Test Failures
If tests fail, check:
1. **Output difference**: Compare expected vs actual output
2. **Exit status**: Ensure exit codes match bash
3. **Parsing**: Verify command is parsed correctly
4. **Execution order**: Check operation sequence

### Common Issues

**Wildcards not expanding:**
- Check if wildcard.c is compiled
- Verify expand_argv_wildcards() is called

**Variable not expanding:**
- Check quote context (single vs double)
- Verify expand_variables() logic

**Redirects failing:**
- Check file permissions
- Verify redirect order (last wins)

**Heredoc not working:**
- Check delimiter matching
- Verify pipe creation

**Exit status incorrect:**
- Check status propagation
- Verify $? expansion timing

## Adding New Tests

To add tests to a category:

1. Open the relevant test file (e.g., `test_expansion.sh`)
2. Add a new test using the `run_test` function:

```bash
run_test "test_name" \
    "command to test" \
    "Description of test"
```

3. Run the test suite to verify

## Test Statistics

Total Tests: **~150+ automated tests**
- Basic: 15 tests
- Redirects: 14 tests
- Expansion: 18 tests
- Heredoc: 12 tests
- Wildcards: 14 tests
- Quotes: 18 tests
- Pipes: 14 tests
- Logical: 14 tests
- Exit Status: 18 tests
- Edge Cases: 38 tests
- Signals: 10 manual tests

## Notes

- Tests assume bash 3.2+ behavior
- Some tests may differ on macOS vs Linux
- Signal tests must be verified manually
- Tests clean up temporary files automatically
- Each test runs in isolated environment

## Reporting Issues

If you find a test that fails incorrectly:
1. Verify the test against actual bash
2. Check if it's a platform difference
3. Update the test if needed
4. Document any known differences
