# Makefile to compile Asymptote files
#
# Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
# Created: Mon 23 Dec 2019

LC    = asy
FLAGS = -threads
IMGS  = gedet-showcase.png gerda-events.png

all : $(IMGS)

define build-rule
  $(1).$(2) : $(1).asy lib/gedetplots.asy
	@echo [$(shell pwd)] $(LC) $(FLAGS) $$<
	@eval $(LC) $(FLAGS) $$<
endef

$(foreach f,$(IMGS),$(eval $(call build-rule,$(firstword $(subst ., ,$(f))),$(lastword $(subst ., ,$(f))))))

clean :
	@echo [$(shell pwd)] rm -rf $(IMGS)
	-@rm -rf $(IMGS)

.PHONY : clean
