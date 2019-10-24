HUFFNAME=huff
UNHUFFNAME=unhuff

# add -O3 later
CFLAG= -Werror -Wall -std=c99 -std=gnu99 -g

HUFF_MAIN=mycode/huff.c
UNHUFF_MAIN=mycode/unhuff.c
HEADER=mycode/huff.h
TXT=example.txt
INCLUDE=mycode/huff.h

FOLDER=ece368-project2

PDF=ms1.pdf
	#report.pdf

SAMPLE=sample/*

LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

$(HUFFNAME):
	$(CC) $(CFLAG) $(HUFF_MAIN) -o $(HUFFNAME) -I mycode
test:
	./$(HUFFNAME) $(TXT)

test1:
	./$(UNHUFFNAME) $(TXT).huff

$(UNHUFFNAME):
	$(CC) $(CFLAG) $(UNHUFF_MAIN) -o $(UNHUFFNAME) -I mycode

clean:
	rm -rf $(HUFFNAME) $(UNHUFFNAME) $(FOLDER) *.huff.*

add:
	git add $(HUFF_MAIN) $(UNHUFF_MAIN) $(PDF) $(SAMPLE) Makefile $(INCLUDE) $(TXT)
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj2ms1 ms1.pdf

finalsubmit:
	mkdir $(FOLDER)
	cp $(HUFF_MAIN) $(UNHUFF_MAIN) $(HEADER) $(FOLDER) 
	#cp $(PDF) $(FOLDER)
	make $(HUFFNAME)
	make $(UNHUFFNAME)
	mv $(HUFFNAME) $(UNHUFFNAME) $(FOLDER)
	turnin -c ece368 -p proj2 $(FOLDER)
