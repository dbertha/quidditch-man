SRC = src/SRD
LATEX = pdflatex -interaction=batchmode

all : SRD.pdf
	mv SRD.pdf pdf/SRD.pdf
	make clean
SRD.pdf : ${SRC}.tex
	${LATEX} $<
	#compilation du glossaire
	makeindex -s SRD.ist -t SRD.glg -o SRD.gls SRD.glo
	#compilation de l'index
	makeindex SRD.idx
	#intégration de l'index et du glossaire
	${LATEX} $<
	${LATEX} $<
	
clean:
	@rm -f *.log *.aux *.dvi *.toc *.lot *.lof *.out *.ist *.idx *.gls *.glo *.glg *.ilg *.ind
