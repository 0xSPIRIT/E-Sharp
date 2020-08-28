# E-Sharp

E-Sharp is an interpreted programming language (no correlation to the E programming language) that I made for fun.

## Running a program

To run a program, simply do:
```
es program.es
```

## Example Programs

You will find some example programs in the examples/ folder. They include:
- A "Hello, World!" Program;
- A Simple Calculator;
- A Fibonacci Sequence Generator;
- A program that prints the content of a file.

I intend to add more example programs.

## Language Specifications

The language is similar to BrainFuck, but with more conveniences added onto it.
The following are all the commands in the language:

```
@number    # Moves the data pointer to the specified index.
>          # Increments the data pointer.
<          # Decrements the data pointer.

+          # Increments the current cell.
-          # Decrements the current cell.
*          # Squares the current cell.

+&number   # Adds the value at the number'th cell to the current cell.
-&number   # Subtracts the value at the number'th cell to the current cell.
*&number   # Multiplies the current cell by the number'th cell.
/&number   # Divides the current cell by the number'th cell.
%&number   # Modulo's the current cell by the number'th cell.
=&number   # Sets the current cell to another cell.


# IF STATEMENT:
? &number1 &number2 {
    # Do this if cell number1 and cell number2 are equal.
} : { # ':' is the equivalent of an else statement.
    # Do this if the cells are not equal.
}


# LOOPS (identical to BrainFuck's loops):

[    # If the current cell's value is zero, skip the code inside the loop.
]    # If the current cell's value is non-zero, jump it back to the start of the loop.


# INPUT / OUTPUT
;            # Prints the value of the current cell.
,            # Prints the value of the current cell in ASCII.
$&num        # Gets the user's input, then stores it in the num'th cell.
(filename)   # Stores each character from the specified file sequentially, starting from the current cell.
```