# Makefile to compile all .tex file in the current directory (and recursively)
# with latexmk. Supports parallel compilation and TEX compiler directive
#
# Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
# Created: Sun 1 Dec 2019, flying to Seattle

PROJ  = main
LC    = latexmk
LOGS  = .auxtex
FLAGS = -pdfxe -pdfxelatex="xelatex -halt-on-error" -bibtex \
        -outdir=$(LOGS) -M -MP -MF $(LOGS)/$(PROJ).deps

all : mkdirs | $(PROJ).pdf

$(PROJ).pdf : $(PROJ).tex mkdirs img FORCE_MAKE
	-$(LC) $(FLAGS) $<
	cp $(LOGS)/$@ .
	xdg-open "$@"

img :
	+cd img && make; cd ..

mkdirs :
	mkdir -p $(LOGS)/chap

clean :
	rm -rf $(PROJ).pdf $(LOGS)

cleanall : clean
	+cd img && make clean; cd ..

.PHONY : FORCE_MAKE clean cleanall img mkdirs

-include $(LOGS)/$(PROJ).deps
