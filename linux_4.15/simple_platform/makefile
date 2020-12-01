KERN_DIR = /home/xuzhiyin/am8268-onebit/am12xx/sdk/linux/
all:
			make -C $(KERN_DIR) M=`pwd` modules
clean:
			make -C $(KERN_DIR) M=`pwd` modules clean
					rm -rf modules.order
					obj-m    += s_i2c.o

