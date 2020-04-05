CC = g++
pkgs = xcb
CFLAGS = `pkg-config --cflags $(pkgs)` -fpermissive -std=c++17
LIBS = `pkg-config --libs $(pkgs)`

.PHONY: all
all: simpletest

xcbhelper.so: src/xcbhelper.o
	$(CC) $(CFLAGS) $(LIBS) -shared -Wl,-soname,xcbhelper.so -o $@ $^

simpletest: test/simpletest.o xcbhelper.so
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^ -L. -Wl,-R -Wl,.
	
.PHONY: clean
clean:
	rm -f *.o */*.o xcbhelper.so simpletest
