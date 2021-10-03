libvfoknob.a: libvfoknob.o
	ar rcs $@ $^

libvfoknob.o: vfo_knob_lib.c vfo_knob_lib.h
	gcc -g -c -o $@ $<

install: libvfoknob.a
	mkdir -p /usr/lib/libvfoknob
	cp libvfoknob.a /usr/lib/libvfoknob
	cp libvfoknob.o /usr/lib/libvfoknob
	mkdir -p /usr/include/libvfoknob
	cp vfo_knob_lib.h /usr/include/libvfoknob

clean:
	rm -f /usr/lib/libvfoknob/*
	rm -f /usr/include/libvfoknob/*
	rm -f *.o *.a $(TARGET)