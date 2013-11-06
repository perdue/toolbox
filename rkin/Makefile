rkin: rkin.for
#	g77 -m32 -Wno-globals $^ -o $@
	g77 -m64 -Wno-globals $^ -o $@

install:
	@cp -v MASS83A.DAT /usr/local/lib
	@cp -v rkin /usr/local/bin

uninstall:
	@rm -vi /usr/local/lib/MASS83A.DAT
	@rm -vi /usr/local/bin/rkin

clean:
	@rm -vf rkin
