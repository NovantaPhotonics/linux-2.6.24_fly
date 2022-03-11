#ifndef __ASM_OFFSETS_H__
#define __ASM_OFFSETS_H__
/*
 * DO NOT MODIFY.
 *
 * This file was generated by Kbuild
 *
 */

#define THREAD 480 /* offsetof(struct task_struct, thread)	 # */
#define THREAD_INFO 4 /* offsetof(struct task_struct, stack)	 # */
#define MM 160 /* offsetof(struct task_struct, mm)	 # */
#define PTRACE 16 /* offsetof(struct task_struct, ptrace)	 # */
#define KSP 0 /* offsetof(struct thread_struct, ksp)	 # */
#define PGDIR 12 /* offsetof(struct thread_struct, pgdir)	 # */
#define PT_REGS 4 /* offsetof(struct thread_struct, regs)	 # */
#define THREAD_FPEXC_MODE 288 /* offsetof(struct thread_struct, fpexc_mode)	 # */
#define THREAD_FPR0 24 /* offsetof(struct thread_struct, fpr[0])	 # */
#define THREAD_FPSCR 280 /* offsetof(struct thread_struct, fpscr)	 # */
#define THREAD_DBCR0 16 /* offsetof(struct thread_struct, dbcr0)	 # */
#define PT_PTRACED 1 /* PT_PTRACED	 # */
#define STACK_FRAME_OVERHEAD 16 /* STACK_FRAME_OVERHEAD	 # */
#define INT_FRAME_SIZE 192 /* STACK_FRAME_OVERHEAD + sizeof(struct pt_regs)	 # */
#define GPR0 16 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[0])	 # */
#define GPR1 20 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[1])	 # */
#define GPR2 24 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[2])	 # */
#define GPR3 28 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[3])	 # */
#define GPR4 32 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[4])	 # */
#define GPR5 36 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[5])	 # */
#define GPR6 40 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[6])	 # */
#define GPR7 44 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[7])	 # */
#define GPR8 48 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[8])	 # */
#define GPR9 52 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[9])	 # */
#define GPR10 56 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[10])	 # */
#define GPR11 60 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[11])	 # */
#define GPR12 64 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[12])	 # */
#define GPR13 68 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[13])	 # */
#define GPR14 72 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[14])	 # */
#define GPR15 76 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[15])	 # */
#define GPR16 80 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[16])	 # */
#define GPR17 84 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[17])	 # */
#define GPR18 88 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[18])	 # */
#define GPR19 92 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[19])	 # */
#define GPR20 96 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[20])	 # */
#define GPR21 100 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[21])	 # */
#define GPR22 104 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[22])	 # */
#define GPR23 108 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[23])	 # */
#define GPR24 112 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[24])	 # */
#define GPR25 116 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[25])	 # */
#define GPR26 120 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[26])	 # */
#define GPR27 124 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[27])	 # */
#define GPR28 128 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[28])	 # */
#define GPR29 132 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[29])	 # */
#define GPR30 136 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[30])	 # */
#define GPR31 140 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, gpr[31])	 # */
#define _NIP 144 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, nip)	 # */
#define _MSR 148 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, msr)	 # */
#define _CTR 156 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, ctr)	 # */
#define _LINK 160 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, link)	 # */
#define _CCR 168 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, ccr)	 # */
#define _MQ 172 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, mq)	 # */
#define _XER 164 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, xer)	 # */
#define _DAR 180 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, dar)	 # */
#define _DSISR 184 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, dsisr)	 # */
#define _DEAR 180 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, dar)	 # */
#define _ESR 184 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, dsisr)	 # */
#define ORIG_GPR3 152 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, orig_gpr3)	 # */
#define RESULT 188 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, result)	 # */
#define TRAP 176 /* STACK_FRAME_OVERHEAD+offsetof(struct pt_regs, trap)	 # */
#define CLONE_VM 256 /* CLONE_VM	 # */
#define CLONE_UNTRACED 8388608 /* CLONE_UNTRACED	 # */
#define MM_PGD 36 /* offsetof(struct mm_struct, pgd)	 # */
#define CPU_SPEC_ENTRY_SIZE 68 /* sizeof(struct cpu_spec)	 # */
#define CPU_SPEC_PVR_MASK 0 /* offsetof(struct cpu_spec, pvr_mask)	 # */
#define CPU_SPEC_PVR_VALUE 4 /* offsetof(struct cpu_spec, pvr_value)	 # */
#define CPU_SPEC_FEATURES 12 /* offsetof(struct cpu_spec, cpu_features)	 # */
#define CPU_SPEC_SETUP 36 /* offsetof(struct cpu_spec, cpu_setup)	 # */
#define TI_TASK 0 /* offsetof(struct thread_info, task)	 # */
#define TI_EXECDOMAIN 4 /* offsetof(struct thread_info, exec_domain)	 # */
#define TI_FLAGS 52 /* offsetof(struct thread_info, flags)	 # */
#define TI_LOCAL_FLAGS 48 /* offsetof(struct thread_info, local_flags)	 # */
#define TI_CPU 8 /* offsetof(struct thread_info, cpu)	 # */
#define TI_PREEMPT 12 /* offsetof(struct thread_info, preempt_count)	 # */
#define pbe_address 0 /* offsetof(struct pbe, address)	 # */
#define pbe_orig_address 4 /* offsetof(struct pbe, orig_address)	 # */
#define pbe_next 8 /* offsetof(struct pbe, next)	 # */
#define TASK_SIZE -2147483648 /* TASK_SIZE	 # */
#define NUM_USER_SEGMENTS 8 /* TASK_SIZE>>28	 # */
#define CFG_TB_ORIG_STAMP 0 /* offsetof(struct vdso_data, tb_orig_stamp)	 # */
#define CFG_TB_TICKS_PER_SEC 8 /* offsetof(struct vdso_data, tb_ticks_per_sec)	 # */
#define CFG_TB_TO_XS 16 /* offsetof(struct vdso_data, tb_to_xs)	 # */
#define CFG_STAMP_XSEC 24 /* offsetof(struct vdso_data, stamp_xsec)	 # */
#define CFG_TB_UPDATE_COUNT 32 /* offsetof(struct vdso_data, tb_update_count)	 # */
#define CFG_TZ_MINUTEWEST 36 /* offsetof(struct vdso_data, tz_minuteswest)	 # */
#define CFG_TZ_DSTTIME 40 /* offsetof(struct vdso_data, tz_dsttime)	 # */
#define CFG_SYSCALL_MAP32 52 /* offsetof(struct vdso_data, syscall_map_32)	 # */
#define WTOM_CLOCK_SEC 44 /* offsetof(struct vdso_data, wtom_clock_sec)	 # */
#define WTOM_CLOCK_NSEC 48 /* offsetof(struct vdso_data, wtom_clock_nsec)	 # */
#define TVAL32_TV_SEC 0 /* offsetof(struct timeval, tv_sec)	 # */
#define TVAL32_TV_USEC 4 /* offsetof(struct timeval, tv_usec)	 # */
#define TSPEC32_TV_SEC 0 /* offsetof(struct timespec, tv_sec)	 # */
#define TSPEC32_TV_NSEC 4 /* offsetof(struct timespec, tv_nsec)	 # */
#define TZONE_TZ_MINWEST 0 /* offsetof(struct timezone, tz_minuteswest)	 # */
#define TZONE_TZ_DSTTIME 4 /* offsetof(struct timezone, tz_dsttime)	 # */
#define CLOCK_REALTIME 0 /* CLOCK_REALTIME	 # */
#define CLOCK_MONOTONIC 1 /* CLOCK_MONOTONIC	 # */
#define NSEC_PER_SEC 1000000000 /* NSEC_PER_SEC	 # */
#define CLOCK_REALTIME_RES 4000000 /* TICK_NSEC	 # */

#endif
