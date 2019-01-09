
# Implementace překladače imperativního jazyka IFJ18
# 
# Vorobiev Nikolaj	xvorob00@stud.fit.vutbr.cz
# Luhin Artsiom		xluhin00@stud.fit.vutbr.cz
# Koša Benjamín		xkosab00@stud.fit.vutbr.cz

CC=gcc
FLAGS=-std=gnu99 -Wall -Wextra -g #-Werror -lrt -
BIN=./main
TARGET = 
SOURCE = 
TEST_DIR = ./tests/
TEST = test.rb

###########################################
all: main
		$(CC) $(TARGET)main.o $(TARGET)scaner.o $(TARGET)parser.o $(TARGET)symtable.o $(TARGET)list.o $(TARGET)expression.o $(TARGET)token_list.o $(TARGET)itable.o $(TARGET)p.o $(TARGET)init.o $(TARGET)p2.o $(TARGET)postfix.o $(TARGET)gen_exp.o -o $(TARGET)$(BIN)
main:  parser
		$(CC) -c $(SOURCE)main.c -o $(TARGET)main.o $(FLAGS)
parser:  table
		$(CC) -c $(SOURCE)parser.c -o $(TARGET)parser.o $(FLAGS)
table : stri
		$(CC) -c $(SOURCE)symtable.c -o $(TARGET)symtable.o $(FLAGS)
stri: list
		$(CC) -c $(SOURCE)scaner.c -o $(TARGET)scaner.o $(FLAGS)
list: expression
		$(CC) -c $(SOURCE)list.c -o $(TARGET)list.o $(FLAGS)
expression: itable
		$(CC) -c $(SOURCE)expression.c -o $(TARGET)expression.o $(FLAGS)
itable: token_list
		$(CC) -c $(SOURCE)itable.c -o $(TARGET)itable.o $(FLAGS)
token_list: p_c
		$(CC) -c $(SOURCE)token_list.c -o $(TARGET)token_list.o $(FLAGS)
p_c: init
		$(CC) -c $(SOURCE)p.c -o $(TARGET)p.o $(FLAGS)
init: postfix
		$(CC) -c $(SOURCE)init.c -o $(TARGET)init.o $(FLAGS)
postfix: p2_c
		$(CC) -c $(SOURCE)postfix.c -o $(TARGET)postfix.o $(FLAGS)
p2_c: gen_exp 
		$(CC) -c $(SOURCE)p2.c -o $(TARGET)p2.o $(FLAGS)
gen_exp: 
		$(CC) -c $(SOURCE)gen_exp.c -o $(TARGET)gen_exp.o $(FLAGS)
clean:
	rm -rf $(TARGET)*.o $(TARGET)$(BIN)

#########################################3

memory_test:
	valgrind $(TARGET)$(BIN) < $(TEST_DIR)$(TEST)
	valgrind $(TARGET)$(BIN) < $(TEST_DIR)1$(TEST)
	valgrind $(TARGET)$(BIN) < $(TEST_DIR)2$(TEST)
	valgrind $(TARGET)$(BIN) < $(TEST_DIR)3$(TEST)
	valgrind $(TARGET)$(BIN) < $(TEST_DIR)4$(TEST)
	##valgrind $(TARGET)$(BIN) < $(TEST_DIR)$(TEST)

test: 0test 1test 2test 3test 4test


############## ARTSIOM'S MAKE
ff: 
	$(CC) $(SOURCE)main.c $(SOURCE)token_list.c $(SOURCE)scaner.c -o tlist $(FLAGS)
fff: 
	$(CC) $(SOURCE)main.c $(SOURCE)symtable.c $(SOURCE)p.c $(SOURCE)scaner.c -o prsr $(FLAGS)
post: 
	$(CC) $(SOURCE)main.c $(SOURCE)symtable.c $(SOURCE)token_list.c $(SOURCE)postfix.c $(SOURCE)scaner.c $(SOURCE)init.c -o post $(FLAGS)
cl:
	rm -rf prsr tlist post
##############


0test:
	$(TARGET)$(BIN) < $(TEST_DIR)$(TEST)

1test: 
	$(TARGET)$(BIN) < $(TEST_DIR)1$(TEST)

2test: 
	$(TARGET)$(BIN) < $(TEST_DIR)2$(TEST)

3test: 
	$(TARGET)$(BIN) < $(TEST_DIR)3$(TEST)

4test: 
	$(TARGET)$(BIN) < $(TEST_DIR)4$(TEST)

#########################################
		
