# Makefile to compile Asymptote files
#
# Author: Luigi Pertoldi - luigi.pertoldi@pd.infn.it
# Created: Mon 23 Dec 2019

LC    = asy
FLAGS = -threads
IMGS  = phIIp-array-2D.pdf phII-array-2D.pdf phII-array.png phII-array-calib.png phIIp-array.png \
        BEGe.png InvCoax.png SemiCoax.png gerda-events.png bb-event.png

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
