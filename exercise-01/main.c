#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#pragma clang diagnostic ignored "-Waddress-of-packed-member"
#include <linux/kconfig.h>
#include <linux/bpf.h>
#pragma clang diagnostic pop
#include <linux/version.h>

#include "../include/raw_insn.h"

int main() {
	int fd, ret;
	const char *license = "GPL";
	char log_buf[16384];

	// FIXME
	struct bpf_insn prog[] = {
		BPF_EXIT_INSN(),
	};

	union bpf_attr attr = {
		.prog_type = BPF_PROG_TYPE_SCHED_CLS,
		.insn_cnt = sizeof(prog) / sizeof(struct bpf_insn),
		.insns = (__u64) (unsigned long) prog,
		.log_buf = (__u64) (unsigned long) log_buf,
		.log_size = sizeof(log_buf),
		.log_level = 2,
		.kern_version = LINUX_VERSION_CODE,
		.license = (__u64) (unsigned long) license,
	};

	fd = syscall(__NR_bpf, BPF_PROG_LOAD, &attr, sizeof(attr));
	printf("--------------------------------------\n");
	printf("verifier log:\n");
	printf("%s\n", log_buf);
	printf("--------------------------------------\n");
	if (fd < 0) {
		fprintf(stderr, "prog load failed: %d\n", errno);
		return 1;
	}
	printf("prog fd: %d\n", fd);

	char data[14];
	union bpf_attr attr_test_run = {
		.test.data_in = (__u64) (unsigned long) data,
		.test.data_size_in = sizeof(data),
		.test.prog_fd = fd,
		.test.repeat = 1,
	};

	ret = syscall(__NR_bpf, BPF_PROG_TEST_RUN, &attr_test_run, sizeof(attr_test_run));
	if (ret < 0) {
		fprintf(stderr, "prog test run failed: %d\n", errno);
		return 1;
	}
	printf("prog return value: %d\n", attr_test_run.test.retval);

	return ret;
}
