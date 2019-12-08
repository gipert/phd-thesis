# Makefile to compile all .tex file in the current directory (and recursively)
# with latexmk. Supports parallel compilation and TEX compiler directive
#
# Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
# Created: Sun 1 Dec 2019, flying to Seattle

LC    = latexmk
LOGS  = .auxtex
FLAGS = -quiet -outdir=$(LOGS) -M -MP -MF $(LOGS)/$*.deps
SRCS  = $(wildcard *.tex)
PDFS  = $(patsubst %.tex, $(LOGS)/%.pdf, $(SRCS))

all : $(PDFS)

$(LOGS)/%.pdf : %.tex $(LOGS) FORCE_MAKE
	@if [[ $$(head -n1 $*.tex | grep -oh "\w*tex\w*") == 'xelatex' ]]; then \
	    echo $(LC) $(FLAGS) -pdfxe -xelatex=\"xelatex -halt-on-error\" $<; \
	    eval $(LC) $(FLAGS) -pdfxe -xelatex=\"xelatex -halt-on-error\" $< &> /dev/null; \
	    exit $$?; \
	elif [[ $$(head -n1 $*.tex | grep -oh "\w*tex\w*") == 'lualatex' ]]; then \
	    echo $(LC) $(FLAGS) -pdflua -lualatex=\"lualatex -halt-on-error\" $<; \
	    eval $(LC) $(FLAGS) -pdflua -lualatex=\"lualatex -halt-on-error\" $< &> /dev/null; \
	    exit $$?; \
	else \
	    echo $(LC) $(FLAGS) -pdf -pdflatex=\"pdflatex -halt-on-error\" $<; \
	    eval $(LC) $(FLAGS) -pdf -pdflatex=\'pdflatex -halt-on-error\' $< &> /dev/null; \
	    exit $$?; \
	fi
	@cp $@ .

$(LOGS) :
	mkdir -p $(LOGS)

clean :
	-rm -f $(patsubst %.tex, %.pdf, $(SRCS))
	-rm -rf $(LOGS)

.PHONY : FORCE_MAKE clean
-include $(LOGS)/*.deps
