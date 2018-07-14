ARCH?=mips

TOPDIR=${PWD}

CFLAGS := -Wall -Wno-unused-variable
#CFLAGS += -Wno-unused-variable -Wno-unused-but-set-variable -Wno-write-strings
#CFLAGS +=  -Wno-unused-function
#CFLAGS += -I${PWD}


Q=@

ifeq ($(ARCH), arm)
CROSS_COMPILE=/opt/allwiner_compiler/external-toolchain/bin/arm-linux-gnueabi-
LIBS_PATH=/opt/yoko_libs

else

ifeq ($(ARCH), mips)
CROSS_COMPILE=/opt/ingenic_compiler/mips-gcc472-glibc216/bin/mips-linux-gnu-
LIBS_PATH=/opt/yoko_libs/ingenic

#libz
#LDFLAGS += -lz -L${LIBS_PATH}/zlib/lib

else
$(error "Unknow ARCH")
endif #mips
endif #arm

CXX=${CROSS_COMPILE}g++
CC=${CROSS_COMPILE}gcc

#LDFLAGS += -lrt -lpthread

ifdef FULL
CFLAGS_FULL:= -Wextra -Wfloat-equal -save-temps 
else
CFLAGS_FULL:=
endif

src := fbtest.cpp 

src += framebuffer/FrameBuffer.cpp 
src += framebuffer/Frameshow.cpp 
CFLAGS += -I${PWD}/framebuffer

src += misc/misc.cpp 
CFLAGS += -I${PWD}/misc

#LDFLAGS += -static-libgcc -static-libstdc++

#src += misc/yokotime.cpp
# cmd.cpp 

objects=$(patsubst %.cpp,%.o, $(src))
target = lcd_test

.PHONY:all
all:${target}

${target}:$(objects)
	$(Q)$(CXX) $^ ${LDFLAGS} -o $@
	
%.o:%.cpp
	$(Q)$(CXX) -c $^ ${CFLAGS} ${CFLAGS_FULL} -o $@

.PHONY:clean
clean:
	@rm -f  ${target}
	@find -name "*.o" -execdir rm {} \;
