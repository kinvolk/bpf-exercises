#include <linux/kconfig.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#include <linux/bpf.h>
#pragma clang diagnostic pop

#include "../include/bpf_helpers.h"

SEC("tracepoint/raw_syscalls/sys_enter")
int tracepoint__raw_sys_enter()
{
	char msg[] = "hello from sys_enter tracepoint\n";
	bpf_trace_printk(msg, sizeof(msg));
	return 0;
}

char _license[] SEC("license") = "GPL";
