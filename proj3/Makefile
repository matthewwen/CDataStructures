NAME=proj3
M2=adjacent

CFLAG= -Werror -O3 -Wall -std=c99 -std=gnu99 -g

MAIN=mycode
HEADER=mycode
INCLUDE=mycode

A_SRC=mycode/adjacent.c

PDF=milestone1.pdf
	#report.pdf

LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

all: 
	make $(M2) 

$(M2):
	$(CC) $(A_SRC) $(CFLAG) -o $(M2)

$(NAME):
	$(CC) $(CFLAG) -o $(NAME) -I mycode

clean:
	rm -rf $(NAME) $(M2)

add:
	git add Makefile $(PDF) $(A_SRC)
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj3ms1 milestone1.pdf

ms2submit:
	turnin -c ece368 -p proj3ms2 adjacent.c
