HUFFNAME=huff
UNHUFFNAME=unhuff

CFLAG= -Werror -Wall -O3

HUFF_MAIN=mycode/huff.c
UNHUFF_MAIN=mycode/unhuff.c

PDF=
#ms2.pdf \
	report.pdf

SAMPLE=sample/*

LOGFILE=$(LOGPATH)/$(shell date --iso=seconds) 

$(HUFFNAME):
	$(CC) $(CFLAG) $(HUFF_MAIN) -o $(HUFFNAME)

$(UNHUFFNAME):
	$(CC) $(CFLAG) $(UNHUFF_MAIN) -o $(UNHUFFNAME) 

clean:
	rm -rf $(HUFFNAME) $(UNHUFFNAME)

add:
	git add $(HUFF_MAIN) $(UNHUFF_MAIN) $(PDF) $(SAMPLE) Makefile
	git commit -m "$(LOGFILE)"
	git push origin master

ms1submit:
	turnin -c ece368 -p proj2ms1 ms1.pdf

finalsubmit:
	turnin -c ece368 -p proj2 report.pdf
