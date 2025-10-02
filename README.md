Here is a program for generating permutations of its arguments (an argument is any command line parameter not interpreted as an option or option—argument). When run without options, permutit displays all permutations to the standard output device (normally a screen). Redirection and pipes are supported on Windows, and on POSIX—like operating systems.  

It is known to build with the Microsoft Visual C++ compiler for Windows and the GNU C compiler for Linux. It'll probably work on anything POSIX with ANSI C.  

The source code is the documentation.  

## Options  
**-** :  End processing of options  
- Everything after the **--** is taken as a positional argument  

**?** :  Show an informational screen  

**c** :  Generate combinations  

**d** :  Change the default whitespace separator ('\n' vs. ' ')  

**i** :  Slurp text from the standard input  
- In this case, input is read line by line using fgets()  
- Any permutable arguments entered at the command line are ignored  
- Useful for when piped output from another program is desirable  

**r** :  Give a value for r  
- where: **1 <= r <= N**  

**s** :  Sort positional arguments in ascending ASCII order  

If options are present, they may appear in any order. However **all options must be grouped before the first argument** intended for permutation.  

## Compilation  
To compile the source on **Windows** with **Visual C++**  
`cl.exe -Fepermutit permutit.c cltail.c combinat.c`

To compile on **Linux** with the **GNU C** compiler  
`gcc -opermutit permutit.c cltail.c combinat.c`

## Examples 
`permutit -?`

`permutit arg1 arg2 arg3`

`permutit -r4 arg1 arg2 arg3 arg4 arg5`

`permutit -c -r3 arg1 arg2 arg3 arg4`

`permutit -s -r3 -- -c -a -d -b`
