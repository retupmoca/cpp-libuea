CXXFLAGS = -Iinclude -O2 -g -std=c++2a \
		   -Wall -Wextra -Wconversion -Wextra-semi -Wold-style-cast -Wnon-virtual-dtor -pedantic -pedantic-errors \

PREFIX = /usr/local
DESTDIR = ""
LIBDIR = lib

OBJECTS = src/queue.o

.PHONY : all
all : lib/libuea.a

# just rebuild everything on a header change, since we aren't tracking
# dependencies for each .cpp file
**/*.o : include/**/*.hpp

lib/libuea.a : $(OBJECTS)
	$(AR) rvs lib/libuea.a $(OBJECTS)

.PHONY : install
install :
	mkdir -p ${DESTDIR}/${PREFIX}/${LIBDIR}
	cp lib/*a ${DESTDIR}/${PREFIX}/${LIBDIR}
	mkdir -p ${DESTDIR}/${PREFIX}/include
	cp -r include/* ${DESTDIR}/${PREFIX}/include

.PHONY : clean
clean :
	rm -f src/*o lib/*a
