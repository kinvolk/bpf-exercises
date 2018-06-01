#include <linux/kconfig.h>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#include <linux/bpf.h>
#pragma clang diagnostic pop

#include "../include/bpf_helpers.h"

#define printt(fmt, ...)                                                   \
	({                                                                 \
		char ____fmt[] = fmt;                                      \
		bpf_trace_printk(____fmt, sizeof(____fmt), ##__VA_ARGS__); \
	})

SEC("sched_cls/demo")
int demo(struct __sk_buff *skb)
{
	printt("Hello from eBPF land!\n");

	return 0;
}

char _license[] SEC("license") = "GPL";
