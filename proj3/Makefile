NAME=proj3
M2=adjacent

CFLAG= -Wall -std=c99 -std=gnu99 -g
#-O3 -Werror

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
	./$(M2) $(MAP)

test:
	./$(NAME) $(DIR)/$(MAP)

$(NAME):
	$(CC) $(CFLAG) $(SRC) -o $(NAME) -I mycode -lm

clean:
	rm -rf $(NAME) $(M2)

add:
	git add Makefile $(PDF) $(A_SRC) $(SRC)
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj3ms1 milestone1.pdf

ms2submit:
	turnin -c ece368 -p proj3ms2 mycode/adjacent.c
