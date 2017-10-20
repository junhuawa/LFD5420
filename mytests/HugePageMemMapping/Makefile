### Automatic Makefile generation by 'genmake' script        ####
### Copyright, Jerry Cooperstein, coop@linuxfoundation.org 2/2003 - 1/2017 ####
### License: GPLv2 ###

all:  userprogs ShareMemory

userprogs:
	@$(MAKE) \
		CFLAGS="-O2 -Wall -pthread" \
	 HugePageMemMapping 

ShareMemory:
	@$(MAKE) \
		CFLAGS="-O2 -Wall -pthread" \
	 ShareMemory

clean:
	rm -rf  HugePageMemMapping ShareMemory 
