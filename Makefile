all: main

SRCS := $(wildcard ./*[!main].cpp)
OBJS := $(SRCS:%.cpp=%.o)

run: main
	./main.out < input.in > image.ppm
	eog image.ppm &

main: main.cpp $(OBJS)
	g++ main.cpp $(OBJS) -o main.out

clean:
	-rm -f *.o main.out
