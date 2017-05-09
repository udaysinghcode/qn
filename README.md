# qn
qn is a text editor which is designed to be lightweight, easily modifiable, and 
entirely written in C.

Inspired by antirez's kilo, the idea is to write a text editor and explore C.

Writing a text editor will probably be boring, but by keeping the scope limited 
and the feature set small, I hope to be able to get back into C for a little bit
 and add some weights to my proverbial bat.

## How to use it
qn is super lightweight, make sure you have gcc installed and c99.

```
which gcc

> /usr/bin/gcc
```

Make sure a result or something similar comes up to make sure gcc is set up as a
compiler. 

To run do the following:

```
make
```

You should then get an executable called qn which can be run as so:

```
./qn
```

You can quit with CTRL-C or CTRL-D 
