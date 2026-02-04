#!/bin/bash

echo ""
echo "=== Variable Expansion Tests ==="
echo ""

run_test "expand_simple" \
    "export VAR=hello && echo \$VAR" \
    "Simple variable expansion"

run_test "expand_multiple" \
    "export A=hello && export B=world && echo \$A \$B" \
    "Multiple variable expansions"

run_test "expand_undefined" \
    "echo \$UNDEFINED_VAR" \
    "Expand undefined variable"

run_test "expand_exit_status" \
    "true && echo \$?" \
    "Expand exit status after success"

run_test "expand_exit_status_fail" \
    "false && echo \$?" \
    "Expand exit status after failure"

run_test "expand_in_quotes" \
    "export VAR=test && echo \"\$VAR\"" \
    "Expansion in double quotes"

run_test "no_expand_single_quotes" \
    "export VAR=test && echo '\$VAR'" \
    "No expansion in single quotes"

run_test "mixed_quotes_expansion" \
    "export VAR=test && echo '\$VAR' \"\$VAR\"" \
    "Mixed quotes with expansion"

run_test "expand_concat" \
    "export VAR=hello && echo \$VAR\$VAR" \
    "Concatenate expanded variables"

run_test "expand_with_text" \
    "export VAR=world && echo hello\$VAR" \
    "Expansion with adjacent text"

run_test "expand_nested" \
    "export A=B && export B=value && echo \$A" \
    "Variable expansion (not nested evaluation)"

run_test "expand_dollar_sign" \
    "echo \$" \
    "Literal dollar sign at end"

run_test "expand_empty" \
    "export EMPTY= && echo [\$EMPTY]" \
    "Expand empty variable"

run_test "expand_in_command" \
    "export CMD=echo && export MSG=hello && \$CMD \$MSG" \
    "Variable expansion in command name"

run_test "expand_path" \
    "export FILE=testfile && cat \$FILE" \
    "Variable expansion in file path"

run_test "expand_multiple_dollars" \
    "echo \$\$" \
    "Multiple dollar signs"

run_test "expand_question_mark" \
    "ls testfile && echo \$? && ls nonexistent 2>/dev/null || echo \$?" \
    "Question mark expansion after different commands"

run_test "expand_in_redirect" \
    "export OUT=output.txt && echo hello > \$OUT && cat output.txt" \
    "Variable expansion in redirect"
