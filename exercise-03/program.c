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

struct bpf_map_def SEC("maps/openat_count") openat_count = {
	.type = BPF_MAP_TYPE_HASH,
	.key_size = sizeof(__u32),
	.value_size = sizeof(__u64),
	.max_entries = 1,
};

SEC("kprobe/SyS_openat")
int kprobe__sys_openat(struct pt_regs *ctx)
{
	char filename[256];
	u64 *count_ptr = NULL;
	u64 current_count = 1, one = 1, zero=0;

	bpf_probe_read(filename, sizeof(filename), (void *)PT_REGS_PARM2(ctx));

	printt("openat(%s)\n", filename);

	count_ptr = bpf_map_lookup_elem(&openat_count, &zero);
	if (count_ptr != NULL) {
		(*count_ptr)++;
		current_count = *count_ptr;
	} else {
		bpf_map_update_elem(&openat_count, &zero, &one, BPF_ANY);
	}

	printt("openat count %lu\n", current_count);

	return 0;
}

char _license[] SEC("license") = "GPL";
// this number will be interpreted by gobpf's elf loader
// to set the current running kernel version
__u32 _version SEC("version") = 0xFFFFFFFE;
