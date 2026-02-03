# Minishell - Quick Start Guide

## What We Have

Three comprehensive planning documents:
1. **CLAUDE.md** - 42 Norm compliance guide
2. **ARCHITECTURE.md** - Overall system architecture
3. **PARSING_PLAN.md** - Detailed parsing strategy with examples
4. **IMPLEMENTATION_ROADMAP.md** - Step-by-step implementation plan

## Implementation Strategy

### Tokenizer → Parser → Expander → Executor

We're starting with **Tokenizer** and **Parser** (the parsing stage).

## Ready to Start?

### Option 1: Start from Scratch

Follow **IMPLEMENTATION_ROADMAP.md** Phase 1:

```bash
# 1. Create directories
mkdir -p includes src/lexer src/parser src/utils tests

# 2. Start with header files
# I can write these for you - just ask!
```

### Option 2: I Write, You Review

I can implement each component while you review:

1. **You tell me**: "Implement Phase 1" or "Write tokenize.c"
2. **I write** the code following the 42 Norm
3. **You review** and test
4. **We iterate** until perfect

### Option 3: Pair Programming

We work together:
- I explain what to write
- You write it (or I write it)
- We debug together
- We test together

## What to Implement First?

### Immediate Next Steps (Phase 1)

1. **Header files** - Define all structures
   - `includes/tokens.h`
   - `includes/lexer.h`
   - `includes/ast.h`
   - `includes/parser.h`
   - `includes/minishell.h`

2. **Makefile** - Set up compilation

3. **Libft** - Ensure these functions exist:
   - `ft_strdup`
   - `ft_strlen`
   - `ft_putstr_fd`

## Estimated Timeline

- **Phase 1 (Setup)**: 1 day
- **Phase 2-3 (Tokenizer)**: 4-5 days
- **Phase 4-8 (Parser)**: 8-10 days
- **Phase 9-10 (Bonus)**: 4-5 days
- **Total**: ~3 weeks for tokenizer + parser

## Key Design Decisions

### Tokenizer
- **State machine** approach (5 states)
- **Linked list** of tokens
- **Quote handling**: Preserve quotes for later expansion
- **Operator recognition**: Lookahead for multi-char operators

### Parser
- **Recursive descent** with operator precedence
- **AST structure**: Union-based nodes (memory efficient)
- **Precedence**: `||` < `&&` < `|` < redirections
- **Syntax validation**: Pre-parse checks

## 42 Norm Quick Reference

```c
// ✅ GOOD
int	function_name(t_type *param)
{
	t_token	*token;
	char	*str;
	int		len;
	int		i;

	token = NULL;
	i = 0;
	while (i < len)
	{
		// Do something
		i++;
	}
	return (value);
}

// ❌ BAD
int function_name(int a, int b, int c, int d, int e) { // Too many params
	for (int i = 0; i < 10; i++) { // For loop forbidden
		int x = (a > b) ? a : b; // Ternary forbidden
	}
}
```

## Testing Strategy

Test after EVERY function:

```c
// Example test
int main(void)
{
	t_token *tok = token_create(TOKEN_WORD, "echo", 0);
	assert(tok != NULL);
	assert(tok->type == TOKEN_WORD);
	assert(strcmp(tok->value, "echo") == 0);
	printf("✓ token_create works\n");
	token_list_free(tok);
	return (0);
}
```

## Common Pitfalls

1. **Don't skip testing** - Test each function immediately
2. **Don't optimize prematurely** - Make it work, then make it fast
3. **Don't forget to free** - Every malloc needs a free
4. **Don't mix concerns** - One function, one job
5. **Don't ignore the Norm** - Run norminette frequently

## Getting Help

If you get stuck:
1. Check the detailed plans (agent outputs)
2. Look at the examples in PARSING_PLAN.md
3. Ask me specific questions
4. Show me your code for review

## Ready?

Tell me how you want to proceed:
- "Let's start with Phase 1"
- "Write the header files for me"
- "Explain how the tokenizer works"
- "Show me an example of [specific feature]"

I'm here to help you build this! 🚀
