CXX ?= g++


server : main.cpp


clean:
	rm -rf build
	rm -rf PIGG_output


.PHONY: clean install all tar
# 在Makefile中，.PHONY后面的target表示的也是一个伪造的target, 而不是真实存在的文件target，注意Makefile的target默认是文件。