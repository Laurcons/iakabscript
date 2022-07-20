# IakabScript

This is the official language specification for IakabScript (IS).

## Profanity warning
Please note that, while the language itself doesn't use profanity in its syntax, profanity is highly recommended when working with IS. The code examples here make extensive use of profanity.

## Sentence structure

All statements in IS are sentences. Ed never touches the Shift key, and instead either talks in all lowercase, or in all uppercase if shouting. Therefore, all sentences in IS are case-insensitive. This being said, it's your responsibility to make the source code's casing interesting.

Additionally, you can only have one sentence per line, and a line per sentence. Optionally, you can fit multiple sentences on one line, by separating them with a DOT (`.`). The last sentence must not be ended with a DOT. Empty lines are ignored.

```
Nu deci ggggggggg. fix pula
NU SE POATE

DUTE COIE
```

## Number literals
Ed doesn't type numbers in chat, and when he does, he surely hates doing it, so IS uses `gg`, `ez` and `nb` to represent number literals.

To represent a number using `gg`, type a bunch of `g`'s. The value of the number literal is the number of `g`'s. For example, `gggggg` would represent the number 6.

As a shorthand, you can write `ez` to represent powers of 10. The amount of `z`'s represent the power. For example, `ezz` would represent 100 and `ezzzzzz` would represent 1000000.

You can additionally represent numbers in binary format. Write the number as you would using 1's and 0's, but instead use `n` and `b`, respectively. For example, `nbbb` is the number 8. (1000b)

The only way to represent a real number is using `eez` notation. This represents a negative power of 10, ie. `eezzzz` is 10^-4, or 0.0001.

The types of real and integer numbers are not differentiated in IS. They fall under the same "number" type.

## String literals
Although it might come as a surprise, Ed does actually use double quotes in messages. Therefore, IS supports it.

Write a string literal by encasing it in double quotes (`"`). For example: `"hohohoooo"`. Escaping characters is not possible: you cannot have the character `"` in your string. It's too complicated and Ed wouldn't do that anyway.

## Undefined literal
In cases when a variable doesn't have a value, the state is represented with the `nui` keyword.

```
nu deci pula ii nui
```

## Booleans
There are no booleans in IS. When evaluating a value as a condition, only a number value of zero is considered false. Everything else (including all strings) are true.

## Identifiers
Identifiers are sequences of characters that identify things in your program, such as function names or variables. You can use any character from A to Z in your identifiers. Please note that identifiers are case-insensitive, as Ed wouldn't care about that. Additionally, you can form an identifier by chaining a bunch of "Smiling face with 3 hearts" emojis. You cannot combine these two methods.

You cannot have an identifier that starts with `e`, `g`, `n` or `b`.

Valid identifiers:
* `hohooo`
* `🥰🥰🥰🥰🥰`
* `ilie`

Invalid identifiers:
* `cacat🥰🥰🥰`
* `nu`
* `busimiaicoiele`

## Comments
You can write a comment at any end of a line by appending `<3`. The rest of the line will be ignored.

```
Nu deci ilie ii nbbnbn <3 dal drecu
<3 ce pula mea se intampla aici
```

## Variable declarations
Before you can use any variable, you must declare it inside its scope, and before it is used. I'm sure Ed doesn't like declaring his variables in advance, but sometimes he has to make compromises.

Variables in IS don't have types: they are dynamic, and can hold any number or string type.

Declare a variable using `nu deci [identifier] ii fix [value]`.

```
nu deci ilie ii gggggggggggg
nu deci bubu ii fix nbnbnbbbnbnnnnbb
```

You can write multiple declarations on a line by separating them with `si`. This is not recommended, as lines can get long, but when has this stopped Ed?

```
nu deci ilie ii gggggggg si bubu ii fix nbnb. nu deci iacobu ii nbbbbbb
```

## Functions
Functions can only be declared in the global scope, using the `hoho` keyword. They follow this general structure:
```
nu hoho deci [function_name] ia [parameters] si face
<3 function code here
gata
```

`function_name` needs to be a valid identifier. `parameters` is a list of formal parameter names, separated by `cu`. If the function takes no parameters, use `nimic` instead.

```
<3 valid syntax
nu hoho deci cplm ia nimic si face
    <3 your function code
gata

<3 or
nu hoho deci uatafac ia pula cu pizda cu dracu si face
    <33333333
gata
```

To return, optionally with a value, use the keyword `iesi`.
```
nu hoho deci pa ia nimic si face
    iesi nbb
gata
```

Newlines after `face` are allowed.

## Function calls
Call a function by writing `hoho`, its identifier and the list of actual parameters, separated by `cu`. The first parameter is prepended with a `cu`. If there are no parameters, don't write any.

```
hoho uatafac cu pula

hoho cplm
```

Function calls are expressions, and can be assigned to variables and such.

You cannot pass a function call as a parameter. Assign it to a variable first.

## Control structures
Control structures follow usual Romanian pseudocode-like expressions.
```
<3 if structure
daca [expression] atunci
    <3333
altfel
    <33333
gata
```
```
<3 while structure
cat timp [expression] fa
    <3333
gata
```

Newlines after `atunci` and `fa` are allowed.

There are no other control structures in IS. Ed doesn't need any more.

## Console input/output
To write to the console, use the built-in function `zic`, followed by the variables you want to print.

To read from the console, use the built-in function `zi`. It will read a string from the keyboard and return it.

```
nu deci pula ii gg si pizda ii nbnb
hoho zic "dute dracu" cu pula cu pizda
```
```
nu deci coie ii hoho zi
hoho zic cu coie
```

## Arithmetic operators
The following table defines the available arithmetic operators and their keywords.

Name | Math | Priority | IS keyword
-----|------|----------|-----------
addition | `+` | 1 | `plus`
subtraction | `-` | 1 | `minus`
multiplication | `*` | 2 | `ori`
division | `/` | 2 | `impartitla`
modulo | `%` | 2 | `modulo`
exponentiation | `^` | 3 | `ridicatla`

There are no parantheses in IS. If they were, Ed wouldn't use them. To simulate priority, just use a sequence of variable assignments.

Additionally, the `plus` operator can be used with two strings instead of numbers, to join them.

## Assignment
Assign an expression to a variable using the `ii` keyword.
```
nu deci pula ii nui
pula ii nb
pula ii "pa"
pula ii hoho functie cu pula
hoho zic cu pula
```

## Including other files
You can include other files in your project with the `avem` keyword, followed by a string. You can only `avem` files in the global scope (not in functions).

The strings need to be names of `.is` files in the current directory, without extensions. Additionally, the interpreter will look for `.is` files in the `coie` directory.

The strings need NOT be surrounded by double quotes. The whole rest of the sentence will be considered as a file name. Spaces will be substituted with underscores.

The strings can only have letters and spaces.

Do not use "piton" as a string. It is reserved for future use.

All included files must have their names in uppercase (on case-sensitive systems like Linux). When including, they can be in any case.

```
avem o pula <3 will look for "PULA.is"
avem dute ba in pizda <3 will look for "DUTE_BA_IN_PIZDA.is"
```

## Standard library
IS provides some utility functions.

### `fanumar`
Takes a string argument and tries to parse a number from it.

This will recognize numbers in the following formats:
* Human format: `1234`, `1234.5`
* `ez` format
* `nb` and `gg` format
* `eez` format

If you want the parser to reject Human formatted numbers, pass `"doariakab"` as a second argument.

The parsed number is returned.

On parsing failure, `nui` is returned.

### `fatext`
This will take as argument a number and return its string representation.

Pass a second parameter to specify the way the number will be converted.

This only converts to the Human format.

### Comparison functions
These functions compare their parameters. If the parameters are strings, it will compare them lexicographically.

* `maimare`
* `maimic`
* `iegal`
* `inegal`
  
### `invers`
If its parameter evaluates as false, return 1. Otherwise, return 0.

## Code examples

### Is prime?
Task: define a function that returns 1 if its parameter is prime, and 0 otherwise.

```
nu hoho deci prim ia pula si face
    nu deci i ii g <3 one
    nu deci p ii b <3 zero
    <3 if it's 1 then return 0
    daca hoho iegal cu pula cu g atunci
        iesi b
    gata

    <3 while i <= pula
    nu deci interm ii hoho maimare cu i cu pula
    cat timp hoho invers cu interm fa
        daca hoho iegal cu pula modulo i cu b atunci
            p ii p plus g
        gata

        i ii i plus g
        interm ii hoho maimare cu i cu pula
    gata
    iesi hoho iegal cu p cu gg <3 if equal to 2, it's prime
gata

nu deci v ii nui
v ii hoho prim cu gg
hoho zic cu v <3 2 is prime
v ii hoho prim cu ggg
hoho zic cu v <3 3 is prime
v ii hoho prim cu gggg
hoho zic cu v <3 4 is NOT prime
```