# Formula 🏎️🔧👨‍🔧
Formula is a functional programming language written for Compiler Construction exam project.

---

## Table of Contents

1. [Introduction](#introduction)
2. [Getting Started](#getting-started)
   - [Installation](#installation)
   - [Hello, World!](#hello-world)
3. [Language Features](#language-features)
   - [Data Types and Variables](#data-types-and-variables)
   - [Operations](#operations)
   - [Control Flow](#control-flow)
   - [Functions](#functions)
   - [Input and Output](#input-and-output)
4. [Example Programs](#example-programs)
5. [Advanced Topics](#advanced-topics)
   - [Optimization](#optimization)
   - [Static Analysis](#static-analysis)
6. [Contributing](#contributing)
7. [License](#license)

---

## Introduction
Features Overview:
- **Paradigm**: Tries to follow the a pure functional padarigm as much as possible.
- **Syntax**: Inspired to mathematical syntax, accessible from everyone.
- **Immutability**: All variables are Constants.
- **Strong typed**: No implicit conversions are made
- **Static typed**: Type confilct at compile time (not 100% yet)
- **Backend**: Written in C. Lexer and Parser are implemented using GNU flex/bison.
- **Execution Process**: The code is compiled in AST format and then executed in runtime.
- **Ecosystem**: Runs on any machine capable of compile/execute C code.
- **Why use it?**: Having fun learning how to deal with functional programming languages!
---

## Getting Started

### Installation

- Packages required: gcc, make, flex, bison.
- Run `make` to compile binaries and generate compiler/executor. 
- Run `./formula examples/hello_world` 

### Hello, World!
Hello, World! example in Formula:
```
write "Hello World!";
```
Easy! As it should be :)

---

## Language Features

### Data Types and Variables
Data types are `N, Z, R, C, S` and lists `N[], Z[], R[], C[], S[]`. Declarations are **statically** typed:
```
% Variable definition
let natural: N = 2;
let integer: Z = -1;
let real: R = 3.14;
let complex: C = 3 I + 2;
let string: S = "This is a string!";

% List types
let string_list: S[] = ["Hello", "World!"];

% Note: Empty lists have a special syntax `<lowercase_type>[]`
let empty_string_list = s[];
let empty_integer_list = z[];

% Variables are immutable
natural = 4; % error!
```

### Operations
Essential Strong typed arithmetic and logic operations are implemented:
- `N, Z, R` support the ops: `+, -, *, /, ^, ==, !=, <; <=, >, >=`
- `C` support the ops: `+, -, *, /, ==, !=`
- `S` support the ops: `+, ==, !=`
```
% This statements evaluate to 1:
2 == 2;
2 != 1;

% This statements evaluate to 0:
2 == 1 
2 != 2

% Operations are permitted only between variables of SAME type
let natural: N = 100;
let real: R = 100.0;
natural + natural; % ok!
natural + real; % error!
100 == 100.0 % error!
```
Lists introduce new data types `<set>[]` and they all share the same operations:
- `<set>[]` support the ops: `+, ==, !=, slicing, dereferencing`
Example:
```
let string_list: S[] = ["Leclerc", "Hamilton", "Verstappen"];

% slicing: from list get type list of <index>
string_list[1:] % returns a list from elem 1 to end
string_list[:1] % return a list from 0 to elem 1
string_list[1] % return a list with elem "Hamilton"

% dereferencing: from list get a type string of <index>
write string_list.0 % we are able to write because list.0 is of type S
write string_list.0 == string.list.1 % prints 0
```

### Control Flow
Insipired from mathematics, conditions are post-poned in an if statement:
```
let integer: N = 1;
write "The best F1 driver ever is ";
:
  write "Niki Lauda!\n", if integer,
  write "Michael Schumacher!\n", else
; 
```

### Functions
From mathematics, functions are first declared indicating Domain and Codomain of Function.
```
% Function declaration
add_integers: N * N => N;

% Function definition
def add_integers(num1, num2) {
  return num1 + num2;
}

% Function call
let result: N = add_integers(3, 7);
write result; write "\n"
```

### Input and Output
Two basic functions are available:
- `write` can be followed by a statement of type: `N, Z, R, C, S`
- `read` can be followed by a statement of type: `N, Z, R, S`
- `read` must be preceeded by a `let var_name: <type> = ` to infer the type conversion of input from stdin.
Example:
```
% Read from standard input and print
let input_string: S = read;
write input_string + "\n";
```
---

## Example Programs
All example programs can also be found in `examples/` directory.
Tower of Hanoi - Example:
```
hanoi: N * S * S * S => N;

def hanoi(n, src, dst, aux) {
        % Recursive solution to the Tower of Hanoi problem.
	:
	 write src + " -> " + dst + "\n", if n == 1,
	 hanoi(n - 1, src, aux, dst), 
	 write src + " -> " + dst + "\n", 
	 hanoi(n - 1, aux, dst, src), else
	;
	return 0;
}
                          
let origin: S = "Source";
let destination: S = "Destination";
let auxiliary: S = "Auxiliary";

hanoi(3, origin, destination, auxiliary);
```

## Advanced Topics

### Optimization
Two kinds of optimization on AST occur before execution: constant evaluation, if statement evaluation.

#### Costant evaluation
Consider the following simple snippet of code:
```
let var: N = 2 + 7 * 3 - 2 + 6 + 2;
write var;
```
Before:
```
├-- Node Type: 1000
    ├-- Node Type: 1000
    │   └-- Node Type: 1200
    │       └-- Node Type: 2400
    │           ├-- Node Type: 2400
    │           │   ├-- Node Type: 2401
    │           │   │   ├-- Node Type: 2400
    │           │   │   │   ├-- Node Type: 2000
    │           │   │   │   └-- Node Type: 2402
    │           │   │   │       ├-- Node Type: 2000
    │           │   │   │       └-- Node Type: 2000
    │           │   │   └-- Node Type: 2000
    │           │   └-- Node Type: 2000
    │           └-- Node Type: 2000
    └-- Node Type: 2600
        └-- Node Type: 2100
```
After:
```
├-- Node Type: 1000
    ├-- Node Type: 1000
    │   └-- Node Type: 1200
    │       └-- Node Type: 2000
    └-- Node Type: 2600
        └-- Node Type: 2100
```
#### If statement evaluation
Consider the following simple snippet of code:
```
let var: N = 3 * 2 - 5;
:
	write "This is always executed!", if var == var,
	write "This is never executed.", else
;
```
Before:
```
├-- Node Type: 1000
    ├-- Node Type: 1000
    │   └-- Node Type: 1200
    │       └-- Node Type: 2401
    │           ├-- Node Type: 2402
    │           │   ├-- Node Type: 2000
    │           │   └-- Node Type: 2000
    │           └-- Node Type: 2000
    └-- Node Type: 1800
        ├-- Node Type: 1000
        │   └-- Node Type: 2603
        │       └-- Node Type: 2002
        ├-- Node Type: 1700
        │   ├-- Node Type: 2100
        │   └-- Node Type: 2100
        └-- Node Type: 1000
            └-- Node Type: 2603
                └-- Node Type: 2002
```
After:
```
├-- Node Type: 1000
    ├-- Node Type: 1000
    │   └-- Node Type: 1200
    │       └-- Node Type: 2000
    └-- Node Type: 1000
        └-- Node Type: 1000
            └-- Node Type: 2603
                └-- Node Type: 2002
```

### Static Analysis
todo - detailed description of static analysis 

---

## Contributing
Feel free to fork or send pull requests! :)

---

## License
MIT License.
