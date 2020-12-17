CXXFLAGS = -I3rdparty/fmt/include -Iinclude -O2 -g -std=c++2a \
		   -Wall -Wextra -Wconversion -Wextra-semi -Wold-style-cast -Wnon-virtual-dtor \
		   -pedantic -pedantic-errors

PREFIX = /usr/local
DESTDIR = ""
LIBDIR = lib

OBJECTS = src/io.o src/ps.o src/user.o src/string.o

.PHONY : all
all : lib/libuea.a

# just rebuild everything on a header change, since we aren't tracking
# dependencies for each .cpp file
**/*.o : include/**/*.hpp

lib/libuea.a : 3rdparty/fmt/libfmt.a $(OBJECTS)
	pushd 3rdparty/fmt/ && $(AR) x libfmt.a && popd
	$(AR) rvs lib/libuea.a $(OBJECTS) 3rdparty/fmt/*.o
	pushd 3rdparty/fmt/ && rm *.o && popd

3rdparty/fmt/libfmt.a :
	pushd 3rdparty/fmt/ && cmake . && $(MAKE) fmt && popd

.PHONY : install
install :
	mkdir -p ${DESTDIR}/${PREFIX}/${LIBDIR}
	cp lib/*a ${DESTDIR}/${PREFIX}/${LIBDIR}
	mkdir -p ${DESTDIR}/${PREFIX}/include
	cp -r include/* ${DESTDIR}/${PREFIX}/include

.PHONY : clean
clean :
	rm -f src/*o lib/*a
	pushd 3rdparty/fmt && make clean && popd
