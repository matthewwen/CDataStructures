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

all: 
	make $(HUFFNAME) $(UNHUFFNAME)

$(HUFFNAME):
	$(CC) $(CFLAG) $(HUFF_MAIN) -o $(HUFFNAME) -I mycode
t:
	./$(HUFFNAME) example.txt 
	./$(UNHUFFNAME) example.txt.huff
	diff example.txt example.txt.huff.unhuff

test:
	./$(HUFFNAME) input/text0.txt
	./$(UNHUFFNAME) input/text0.txt.huff
	diff input/text0.txt input/text0.txt.huff.unhuff

test1:
	./$(HUFFNAME) input/text1.txt
	./$(UNHUFFNAME) input/text1.txt.huff
	diff input/text1.txt input/text1.txt.huff.unhuff

test2:
	./$(HUFFNAME) input/text2.txt
	./$(UNHUFFNAME) input/text2.txt.huff
	diff input/text2.txt input/text2.txt.huff.unhuff

test3:
	./$(HUFFNAME) input/text3.txt
	./$(UNHUFFNAME) input/text3.txt.huff
	diff input/text3.txt input/text3.txt.huff.unhuff

test4:
	./$(HUFFNAME) input/text4.txt
	./$(UNHUFFNAME) input/text4.txt.huff
	diff input/text4.txt input/text4.txt.huff.unhuff

test5:
	./$(HUFFNAME) input/text5.txt
	./$(UNHUFFNAME) input/text5.txt.huff
	diff input/text5.txt input/text5.txt.huff.unhuff

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
