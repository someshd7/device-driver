obj.m := hello.o


KERNELDIR=lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default=
	$(MAKE) -C $(KERNELDIR) M=$(pwd) modules


insatall:
	$(MAKE) -C $(KERNELDIR) M=$(pwd) modules_install

clean:
	$(MAKE) -c $(KERNELDIR) M=$(pwd) clean



