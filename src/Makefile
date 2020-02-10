# Makefile to compile all .tex file in the current directory (and recursively)
# with latexmk. Supports parallel compilation and TEX compiler directive
#
# Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
# Created: Sun 1 Dec 2019, flying to Seattle

LC    = latexmk
LOGS  = .auxtex
FLAGS = -pdfxe -pdfxelatex="xelatex -halt-on-error" \
        -outdir=$(LOGS) -M -MP -MF $(LOGS)/$*.deps
PROJ  = main

all : $(PROJ).pdf

$(PROJ).pdf : $(PROJ).tex mkdirs img FORCE_MAKE
	-$(LC) $(FLAGS) $<
	cp $(LOGS)/$@ .

img :
	+cd img && make; cd ..

mkdirs :
	mkdir -p $(LOGS)/chap

clean :
	rm -rf $(PROJ).pdf $(LOGS)

cleanall : clean
	cd img && make clean; cd ..

.PHONY : FORCE_MAKE clean cleanall img mkdirs
-include $(LOGS)/*.deps