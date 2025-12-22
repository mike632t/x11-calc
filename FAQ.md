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

- [Submitting a problem report](#problems)


<a id="Q1"></a>
### How to use Reverse Polish Notation (RPN) <sup><sup>[Back to Top](#top)</sup></sup>

Algebraic expressions are written using infix notation, which requires  the
operator to know the order in which an expression should be evaluated.

```
7 * (4 + 5) + 3
```

Postfix  notation places the operators after the arguments which allows the
expression above to be rewritten as

```
4 5 + 7 * 3 +
```

This is now known as Reverse Polish Notation in honour of Jan Lukasiewicz.

To use RPN you key in an argument and enter it onto the stack before keying
in the second argument and selecting the operation.

So you would use the following keystrokes to evaluate the expression above.

```
4
Enter
5
+
7
*
3
+
```

Further details can be found in the [Owners Guides](https://literature.hpcalc.org/).

<a id="Q2"></a>
### How to run X11-calc on Windows <sup><sup>[Back to Top](#top)</sup></sup>

Installing Windows Subsystem for Linux on Windows 11 will allow x11-calc to
be run on Windows.  First install WSL2 and then follow the instructions for
Ubuntu to download and compile the program.


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
