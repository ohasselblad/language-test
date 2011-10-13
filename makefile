# *****************************************************
# Parameters to control Makefile operation
# Configured for my computer -- we should learn a more general way to make makefiles
vpath %.cpp src
vpath %.h include


CXX = g++-mp-4.4
CXXFLAGS = -I include

OBJECTS = functions.o Flashcard.o listDicts.o testResults.o Hint.o cmdLineInput.o wordCompare.o SmartPicker.o WordData.o
HEADERS = functions.h Flashcard.h listDicts.h testResults.h Hint.h cmdLineInput.h wordCompare.h SmartPicker.h WordData.h
#PROTO_OBJECTS = functions.o account.o Flashcard.o WordData.o
#PROTO_HEADERS = functions.h account.h Flashcard.h WordData.h

# ****************************************************

mkcln: ltest clean

ltest: LanguageTest.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o ltest

LanguageTest.o: LanguageTest.cpp

$(OBJECTS): %.o: %.cpp
	$(CXX) -I include -c $< -o $@

#proto: main_prototype.o $(PROTO_OBJECTS)
#	$(CXX) $(CXXFLAGS) $^ -o proto
#
#$(PROTO_OBJECTS): %.o: %.cpp
#	$(CXX) -I include -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJECTS) LanguageTest.o
