#
# makefile
# ingrid, 2017-06-11 14:51
#

all:
	gcc main.c inverted.c sort.c -g -o ingrid -Wall -Werror -Wextra --pedantic -std=c99

# vim:ft=make
#
