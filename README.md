# AuTomato

Pronounced `Au * Tomato` (as in Gold Tomato).

```
>print("Hello World")
Hello World
```

## The Goal

The goal of this project is to expand what I learnt in the University course about compiled languages and create an interpretted language.

I built the lexer and parser based off the code from the course I took (where we built a compiler for a subset of the C++ language) and I extended it using the **Visitor Design Pattern** to run the code rather than compile it.

The language is implemented using `C++`, `make`, `Object-Oriented Programming` and `Design Pattern` fundamentals.

AuTomato is meant to look and feel similar to Python, which was my inspiration for the syntax.

## Building And Running

1. Configure with `./configure`
2. Install using `make install`
3. `automato <filename>.aut` to run a code file
4. `automato` to use the shell
