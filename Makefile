# Makefile
# author: David Černý (xcerny74)
LOGIN1 = xcerny74
LOGIN2 = xdoros01
VARIANT = 2

.PHONY: clean pack doxygen run build

build:
	mkdir -p build
	cd build && cmake ../src && make
	cp -r src/TextureProcessor/nodes build/TextureProcessor/nodes

run: build
	cd ./build/TextureProcessor && ./TextureProcessor

clean:
	rm -rf build
	rm -rf doc/*

pack: clean
	rm -rf doc
	mkdir -p doc
	zip -r $(VARIANT)-$(LOGIN1)-$(LOGIN2).zip Doxyfile Makefile README.txt src doc examples images

doxygen:
	doxygen Doxyfile
