## Welcome

Welcome, before starting with code, you must understand the structure of our compiler, this is its tree:

```
├── CMakeLists.txt
├── CODE_OF_CONDUCT.md
├── LICENSE
├── README.md
├── cmake_install.cmake
├── docs
│   ├── contributing.md
│   ├── contributors_guide
│   │   └── start.md
│   ├── first_steps.md
│   └── grammar.md
├── examples
│   ├── a.glx
│   ├── small.glx
│   └── test.glx
├── include
│   ├── ast
│   │   ├── core.h
│   │   └── definitions.h
│   ├── lexer
│   │   ├── core.h
│   │   ├── error.h
│   │   └── freeTokens.h
│   ├── parser
│   │   ├── core.h
│   │   ├── expressions
│   │   │   ├── binary_operations
│   │   │   │   ├── parse_additive_expr.h
│   │   │   │   └── parse_multiplicative_expr.h
│   │   │   ├── parse_assignment_expr.h
│   │   │   ├── parse_expr.h
│   │   │   ├── parse_object_expr.h
│   │   │   ├── parse_primary_expr.h
│   │   │   └── parse_unary_expr.h
│   │   ├── printer
│   │   │   ├── nodes
│   │   │   │   ├── print_assignment.h
│   │   │   │   ├── print_binary_expr.h
│   │   │   │   ├── print_identifier.h
│   │   │   │   ├── print_import.h
│   │   │   │   ├── print_minus.h
│   │   │   │   ├── print_numeric_literal.h
│   │   │   │   ├── print_object.h
│   │   │   │   ├── print_package.h
│   │   │   │   ├── print_pre_decrement.h
│   │   │   │   ├── print_program.h
│   │   │   │   └── print_property.h
│   │   │   ├── print_ast.h
│   │   │   ├── print_indent.h
│   │   │   └── visited.h
│   │   └── statements
│   │       ├── parse_import_stmt.h
│   │       ├── parse_package_stmt.h
│   │       └── parse_stmt.h
│   └── utils.h
└── src
    ├── frontend
    │   ├── CMakeLists.txt
    │   ├── lexer
    │   │   ├── CMakeLists.txt
    │   │   ├── cmake_install.cmake
    │   │   ├── lexer.c
    │   │   ├── lexer.test.c
    │   │   └── lexer_error.c
    │   ├── node_definitions
    │   │   ├── CMakeLists.txt
    │   │   ├── ast.c
    │   │   └── cmake_install.cmake
    │   └── parser
    │       ├── CMakeLists.txt
    │       ├── cmake_install.cmake
    │       ├── expressions
    │       │   ├── binary_operations
    │       │   │   ├── parse_additive_expr.c
    │       │   │   └── parse_multiplicative_expr.c
    │       │   ├── parse_assignment_expr.c
    │       │   ├── parse_expr.c
    │       │   ├── parse_object_expr.c
    │       │   ├── parse_primary_expr.c
    │       │   └── parse_unary_expr.c
    │       ├── parser.c
    │       ├── parser.test.c
    │       ├── printer
    │       │   ├── nodes
    │       │   │   ├── print_assignment.c
    │       │   │   ├── print_binary_expr.c
    │       │   │   ├── print_identifier.c
    │       │   │   ├── print_import.c
    │       │   │   ├── print_minus.c
    │       │   │   ├── print_numeric_literal.c
    │       │   │   ├── print_object.c
    │       │   │   ├── print_package.c
    │       │   │   ├── print_pre_decrement.c
    │       │   │   ├── print_program.c
    │       │   │   └── print_property.c
    │       │   ├── print_ast.c
    │       │   ├── print_indent.c
    │       │   └── visited.c
    │       └── statements
    │           ├── parse_import_stmt.c
    │           ├── parse_package_stmt.c
    │           └── parse_stmt.c
    └── main.c
```

Don't be scared, it's easier than it looks, let's start with the frontend!

## frontend

#### Lexer: 
```
    │   ├── lexer
    │   │   ├── CMakeLists.txt
    │   │   ├── cmake_install.cmake
    │   │   ├── lexer.c
    │   │   ├── lexer.test.c
    │   │   └── lexer_error.c
```

In the lexer folder we have 3 **main** files, **lexer.c**, **lexer.test.c** and **lexer_error.c**, let's start by talking about lexer.c, the lexer is responsible for identifying and tokenizing the important parts of your code, so that the parser can generate the ast and pro semantic check the syntax. Our lexer so far has these tokens:

```
    TOKEN_UNKNOWN,
    TOKEN_NOT,
    TOKEN_BITWISE_NOT,
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_PACKAGE,
    TOKEN_IMPORT,
    TOKEN_DEF,
    TOKEN_RETURN,
    TOKEN_IDENTIFIER,
    TOKEN_INT,
    TOKEN_DECIMAL,
    TOKEN_STRING,
    TOKEN_END,
    TOKEN_SEMICOLON,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_ELLIPSIS,
    TOKEN_IF,
    TOKEN_FOR,
    TOKEN_ELSE,
    TOKEN_ELIF,
    TOKEN_SWITCH,
    TOKEN_CASE,
    TOKEN_DEFAULT,
    TOKEN_OPAREN,
    TOKEN_CPAREN,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_ARROW,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_POWER,
    TOKEN_MODULUS,
    TOKEN_ASSIGN,
    TOKEN_DOT,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_EQUAL,
    TOKEN_CARET,
    TOKEN_RANGE,
    TOKEN_LEQUAL,
    TOKEN_GEQUAL,
    TOKEN_OBRACE,
    TOKEN_CBRACE,
    TOKEN_EOF
```

> Where each of these tokens has a responsibility, which I already mentioned above, lexer.c depends on the header files core.h, error.h and utils.h, which are in the include folder.

Now lexer_error.c is our error handler, it is responsible for handling tokenization errors in a concise and detailed way, so that the error messages are better to understand where these errors came from, or what is These errors, tokenization errors, are the only ones that take care of. 

Now lexer.test.c is our test file, it is very important to update it whenever you add or change a token, and always before committing, try to run unit tests, this is very important to maintain the quality of our language.