# IakabScript

This is the official language specification for IakabScript (IS).

## Version
This spec is still a draft, and a work in progress. This spec will get version number v1.0 once the official interpreter is out. Until then, feel free to contribute with any suggestions or code examples.

## Profanity warning
Please note that, while the language itself doesn't use profanity in its syntax, profanity is highly recommended when working with IS. The code examples here make extensive use of profanity.

## Sentence structure

All statements in IS are sentences. Ed never touches the Shift key, and instead either talks in all lowercase, or in all uppercase if shouting. Therefore, all sentences in IS are case-insensitive. This being said, it's your responsibility to make the source code's casing interesting.

A sentence is always followed by a DOT `.` or a NEWLINE `<NL>`. This means that sentences **cannot** be split on multiple lines.

```
Nu deci ilie ii gggggggg
Hoho dutedracu cu ilie oho. Hoho zic "duten pula" hoh
```

## Number literals
Ed doesn't type numbers in chat, and when he does, he surely hates doing it, so IS uses `gg`, `ez` and `nb` to represent number literals.

To represent a number using `gg`, type a bunch of `g`'s. The value of the number literal is the number of `g`'s. For example, `gggggg` would represent the number 6.

As a shorthand, you can write `ez` to represent powers of 10. The amount of `z`'s represent the power. For example, `ezz` would represent 100 and `ezzzzzz` would represent 1000000.

You can additionally represent numbers in binary format. Write the number as you would using 1's and 0's, but instead use `n` and `b`, respectively. For example, `nbbb` is the number 8. (1000b)

The only way to represent a real number is using `eez` notation. This represents a negative power of 10, ie. `eezzzz` is 10^-4, or 0.0001.

The types of real and integer numbers are not differentiated in IS. They fall under the same "number" type.

Compose more complex numbers by using operators such as `ori` or `plus`.

## String literals
Although it might come as a surprise, Ed does actually use double quotes in messages. Therefore, IS supports it.

Write a string literal by encasing it in double quotes (`"`). For example: `"hohohoooo"`. Escaping characters is not possible: you cannot have the character `"` in your string. It's too complicated and Ed wouldn't do that anyway.

## Undefined literal
In cases when a variable doesn't have a value, the state is represented with the `nui` keyword.

```
nu deci pula ii nui
```

## Arrays
TODO: define a method for how arrays would work.

They would accept strings as indexes and _any_ object as values so they could realistically simulate objects, which would be really powerful.

Mutating them would be done using either standard custom operators, or some utility functions defined in the SL.

## Booleans
There are no booleans in IS. When evaluating a value as a condition, only a number value of zero is considered false. Everything else (including all strings) are true.

## Identifiers
Identifiers are sequences of characters that identify things in your program, such as function names or variables. You can use any character from A to Z in your identifiers. Please note that identifiers are case-insensitive, as Ed wouldn't care about that. Additionally, you can form an identifier by chaining a bunch of "Smiling face with 3 hearts" emojis. You cannot combine these two methods.

**NOTE:** The interpreter you use determines whether it accepts Unicode source code or not. If not, the emoji notation is not possible.

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

## Variable declarations (scalars)
Before you can use any variable, you must declare it inside its scope, and before it is used. I'm sure Ed doesn't like declaring his variables in advance, but sometimes he has to make compromises.

Variables in IS don't have types: they are dynamic, and can hold any number or string type.

Declare a variable using `nu deci [identifier] ii [value]`.

```
nu deci ilie ii gggggggggggg
nu deci bubu ii nbnbnbbbnbnnnnbb
```

You can write multiple declarations on a line by separating them with `si`. This is not recommended, as lines can get long, but when has this stopped Ed?

```
nu deci ilie ii gggggggg si bubu ii nbnb. nu deci iacobu ii nbbbbbb
```

## Functions
Functions can only be declared in the global scope, using the `hoho` keyword. They follow this general structure:
```
nu hoho deci [function_name] ia [parameters] si fa
<3 function code here
gata
```

`function_name` needs to be a valid identifier. `parameters` is a list of formal parameter names, separated by `cu`. If the function takes no parameters, use `nimic` instead.

```
<3 valid syntax
nu hoho deci cplm ia nimic si fa
    <3 your function code
gata

<3 or
nu hoho deci uatafac ia pula cu pizda cu dracu si fa
    <33333333
gata
```

To return, optionally with a value, use the keyword `iesi`.
```
nu hoho deci pa ia nimic si fa
    iesi nbb
gata
```

A NL or DOT after `fa` is required.

## Function calls
Call a function using the following syntax:
* With parameters: `hoho [functionName] [params] hoh`
* Without parameters: `hohoh [functionName]`

Separate parameters using the `cu` keyword.

```
hoho uatafac pula cu pizda oho

hohoh cplm
```

Function calls are expressions, and can be assigned to variables and such.

## Control structures
Control structures follow usual Romanian pseudocode-like expressions.
```
<3 if structure
daca [expression] atunci fa
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

A DOT or NL must always follow the `fa` keyword.

There are no other control structures in IS. Ed doesn't need any more.

## Console input/output
To write to the console, use the built-in function `zic`, followed by the variables you want to print.

To read from the console, use the built-in function `zi`. It will read a string from the keyboard and return it.

```
nu deci pula ii gg si pizda ii nbnb
hoho zic "dute dracu" cu pula cu pizda oho
```
```
nu deci coie ii hohoh zi
hoho zic coie hoh
```

## Operators
The following table lists all IS operators, in order of precedence (topmost have least priority). Equal priority operators are evaluated LTR. An `X` marks where the operands would be placed.

Keyword | Category | Name | Priority | Observations
--------|----------|------|----------|-------------
X `egal` X | equality | equal | 1
X `inegal` X | equality | not equal | 1
`invers` X | boolean | logical negation | 2
X `sau` X | boolean | logical OR | 2
X `deodatacu` X | boolean | logical AND | 2 | `si` is already a reserved keyword
X `maimare` X | comparison | more than | 3
X `maimic` X | comparison | less than | 3
X `plus` X | arithmetic | addition | 4
X `minus` X | arithmetic | subtraction | 4
`minus` X | arithmetic | negation | 4
X `ori` X | arithmetic | multiplication | 4
X `impartit la` X | arithmetic | division | 4
X `modulo` X | arithmetic | modulo | 4

There are no parantheses in IS. If they were, Ed wouldn't use them. To simulate overriding priority, just use a sequence of variable assignments.

* Comparison and equality operators work with strings by comparing lexicographically.
* The PLUS operator works with strings by returning a concatenated string.

TODO: extend functionality with arrays?

## Assignment
Assign an expression to a variable using the `ii` keyword.
```
nu deci pula ii nui
pula ii nb
pula ii "pa"
pula ii hoho functie pula hoh
hoho zic pula hoh
```

## Including other files
You can include other files in your project with the `avem` keyword, followed by a string. You can only `avem` files in the global scope (not in functions).

The strings need to be names of `.is` files in the current directory, without extensions. Additionally, the interpreter will look for `.is` files in the `coie` directory.

The strings need NOT be surrounded by double quotes. The whole rest of the sentence will be considered as a file name. Spaces will be substituted with underscores.

The strings can only have letters and spaces.

Do not use the word "piton" in any inclusion. It is a reserved keyword.

All included files must have their names in uppercase (on case-sensitive systems like Linux). When including, they can be in any case.

```
avem o pula <3 will look for "O_PULA.is"
avem dute ba in pizda <3 will look for "DUTE_BA_IN_PIZDA.is"
```

## Standard library
IS provides some utility functions. Additionally, any interpreter might provide its own Standard Library utility functions.

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

```
hoho zic hoho fanumar "1234" oho oho <3 prints "1234"
hoho zic hoho fanumar "1234" "doariakab" oho oho <3 prints <nui>
hoho zic hoho fanumar "gg" "doariakab" oho oho <3 prints 2
```

### `fatext`
This will take as argument a number and return its string representation.

Pass a second parameter to specify the way the number will be converted.

This only converts to the Human format, because what Human other than Ed would understand what "gggnb ezzzz" means?

```
hoho zic hoho fatext gggg oho oho <3 prints 4
```

## Code examples

### Is prime?
Task: define a function that returns 1 if its parameter is prime, and 0 otherwise.

```
nu hoho deci prim ia pula si fa
    nu deci i ii g <3 one
    nu deci p ii b <3 zero
    <3 if it's 1 then return 0
    daca pula egal g atunci fa
        iesi b
    gata

    <3 while i <= pula
    cat timp invers i maimare pula fa
        daca pula modulo i egal b atunci fa
            p ii p plus g
        gata

        i ii i plus g
    gata
    iesi p egal gg <3 if equal to 2, it's prime
gata

nu deci v ii nui
v ii hoho prim cu gg
hoho zic cu v <3 2 is prime
v ii hoho prim cu ggg
hoho zic cu v <3 3 is prime
v ii hoho prim cu gggg
hoho zic cu v <3 4 is NOT prime
```