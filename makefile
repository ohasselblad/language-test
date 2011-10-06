# *****************************************************
# Parameters to control Makefile operation
# Configured for my computer -- we should learn a more general way to make makefiles
vpath %.cpp src
vpath %.h include

CXX = g++-mp-4.4
CXXFLAGS = -I include

OBJECTS = functions.o wordData.o Flashcard.o listDicts.o testResults.o Hint.o cmdLineInput.o
HEADERS = functions.h wordData.h Flashcard.h listDicts.h testResults.h Hint.h cmdLineInput.h
# ****************************************************

mkcln: ltest clean

ltest: LanguageTest.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o ltest

LanguageTest.o: LanguageTest.cpp

$(OBJECTS): %.o: %.cpp
	$(CXX) -I include -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJECTS) LanguageTest.o
