LINUX_HEADERS?=$(shell ls /var/lib/dpkg/info/linux-headers-*.list | head -n 1 | sed -rn 's/.*(linux-headers-.*-generic).list/\/usr\/src\/\1/p')

build:
	clang \
		-D__KERNEL__ \
		$(foreach path,$(LINUX_HEADERS), \
		-I $(path)/include/ \
		-I $(path)/include/uapi \
		-I $(path)/include/generated/uapi \
		-I $(path)/arch/x86/include \
		-I $(path)/arch/x86/include/generated \
		-I $(path)/arch/x86/include/uapi )\
		-O2 -emit-llvm -c program.c \
		-o - | \
		llc -march=bpf -filetype=obj -o program.o

.PHONY: build
