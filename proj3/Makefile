NAME=proj3
M2=adjacent

CFLAG= -Wall -std=c99 -std=gnu99 -g -O3 -Werror

MAIN=mycode
HEADER=mycode
INCLUDE=mycode

A_SRC=mycode/adjacent.c

SRC=mycode/source.c\
    main.c

PDF=milestone1.pdf
	#report.pdf

DIR=testfiles
MAP=map5x5.txt
QUERY=query5x5.txt
ROADCALC=roadCA-large-coordinates.txt
ROADCA=roadCA.txt
ROADCA0=roadCA-v0.txt
SHORTPATH=shortestpath
USA100=usa100.txt
USA10=usa10.txt
USA1=usa1.txt
USA=usa.txt

LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

all: 
	make $(NAME) 

$(M2):
	$(CC) $(A_SRC) $(CFLAG) -o $(M2)

atest:
	./$(M2) $(DIR)/$(MAP) 

test1:
	./$(NAME) $(DIR)/$(USA) $(DIR)/$(USA1) > test1.txt
	diff actual1.txt test1.txt > diff.txt

test2:
	./$(NAME) $(DIR)/$(USA) $(DIR)/$(USA10) > test2.txt
	diff actual2.txt test2.txt > diff.txt

test3:
	./$(NAME) $(DIR)/$(USA) $(DIR)/$(USA100) > test3.txt
	diff actual3.txt test3.txt > diff.txt

mtest:
	valgrind ./$(NAME) $(DIR)/$(MAP) $(DIR)/$(QUERY)

$(NAME):
	$(CC) $(CFLAG) $(SRC) -o $(NAME) -I mycode -lm

clean:
	rm -rf $(NAME) $(M2)

add:
	git add Makefile $(PDF) $(A_SRC) $(SRC) $(HEADER)/header.h
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj3ms1 milestone1.pdf

ms2submit:
	turnin -c ece368 -p proj3ms2 mycode/adjacent.c
