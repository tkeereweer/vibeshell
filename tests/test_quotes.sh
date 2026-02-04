#!/bin/bash

echo ""
echo "=== Quote Processing Tests ==="
echo ""

run_test "double_quotes" \
    "echo \"hello world\"" \
    "Double quotes"

run_test "single_quotes" \
    "echo 'hello world'" \
    "Single quotes"

run_test "mixed_quotes" \
    "echo \"hello\" 'world'" \
    "Mixed quotes"

run_test "quotes_with_spaces" \
    "echo \"  hello  world  \"" \
    "Quotes preserving spaces"

run_test "empty_quotes_double" \
    "echo \"\"" \
    "Empty double quotes"

run_test "empty_quotes_single" \
    "echo ''" \
    "Empty single quotes"

run_test "quotes_in_quotes" \
    "echo \"it's working\"" \
    "Single quote in double quotes"

run_test "expansion_in_double" \
    "export VAR=test && echo \"value is \$VAR\"" \
    "Variable expansion in double quotes"

run_test "no_expansion_single" \
    "export VAR=test && echo 'value is \$VAR'" \
    "No expansion in single quotes"

run_test "quote_concatenation" \
    "echo hello\"world\"test" \
    "Quote concatenation"

run_test "adjacent_quotes" \
    "echo \"hello\"\"world\"" \
    "Adjacent double quotes"

run_test "alternating_quotes" \
    "echo 'hello'\"world\"'test'" \
    "Alternating quote types"

run_test "quote_with_special" \
    "echo \"test\$?test\"" \
    "Special chars in quotes"

run_test "unclosed_handling" \
    "echo test" \
    "Properly closed quotes (baseline)"

run_test "multiple_quote_pairs" \
    "echo \"a\" \"b\" \"c\"" \
    "Multiple quoted arguments"

run_test "quote_empty_var" \
    "export EMPTY= && echo \"\$EMPTY\"" \
    "Quoted empty variable expansion"

run_test "quotes_preserve_stars" \
    "echo \"*\"" \
    "Quotes prevent wildcard expansion"

run_test "single_quotes_preserve_all" \
    "echo '\$VAR * \$?'" \
    "Single quotes preserve everything"
