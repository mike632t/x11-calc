<a id="top"></a>

[![latest packaged version(s)](https://repology.org/badge/latest-versions/x11-calc.svg)](https://repology.org/project/x11-calc/versions)

## x11-calc - Another RPN (Reverse Polish) calculator

Includes emulators for the HP35, HP80, HP45, HP70, HP55, HP21, HP22,  HP25,
HP25C, HP27, HP29C, HP31E, HP32E, HP33E, HP33C, HP34C, HP37E, HP38C, HP38E,
HP10, HP67, HP10C, HP11C, HP12C,  HP15C, and HP16C calculators.

Note - With  the  exception  of the HP10 all the above models  use  Reverse
Polish Notation (RPN) when solving problems.

### Contents <sup><sup>[Back to Top](#top)</sup></sup>

- [How to use RPN](#Q1)

- [How to run x11-calc on Windows](#Q2)

- [How to set the window position at start up](#Q3)

- [Submitting a problem report](#problems)


<a id="Q1"></a>
### How to use Reverse Polish Notation (RPN) <sup><sup>[Back to Top](#top)</sup></sup>

Algebraic expressions are written using infix notation, which requires  the
operator to know the order in which an expression should be evaluated.

```
2 * (3 + 7 * 6) / 3
```

Postfix  notation places the operators after the arguments which allows the
expression above to be rewritten as

```
7 6 * 3 + 2 * 3 /
```

Postfix notation was adopted by Hewlett Packard for their calculators as it
simplified the evaluation of complex expressions and called Reverse  Polish
Notation in honour of Jan Lukasiewicz.

To use RPN you key in an argument and enter it onto the stack before keying
in the second argument and performing an operation.

So to evaluate the expression above you would use the following keystrokes.

```
7
Enter
6
*
3
+
2
*
3
/
```

You do not need to enter it on to the stack if the operation only takes one
argument.

```
tan(45) / 3
```

Is evaluated using the following keystrokes

```
45
tan
3
/
```


Further details can be found in the [Owners Guides](https://literature.hpcalc.org/).

<a id="Q2"></a>
### How to run X11-calc on Windows <sup><sup>[Back to Top](#top)</sup></sup>

Installing Windows Subsystem for Linux on Windows 11 will allow x11-calc to
be run on Windows.  First install WSL2 and then follow the instructions for
Ubuntu to download and compile the program.


<a id="Q3"></a>
### How to set the window position at start up <sup><sup>[Back to Top](#top)</sup></sup>

The position of the window at startup can be defined using the `--geometry`
command line option.

To position the window 120 pixels from the top and 960 pixels from the left
hand side of the screen use the following

```
x11-calc --geometry=+960+120
```


<a id="problems"></a>
### Problem Reports <sup><sup>[Back to Top](#top)</sup></sup>

If  you encounter an issue or have suggestions for future changes to  these
emulators, then please create a new [issue](https://github.com/mike632t/x11-calc/issues).

Your problem report should contain:

- Whether you are using a pre-built binary (Alpine / Flatpak);

- The output from `sh-get-environment.sh`;

- The commit ID;

- A description of the problem.

Thank you.
