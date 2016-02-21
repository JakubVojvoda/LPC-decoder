###################################################
# 
#  Simple LPC decoder
#  by Jakub Vojvoda [vojvoda@swdeveloper.sk]
#  2015
#
###################################################

NAME = decoder

CXX = g++
CXXFLAGS = -c -O2 -pipe -Wall -W

LDFLAGS = -Wl,-O1


all: src/main.o src/decoder.o src/wavfile.o $(NAME)

src/main.o: src/main.cpp 
	$(CXX) $(CXXFLAGS) -o src/main.o src/main.cpp

src/decoder.o: src/decoder.cpp src/decoder.h
	$(CXX) $(CXXFLAGS) -o src/decoder.o src/decoder.cpp

src/wavfile.o: src/wavfile.cpp src/wavfile.h
	$(CXX) $(CXXFLAGS) -o src/wavfile.o src/wavfile.cpp

$(NAME): src/main.o src/decoder.o src/wavfile.o
	$(CXX) $(LDFLAGS) -o $(NAME) src/main.o src/decoder.o src/wavfile.o


clean:
	rm -f $(NAME) src/wavfile.o src/decoder.o src/main.o src/*~