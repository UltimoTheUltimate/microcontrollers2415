# Copyright 2019: Johann A. Briffa <johann.briffa@um.edu.mt>

SOURCES := $(wildcard *.tex)
TARGETS := design_brief.pdf

# Master targets

all: $(TARGETS)

## Setting targets

FORCE:

.PHONY:	FORCE all clean

.SUFFIXES: # Delete the default suffixes

.DELETE_ON_ERROR:

# Dependencies

$(TARGETS): $(SOURCES)

# Rules for main compilation

%.bbl: %.aux
	-bibtex $*

%.aux: %.tex
	xelatex -interaction=batchmode -no-pdf $*

%.pdf: %.tex %.aux %.bbl
	latexmk -pdfxe -silent $*

clean:
	-/bin/rm -f *.aux *.out *.log *.bbl *.blg *.toc *.fls *.fdb_latexmk *.xdv $(TARGETS)
