
# sqlfun

An SQL parser, generated from Bison (Yacc) and Flex (Lex).

Original version - most of the work - comes from John Levine's
Flex & Bison book published by O'Reilley.

## Example usage:

	$ echo "SELECT a.name,b.address FROM people ORDER BY name;" | ./sql

## Requirements

1. GNU Flex
2. GNU bison
3. GNU autotools (autoconf, automake, etc.)
4. A C compiler

## Standard GNU autotools building instructions:

	$ ./autogen.sh
	$ ./configure
	$ make && make check
	$ sudo make install

