BOF_Function := servicemove
CC_x64 := x86_64-w64-mingw32-gcc
all:
	$(CC_x64) -o $(BOF_Function).x64.o -c $(BOF_Function).c

clean:
	rm $(BOF_Function).o
