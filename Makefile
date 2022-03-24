all:
	@cmake . -B build
	@cmake --build build

clean:
	@rm -rf build
	@rm -rf deus

.PHONY: all clean