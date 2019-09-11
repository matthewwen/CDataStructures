NAME=proj1
C_FLAG=-Werror -Wall -O3
SRC=mycode/sorting.c \
	sorting_main.c
HEADER=mycode/sorting.h
LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

$(NAME):
	$(CC) $(CFLAG) $(SRC) -o $(NAME)

rand:
	random_file output_filename num_of_integers random_seed

clean:
	rm -rf $(NAME)

add:
	git add $(SRC) $(HEADER) Makefile
	git commit -m "$(LOGFILE)"
	git push origin master
