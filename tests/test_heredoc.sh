#!/bin/bash

echo ""
echo "=== Heredoc Tests ==="
echo ""

run_test "heredoc_simple" \
    "cat << EOF
line1
line2
EOF" \
    "Simple heredoc"

run_test "heredoc_with_expansion" \
    "export VAR=expanded && cat << EOF
before \$VAR after
EOF" \
    "Heredoc with variable expansion"

run_test "heredoc_empty" \
    "cat << EOF
EOF" \
    "Empty heredoc"

run_test "heredoc_single_line" \
    "cat << EOF
single line
EOF" \
    "Single line heredoc"

run_test "heredoc_with_quotes" \
    "cat << EOF
\"quoted text\"
EOF" \
    "Heredoc with quotes"

run_test "heredoc_exit_status" \
    "export VAR=42 && cat << EOF
Status: \$?
EOF" \
    "Heredoc with exit status expansion"

run_test "heredoc_multiple_vars" \
    "export A=hello && export B=world && cat << EOF
\$A \$B
EOF" \
    "Heredoc with multiple variable expansions"

run_test "heredoc_with_command" \
    "cat << EOF | grep line2
line1
line2
line3
EOF" \
    "Heredoc piped to command"

run_test "heredoc_undefined_var" \
    "cat << EOF
\$UNDEFINED
EOF" \
    "Heredoc with undefined variable"

run_test "heredoc_concat_expansion" \
    "export VAR=test && cat << EOF
prefix\${VAR}suffix
EOF" \
    "Heredoc with concatenated expansion"

run_test "heredoc_special_chars" \
    "cat << EOF
\$? \$\$ \$VAR
EOF" \
    "Heredoc with special expansions"

run_test "heredoc_to_file" \
    "cat << EOF > heredoc_out.txt && cat heredoc_out.txt
content
EOF" \
    "Heredoc redirected to file"
