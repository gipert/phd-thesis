DIRS = $(sort $(dir $(wildcard */Makefile)))

all: $(DIRS)

$(DIRS): FORCE_MAKE
	@cd $@ && make; cd ..

clean:
	@for d in $(DIRS); do \
	    cd $$d && make clean; cd ..; \
	done

.PHONY: clean FORCE_MAKE
