# Our-C Interpreter / Parser

A recursive descent parser and interpreter for a simplified C-like language named **Our-C**, developed for a Programming Languages final project.

## Features

- Tokenizer (Lexical Analyzer)
- Recursive Descent Parser
- Function and Variable Symbol Table
- Basic Interpreter Capabilities
- Built-in Commands:
  - `ListAllFunctions();`
  - `ListAllVariables();`
  - `Done();`

## Project Structure

```
Our-C-Project/
├── README.md
├── LICENSE
├── .gitignore
├── src/
│   └── main.cpp
├── test/
│   └── sample_code.c
├── doc/
│   ├── Our-C-Grammar-2016-07-15.doc
│   ├── RecursiveDescentParsing-Intro.doc
│   └── Java6-ExceptionHandling.doc
```

## Build & Run

```bash
g++ src/main.cpp -o ourc
./ourc < test/sample_code.c
```

## Sample Input

```c
int a;
void print() {
  a = 5;
}
Done();
```

## Documentation

- `doc/Our-C-Grammar-2016-07-15.doc`: Detailed grammar of the Our-C language
- `doc/RecursiveDescentParsing-Intro.doc`: Parsing strategy overview
- `doc/Java6-ExceptionHandling.doc`: Reference on exception handling

## License

This project is licensed under the MIT License.
