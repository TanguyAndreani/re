snes-controller: snes-controller.c
	cc -lxdo -I/usr/local/include -L/usr/local/lib $< -o $@

.PHONY: clean
clean:
	rm -f snes-controller