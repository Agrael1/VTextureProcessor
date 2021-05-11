CXX = g++-10
CXXFLAGS = -std=c++20 -I./src
LDFLAGS = -LQt5Core
LOGIN1 = xcerny74
LOGIN2 = xdoros01
VARIANT = 2
SRC = ./src

.PHONY: clean pack doxygen run build

build:
	mkdir -p build
	cd build && cmake ../src && make
	cp -r src/TextureProcessor/nodes build/TextureProcessor/nodes

run: build
	cd build/TextureProcessor && ./TextureProcessor

clean:
	rm -rf latex html
	# TODO

pack:
	zip -r $(VARIANT)-$(LOGIN1)-$(LOGIN2).zip Doxyfile Makefile README.txt

doxygen:
	doxygen Doxyfile
