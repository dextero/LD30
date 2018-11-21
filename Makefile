default: build/Makefile
	$(MAKE) -C build

build/Makefile:
	mkdir -p build
	cmake -Bbuild -H.

%:
	$(MAKE) -C build $@

clean:
	rm -rf build
