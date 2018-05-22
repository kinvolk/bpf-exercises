Why does the verifier reject a program with only `BPF_EXIT_INSN()`?

Fix the program to pass the verification and return value `1`. For that you
need a second instruction `BPF_MOV64_IMM` to move an immediate value
into a target register. We know that register 0 holds the exit value for
an eBPF program, i.e. here the target register is `BPF_REG_0`.
