# Galaxy Language Documentation

Welcome to the Galaxy Language project! This documentation provides an overview of the core components of the Galaxy language, focusing on the frontend's Lexer and Parser modules. The goal is to help contributors and users understand the structure and functionality of these modules, enabling efficient development and debugging.

## Frontend

### Lexer

The Lexer is located in the `frontend/lexer` folder, comprising three main files:

- **lexer.c**: Responsible for identifying and tokenizing the essential parts of your code.
- **lexer\_error.c**: Handles tokenization errors with concise and detailed error messages.
- **lexer.test.c**: Ensures the Lexer works correctly through unit tests.

#### Token List

The Lexer generates the following tokens:

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

Each token has a specific role in facilitating syntax analysis and semantic checks by the parser.

#### File Dependencies

- `lexer.c` depends on header files located in the `include` folder, including `core.h`, `error.h`, and `utils.h`.

#### Error Handling

- **lexer\_error.c** manages tokenization errors, ensuring messages are informative and help identify the source of issues.

#### Testing

- **lexer.test.c** contains unit tests. Always update tests when adding or modifying tokens and run them before committing changes.


### Parser

The Parser is the next step after tokenization. Its main responsibilities are to construct the Abstract Syntax Tree (AST) and validate the program's syntax.

#### File Structure

- **parser.c**: The main entry point for parsing.
- **parser.test.c**: Unit tests for the Parser.
- **expressions/**: Handles various types of expressions (e.g., binary, unary, object).
- **statements/**: Processes program statements (e.g., package, import).
- **printer/**: Contains utilities for printing the AST.

#### Parsing Process

The Parser begins by creating the **Program Node**, which holds all statements. It then:

1. Calls `parse_stmt.c` to process statements such as:
   - **Package statement** ([parse\_package\_stmt.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/statements/parse_package_stmt.c))
   - **Import statement** ([parse\_import\_stmt.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/statements/parse_import_stmt.c))
2. If no statement is detected, it processes expressions using `parse_expr.c`.

#### Expression Parsing

- **Unary Expressions** ([parse\_unary\_expr.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/expressions/parse_unary_expr.c))
- **Assignment Expressions** ([parse\_assignment\_expr.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/expressions/parse_assignment_expr.c))

Expressions are processed hierarchically:

- Objects ([parse\_object\_expr.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/expressions/parse_object_expr.c))
- Binary Operations ([parse\_additive\_expr.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/expressions/binary_operations/parse_additive_expr.c) → [parse\_multiplicative\_expr.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/expressions/binary_operations/parse_multiplicative_expr.c))
- Primary Expressions ([parse\_primary\_expr.c](https://github.com/galaxy-lang/galaxy/blob/main/src/frontend/parser/expressions/parse_primary_expr.c))


