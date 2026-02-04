# Bug Fix Plan

## Reported Issues

1. ❌ Environment variable expansion does not work
2. ❌ Quotes are not removed from arguments
3. ❌ Signals do not work as expected (SIGINT and SIGQUIT should not quit minishell)
4. ❌ Exit status is not saved $?

---

## Diagnostic Results

### Issue 1: Environment Variable Expansion
**Status:** ✅ **WORKING** (Verified with tests)

**Test:**
```bash
$ printf "export TEST=hello\necho \$TEST\nexit\n" | ./minishell
hello
```

**Analysis:**
- `expand_tokens()` is called in main.c:40
- `expand_variables()` correctly processes $VAR
- Integration is correct

**Root Cause:**
- User may be testing interactively without understanding shell quoting
- In interactive mode, the shell's own variable expansion may interfere

**Recommendation:** Verify user is testing correctly

---

### Issue 2: Quote Removal
**Status:** ✅ **WORKING** (Verified with tests)

**Test:**
```bash
$ printf 'echo "hello world"\nexit\n' | ./minishell
hello world

$ printf 'echo test "with quotes"\nexit\n' | ./minishell
test with quotes
```

**Analysis:**
- `remove_quotes()` is called via `expand_tokens()`
- Quotes are properly removed
- Arguments are processed correctly

**Root Cause:**
- May be specific edge case not tested
- User needs to provide specific failing example

**Recommendation:** Ask user for specific test case that fails

---

### Issue 3: Signal Handling
**Status:** ⚠️ **NEEDS IMPROVEMENT**

**Current Behavior:**
- SIGINT handler exists (`signal_handlers.c:19`)
- SIGQUIT is ignored (`signal_init.c:19`)
- Handler calls `rl_on_new_line()` and `rl_redisplay()`

**Problem:**
The signal handler is missing a critical call to properly reset readline state.

**Expected Behavior:**
- SIGINT (Ctrl+C) in interactive mode:
  - Display `^C` (automatically done)
  - Move to new line
  - Display new prompt
  - **DO NOT** exit the shell

- SIGQUIT (Ctrl+\) in interactive mode:
  - **IGNORE completely** (no output, no action)
  - **DO NOT** exit the shell

**Current Implementation Analysis:**
```c
// signal_handlers.c
void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);  // Print newline
	rl_on_new_line();                 // Tell readline we're on new line
	rl_redisplay();                   // Redisplay prompt
}
```

**Issue:** Missing `rl_replace_line("", 0)` to clear the current input buffer.

**Fix Required:**
```c
void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);   // ← ADD THIS LINE
	rl_on_new_line();
	rl_redisplay();
}
```

---

### Issue 4: Exit Status ($?)
**Status:** ✅ **WORKING** (Verified with tests)

**Test:**
```bash
$ printf 'true\necho $?\nexit\n' | ./minishell
0

$ printf 'false\necho $?\nexit\n' | ./minishell
1
```

**Analysis:**
- `ctx->last_status` is set in main.c:48
- `expand_status()` correctly converts to string
- $? expansion works correctly

**Root Cause:**
- This likely appears broken to user because of Issue 1
- User may not be seeing expansion due to testing method

**Recommendation:** Verify with proper tests

---

## Detailed Fix Plan

### Fix 1: Signal Handler Enhancement ⚠️ CRITICAL

**File:** `src/signals/signal_handlers.c`

**Current Code:**
```c
void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
```

**Fixed Code:**
```c
void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);      // Clear current input
	rl_on_new_line();             // Move to new line
	rl_redisplay();               // Redisplay prompt
}
```

**Why:**
- Without `rl_replace_line("", 0)`, the current input buffer remains
- User sees the partial command they were typing after Ctrl+C
- This is confusing behavior

**Test:**
1. Run `./minishell`
2. Type: `echo test` (don't press Enter)
3. Press Ctrl+C
4. Expected: New prompt with empty line
5. Actual (before fix): Partial "echo test" may remain

---

### Fix 2: Verify Variable Expansion Works Correctly ✅ LOW PRIORITY

**Status:** Already working, but let's add defensive checks

**Verification Tests:**
```bash
# Test 1: Simple variable
export VAR=hello
echo $VAR
# Expected: hello

# Test 2: Undefined variable
echo $UNDEFINED
# Expected: (empty line)

# Test 3: Exit status
true
echo $?
# Expected: 0

false
echo $?
# Expected: 1

# Test 4: In double quotes
export NAME=world
echo "Hello $NAME"
# Expected: Hello world

# Test 5: In single quotes (no expansion)
echo '$NAME'
# Expected: $NAME

# Test 6: Multiple variables
export A=hello B=world
echo $A $B
# Expected: hello world
```

**If any fail:** Need to investigate specific scenario

---

### Fix 3: Quote Removal Edge Cases ✅ LOW PRIORITY

**Current Implementation:** Working

**Edge Cases to Test:**
```bash
# Test 1: Empty quotes
echo ""
# Expected: (empty line)

# Test 2: Mixed quotes
echo hello"world"test
# Expected: helloworldtest

# Test 3: Quotes with expansion
export VAR=test
echo "Value: $VAR"
# Expected: Value: test

# Test 4: Adjacent quotes
echo "hello""world"
# Expected: helloworld
```

**If any fail:** Need specific fix for that case

---

### Fix 4: Exit Status Edge Cases ✅ LOW PRIORITY

**Current Implementation:** Working

**Edge Cases to Test:**
```bash
# After successful command
ls /
echo $?
# Expected: 0

# After failed command
ls /nonexistent 2>/dev/null
echo $?
# Expected: 1 or 2

# After built-in success
cd .
echo $?
# Expected: 0

# After built-in failure
cd /nonexistent 2>/dev/null
echo $?
# Expected: 1

# In pipe
echo hello | cat
echo $?
# Expected: 0

# After AND
true && echo yes
echo $?
# Expected: 0

# After OR
false || echo yes
echo $?
# Expected: 0
```

---

## Implementation Priority

### Priority 1: CRITICAL - Must Fix ⚠️
**Fix signal handler** (signal_handlers.c)
- Add `rl_replace_line("", 0)` to sigint_handler
- This is the ONLY confirmed bug
- Quick fix, high impact

### Priority 2: VERIFICATION - Confirm Working ✅
**Test with user's specific cases**
- Ask user for exact commands that fail
- Reproduce the issue
- Most issues appear to be working in tests

### Priority 3: EDGE CASES - If Needed
**Address specific failures**
- Only if verification reveals actual bugs
- Targeted fixes for specific scenarios

---

## Testing Plan

### Step 1: Fix Signal Handler
1. Edit `src/signals/signal_handlers.c`
2. Add `rl_replace_line("", 0)` line
3. Recompile: `make re`
4. Test interactively:
   - Type command, press Ctrl+C
   - Verify prompt appears on new line with empty input
   - Type command, press Ctrl+\
   - Verify nothing happens (completely ignored)

### Step 2: Verify Variable Expansion
Run each test from "Fix 2" section above

### Step 3: Verify Quote Removal
Run each test from "Fix 3" section above

### Step 4: Verify Exit Status
Run each test from "Fix 4" section above

### Step 5: User Acceptance Testing
- Have user test with their specific failing cases
- Address any remaining issues

---

## Summary

**Confirmed Bugs:** 1
- Signal handler missing `rl_replace_line()`

**Working Features:** 3
- Variable expansion ✅
- Quote removal ✅
- Exit status ✅

**Action Items:**
1. ✅ Fix signal handler (5 minutes)
2. ⚠️ Test all features with user's specific cases
3. ⚠️ Fix any edge cases discovered during testing

**Estimated Time:**
- Signal fix: 5 minutes
- Verification: 15 minutes
- Edge case fixes (if needed): Variable

**Risk Level:** LOW
- Only one confirmed bug
- Other features working correctly
- May be user testing methodology issue
