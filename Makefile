all: mmruby_lib mmruby_bin


mmruby_lib:
	cd src ; make all

mmruby_bin:
	cd test ; make all

clean:
	cd src ; make clean
	cd test ; make clean

