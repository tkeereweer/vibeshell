# Interactive Testing Guide

## How to Test Your Minishell

### ⚠️ IMPORTANT: Testing Method Matters!

The way you test affects the results. Here are the correct methods:

---

## Method 1: Interactive Testing (What You're Doing)

**Start the shell:**
```bash
$ ./minishell
minishell>
```

**Now type commands ONE AT A TIME:**

### Test 1: Environment Variables (Inherited)
```bash
minishell> echo $USER
```
**Expected:** Your username (e.g., "thijskeereweer")

```bash
minishell> echo $HOME
```
**Expected:** Your home directory path

```bash
minishell> echo $PATH
```
**Expected:** Your PATH variable contents

### Test 2: Export and Use Variables
```bash
minishell> export MYVAR=hello
minishell> echo $MYVAR
```
**Expected:** "hello"

```bash
minishell> export NAME=world
minishell> echo "Hello $NAME"
```
**Expected:** "Hello world"

### Test 3: Exit Status
```bash
minishell> true
minishell> echo $?
```
**Expected:** "0"

```bash
minishell> false
minishell> echo $?
```
**Expected:** "1"

### Test 4: Quotes
```bash
minishell> echo "hello world"
```
**Expected:** hello world (quotes removed)

```bash
minishell> echo '$USER'
```
**Expected:** $USER (no expansion in single quotes)

```bash
minishell> export VAR=test
minishell> echo "$VAR"
```
**Expected:** test (expansion in double quotes)

### Test 5: Signals
**While at prompt:**
1. Press `Ctrl+C`
   - Expected: New line, new prompt
   - Shell should NOT exit

2. Type: `sleep 10` then press Enter
3. While sleep is running, press `Ctrl+C`
   - Expected: Command terminates, back to prompt
   - Shell should NOT exit

4. Press `Ctrl+\` at prompt
   - Expected: Nothing (completely ignored)
   - Shell should NOT exit

---

## Method 2: Piped Testing (For Automation)

**Test with piped commands:**
```bash
$ printf 'export TEST=hello\necho $TEST\nexit\n' | ./minishell
```
**Expected output:**
```
hello
exit
```

**Test exit status:**
```bash
$ printf 'true\necho $?\nexit\n' | ./minishell
```
**Expected output:**
```
0
exit
```

---

## Method 3: Here-Document Testing

```bash
$ ./minishell << 'EOF'
export MYVAR=value
echo $MYVAR
exit
EOF
```

---

## Common Issues and Solutions

### Issue: `echo $USER` shows `$USER` literally

**Possible Causes:**

1. **Shell Variable Expansion Interference**
   - If you're in bash/zsh and type: `./minishell` then `echo $USER`
   - Bash might expand $USER BEFORE passing to minishell
   - **Solution:** The variable SHOULD be expanded by YOUR minishell, not the parent shell

2. **Testing with Script Instead of Interactive**
   - If using a script with single quotes: `echo '$USER'`
   - Single quotes prevent expansion (this is CORRECT behavior)
   - **Solution:** Use double quotes or no quotes: `echo $USER` or `echo "$USER"`

3. **Tokenizer Issue**
   - Rare: tokenizer might not preserve the $
   - **Solution:** Check if typing works vs piping

### Issue: Export doesn't persist

**Check:**
```bash
minishell> export TEST=hello
minishell> export | grep TEST
```
Should show: `declare -x TEST="hello"`

If not shown, there's a bug in export.

**Then test:**
```bash
minishell> echo $TEST
```
Should show: `hello`

### Issue: Quotes not removed

**Test explicitly:**
```bash
minishell> echo "test"
```
Should output: `test` (NOT `"test"`)

---

## Verification Checklist

Run these commands IN ORDER in interactive mode:

```bash
./minishell

# ✓ Check 1: Inherited variables
echo $USER
echo $HOME

# ✓ Check 2: Export variables
export MYVAR=test123
echo $MYVAR

# ✓ Check 3: Exit status
true
echo $?
false
echo $?

# ✓ Check 4: Quotes
echo "hello world"
echo 'hello world'
echo '$USER'
echo "$USER"

# ✓ Check 5: Expansion in various contexts
export NAME=world
echo Hello $NAME
echo "Hello $NAME"
echo 'Hello $NAME'

# ✓ Check 6: Multiple variables
export A=hello B=world
echo $A $B

# ✓ Check 7: Pipes
echo test | cat

# ✓ Check 8: Redirects
echo test > /tmp/test.txt
cat /tmp/test.txt

# ✓ Check 9: Logical operators
true && echo yes
false || echo yes

# Exit
exit
```

---

## Expected vs Actual

For each command above, note:
- **What you typed**
- **What you expected**
- **What you actually got**

If something doesn't work, provide the EXACT command and output.

---

## Debugging Steps

If variable expansion doesn't work:

1. **Verify export works:**
   ```bash
   minishell> export TEST=hello
   minishell> export | grep TEST
   ```

2. **Test with simple variable:**
   ```bash
   minishell> export X=1
   minishell> echo $X
   ```

3. **Test with inherited variable:**
   ```bash
   minishell> echo $HOME
   ```

4. **Test with piped input:**
   ```bash
   $ printf 'echo $USER\nexit\n' | ./minishell
   ```

5. **Check tokenizer output (if you have debug mode):**
   - Does the token contain "$USER" or just "USER"?

---

## Known Differences from Bash

1. **No semicolon support**
   - Bash: `echo hello; echo world` ✓
   - Minishell: `echo hello; echo world` ✗
   - Use: `echo hello` (Enter) then `echo world` (Enter) ✓

2. **No command substitution**
   - Bash: `echo $(date)` ✓
   - Minishell: `echo $(date)` ✗

3. **No brace expansion**
   - Bash: `echo {a,b,c}` ✓
   - Minishell: `echo {a,b,c}` ✗

4. **Limited redirect syntax**
   - Bash: `2>&1` ✓
   - Minishell: `2>&1` ✗
   - Use: Basic `>`, `<`, `>>`, `<<` only ✓

---

## My Test Results (Automated)

```bash
$ printf 'export MYVAR=test\necho $MYVAR\nexit\n' | ./minishell
test  ← ✓ WORKS

$ printf 'echo $USER\nexit\n' | ./minishell
thijskeereweer  ← ✓ WORKS

$ printf 'true\necho $?\nexit\n' | ./minishell
0  ← ✓ WORKS

$ printf 'echo "hello world"\nexit\n' | ./minishell
hello world  ← ✓ WORKS (quotes removed)
```

**All features working in automated tests!**

---

## If Still Not Working

Please provide:

1. **Exact commands you're typing** (copy-paste from terminal)
2. **Exact output you're seeing**
3. **Expected output**
4. **Are you testing interactively or with a script?**
5. **macOS version** (this might affect readline behavior)

Example:
```
I typed: echo $USER
I expected: thijskeereweer
I got: $USER
Testing method: Interactive (typed in ./minishell prompt)
macOS version: 15.2
```

This will help me identify the exact issue!
