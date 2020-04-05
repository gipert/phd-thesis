DIRS = $(sort $(dir $(wildcard */Makefile))) \
       $(sort $(dir $(wildcard */*/Makefile))) \
       $(sort $(dir $(wildcard */*/*/Makefile))) \
       $(sort $(dir $(wildcard */*/*/*/Makefile))) \
       $(sort $(dir $(wildcard */*/*/*/*/Makefile)))
PWD = $(shell pwd)

all: $(DIRS)

$(DIRS): FORCE_MAKE
	@+cd $@ && make; cd $(PWD)

clean:
	@+for d in $(DIRS); do \
	    cd $$d && make clean; cd $(PWD); \
	done

.PHONY: clean FORCE_MAKE
