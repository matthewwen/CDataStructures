NAME=proj1
C_FLAG=-Werror -Wall -O3

SRC=mycode/sorting.c

MAIN=\
	sorting_main.c \
	#test_main.c\

OTHER=mycode/sequence.py

PDF=ms1.pdf

HEADER=mycode/sorting.h
LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

$(NAME):
	$(CC) $(CFLAG) $(SRC) $(MAIN) -o $(NAME) -I mycode

rand:
	random_file output_filename num_of_integers random_seed

clean:
	rm -rf $(NAME)

add:
	git add $(SRC) $(OTHER) $(MAIN) $(HEADER) $(PDF) Makefile
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj1ms1 ms1.pdf
