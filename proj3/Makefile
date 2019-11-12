NAME=proj3

CFLAG= -Werror -O3 -Wall -std=c99 -std=gnu99 -g

MAIN=mycode
HEADER=mycode
INCLUDE=mycode

PDF=milestone1.pdf
	#report.pdf

LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

all: 
	make $(NAME) $(UNNAME)

$(NAME):
	$(CC) $(CFLAG) -o $(NAME) -I mycode

clean:
	rm -rf $(NAME) 

add:
	git add Makefile $(PDF)
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj3ms1 milestone1.pdf

ms2submit:
	turnin -c ece368 -p proj3ms2 adjacent.c
