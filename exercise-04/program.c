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

struct syscall_event_t {
	__u64 timestamp;
	__u64 cpu;
	__u32 pid;
	__u32 syscall_id;
};

struct bpf_map_def SEC("maps/pids_to_watch") pids_to_watch = {
	.type = BPF_MAP_TYPE_HASH,
	.key_size = sizeof(__u32),
	.value_size = sizeof(__u32),
	.max_entries = 16,
};

struct bpf_map_def SEC("maps/syscall_event") syscall_event = {
	.type = BPF_MAP_TYPE_PERF_EVENT_ARRAY,
	.key_size = sizeof(int),
	.value_size = sizeof(__u32),
	.max_entries = 1024,
};

// arg list generated with
// https://github.com/iovisor/bcc/blob/master/tools/tplist.py
struct sys_enter_args {
	unsigned long long unused; // syscall preemble
	long id;
	unsigned long args[6];
};

SEC("tracepoint/raw_syscalls/sys_enter")
int tracepoint__raw_sys_enter(struct sys_enter_args *ctx)
{
	u64 pid = bpf_get_current_pid_tgid();
	u32 cpu = bpf_get_smp_processor_id();
	u32 tgid = pid >> 32;
	u32 syscall_id = ctx->id;
	u32 *exists = NULL;

	exists = bpf_map_lookup_elem(&pids_to_watch, &tgid);
	if (exists == NULL || !*exists) {
		return 0;
	}

	/* printt("tracepoint pid: %u syscall: %u\n", tgid, syscall_id); */

	struct syscall_event_t evt = {
		.timestamp = bpf_ktime_get_ns(),
		.cpu = cpu,
		.pid = pid,
		.syscall_id = syscall_id,
	};

	bpf_perf_event_output(ctx, &syscall_event, cpu, &evt, sizeof(evt));

	return 0;
}

char _license[] SEC("license") = "GPL";
