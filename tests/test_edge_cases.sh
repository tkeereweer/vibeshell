#!/bin/bash

echo ""
echo "=== Edge Cases and Complex Scenarios ==="
echo ""

run_test "complex_redirects_1" \
    "cat < file1 > out.txt < file2 < file3 && cat out.txt" \
    "Multiple input redirects (last wins)"

run_test "complex_redirects_2" \
    "echo hello > a.txt > b.txt > c.txt && cat c.txt" \
    "Multiple output redirects (last wins)"

run_test "complex_redirects_3" \
    "< file1 > out.txt cat && cat out.txt" \
    "Redirects before command"

run_test "expand_to_expand" \
    "export A=B && export B=VALUE && echo \$A" \
    "Variable expands to another variable name (not nested)"

run_test "expand_empty_concat" \
    "export EMPTY= && echo prefix\${EMPTY}suffix" \
    "Empty variable in concatenation"

run_test "wildcard_in_quotes" \
    "echo '*' '*.txt'" \
    "Wildcards in quotes (no expansion)"

run_test "pipe_with_wildcards" \
    "cat *.txt | grep line" \
    "Pipe with wildcard expansion"

run_test "redirect_wildcard" \
    "cat file* > combined.txt && cat combined.txt" \
    "Redirect with wildcards"

run_test "heredoc_in_pipe" \
    "cat << EOF | grep hello
hello world
test
EOF" \
    "Heredoc in pipeline"

run_test "var_in_heredoc_redirect" \
    "export OUT=hd_out.txt && cat << EOF > \$OUT && cat hd_out.txt
content
EOF" \
    "Variable expansion in heredoc redirect"

run_test "complex_quotes_1" \
    "echo \"it's\" 'a \"test\"' mixed" \
    "Complex quote mixing"

run_test "complex_quotes_2" \
    "export VAR=val && echo '\$VAR' \"\$VAR\" \$VAR" \
    "Variable expansion with different quotes"

run_test "empty_var_quotes" \
    "export E= && echo \"[\$E]\" '[\$E]' [\$E]" \
    "Empty variable in different quote contexts"

run_test "many_pipes" \
    "echo test | cat | cat | cat | cat | cat" \
    "Many pipes in sequence"

run_test "logical_with_pipes" \
    "echo hello | grep hello && echo found || echo not_found" \
    "Logical operators with pipes"

run_test "all_features_combo" \
    "export VAR=testfile && cat < \$VAR | grep test > out.txt && cat out.txt" \
    "Combination: expansion, redirect, pipe"

run_test "quote_empty_space" \
    "echo \"\" \"\" \"\"" \
    "Multiple empty quotes"

run_test "special_chars_quoted" \
    "echo '\$? \$\$ \$VAR * | > <'" \
    "Special characters in single quotes"

run_test "redirect_ambiguous" \
    "export EMPTY= && echo test > \$EMPTY" \
    "Ambiguous redirect (empty variable)"

run_test "exit_status_in_heredoc" \
    "false && cat << EOF
Status was \$?
EOF" \
    "Exit status expansion in heredoc"

run_test "wildcard_no_match_literal" \
    "echo *.nonexistent_ext" \
    "Wildcard with no match stays literal"

run_test "pipe_exit_status" \
    "false | true && echo \$?" \
    "Exit status of pipeline"

run_test "redirect_permissions" \
    "echo test > testfile && cat testfile" \
    "Redirect overwrites existing file"

run_test "heredoc_delimiter_quoted" \
    "cat << \"EOF\"
test \$VAR
EOF" \
    "Heredoc with quoted delimiter (no expansion)"

run_test "cd_home_tilde" \
    "cd && pwd" \
    "CD to home directory"

run_test "export_multiple" \
    "export A=1 B=2 C=3 && echo \$A \$B \$C" \
    "Export multiple variables (if supported)"

run_test "unset_multiple" \
    "export A=1 && export B=2 && unset A B && echo [\$A] [\$B]" \
    "Unset multiple variables"

run_test "echo_backslash" \
    "echo \\" \
    "Echo with backslash"

run_test "dollar_no_var" \
    "echo \$" \
    "Dollar sign alone"

run_test "dollar_number" \
    "echo \$0 \$1 \$2" \
    "Dollar with numbers (positional params)"

run_test "consecutive_pipes" \
    "echo a | cat | cat" \
    "Consecutive pipes"

run_test "redirect_then_pipe" \
    "echo hello > tmp.txt && cat tmp.txt | grep hello" \
    "Redirect followed by pipe (separate commands)"

run_test "long_command_line" \
    "echo a b c d e f g h i j k l m n o p q r s t u v w x y z" \
    "Long command line with many arguments"

run_test "many_vars_expand" \
    "export A=1 B=2 C=3 D=4 E=5 && echo \$A\$B\$C\$D\$E" \
    "Many variable expansions concatenated"

run_test "nested_quotes_complex" \
    "echo \"'\$VAR'\" '\"\$VAR\"'" \
    "Nested quotes complex"

run_test "expansion_then_wildcard" \
    "export PATTERN='*.txt' && echo \$PATTERN" \
    "Variable contains wildcard pattern"

run_test "redirect_to_existing" \
    "echo old > file && echo new > file && cat file" \
    "Redirect overwrites existing content"

run_test "append_to_nonexistent" \
    "echo hello >> newfile.txt && cat newfile.txt" \
    "Append creates file if nonexistent"
