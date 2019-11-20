NAME=proj3
M2=adjacent

CFLAG= -Werror -O3 -Wall -std=c99 -std=gnu99 -g

MAIN=mycode
HEADER=mycode
INCLUDE=mycode

A_SRC=mycode/adjacent.c

PDF=milestone1.pdf
	#report.pdf

MAP=testfiles/map5x5.txt
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
	make $(M2) 

$(M2):
	$(CC) $(A_SRC) $(CFLAG) -o $(M2)

atest:
	./$(M2) $(MAP)

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
	turnin -c ece368 -p proj3ms2 mycode/adjacent.c
