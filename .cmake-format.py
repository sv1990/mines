# How wide to allow formatted cmake files
line_width = 80

# How many spaces to tab for indent
tab_size = 2

# If arglists are longer than this, break them always
max_subargs_per_line = 3

# If true, separate flow control names from their parentheses with a space
separate_ctrl_name_with_space = True

# If true, separate function names from parentheses with a space
separate_fn_name_with_space = False

# If a statement is wrapped to more than one line, than dangle the closing
# parenthesis on it's own line
dangle_parens = True

# What character to use for bulleted lists
bullet_char = '*'

# What character to use as punctuation after numerals in an enumerated list
enum_char = '.'

# What style line endings to use in the output.
line_ending = 'unix'

# Format command names consistently as 'lower' or 'upper' case
command_case = 'lower'

# Format keywords consistently as 'lower' or 'upper' case
keyword_case = 'unchanged'

# Specify structure for custom cmake functions
additional_commands = {
  "foo": {
    "flags": [
      "BAR",
      "BAZ"
    ],
    "kwargs": {
      "HEADERS": "*",
      "SOURCES": "*",
      "DEPENDS": "*"
    }
  }
}

# A list of command names which should always be wrapped
always_wrap = []

