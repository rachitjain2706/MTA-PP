# Steps to create the dataset for PARSEC 3.0 data:

# lscpu: Machine configurations:

# Architecture:                    x86_64
# CPU op-mode(s):                  32-bit, 64-bit
# Byte Order:                      Little Endian
# Address sizes:                   39 bits physical, 48 bits virtual
# CPU(s):                          4
# On-line CPU(s) list:             0-3
# Thread(s) per core:              2
# Core(s) per socket:              2
# Socket(s):                       1
# NUMA node(s):                    1
# Vendor ID:                       GenuineIntel
# CPU family:                      6
# Model:                           78
# Model name:                      Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz
# Stepping:                        3
# CPU MHz:                         1603.411
# CPU max MHz:                     2800.0000
# CPU min MHz:                     400.0000
# BogoMIPS:                        4800.00
# Virtualization:                  VT-x
# L1d cache:                       64 KiB
# L1i cache:                       64 KiB
# L2 cache:                        512 KiB
# L3 cache:                        3 MiB
# NUMA node0 CPU(s):               0-3
# Vulnerability Itlb multihit:     KVM: Mitigation: Split huge pages
# Vulnerability L1tf:              Mitigation; PTE Inversion; VMX conditional cache flushes, SMT vulnerable
# Vulnerability Mds:               Mitigation; Clear CPU buffers; SMT vulnerable
# Vulnerability Meltdown:          Mitigation; PTI
# Vulnerability Spec store bypass: Mitigation; Speculative Store Bypass disabled via prctl and seccomp
# Vulnerability Spectre v1:        Mitigation; usercopy/swapgs barriers and __user pointer sanitization
# Vulnerability Spectre v2:        Mitigation; Full generic retpoline, IBPB conditional, IBRS_FW, STIBP conditional, RSB filling
# Vulnerability Srbds:             Mitigation; Microcode
# Vulnerability Tsx async abort:   Not affected
# Flags:                           fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault invpcid_single pti ssbd ibrs ibpb stibp tpr_shadow vnmi flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid mpx rdseed adx smap clflushopt intel_pt xsaveopt xsavec xgetbv1 xsaves dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp md_clear flush_l1d

# Importing modules
import subprocess, sys, os, time


def main():
    # Defining modules for programs which have been compiled
    modules = ['blackscholes', 'bodytrack', 'canneal', 'fluidanimate', 'freqmine', 'streamcluster', 'swaptions',
               'splash2', 'facesims', 'ferret']
    output_file = open("dataset_parsec.csv", "a")
    output_file.write(
        "Name, Threads, Size, branch-instructions, branch-misses in %, L3-cache-misses, L3-cache-miss-rate in %, L3-cache-references, cpu-cycles, total-instructions, IPC, cpu-clock, page-faults, L1-data-cache-loads, L1-instruction-cache-load-misses, LLC-load-misses, Exe-time, Speedup\n")
    output_file.close()
    for module in modules:
        threads = [1, 2, 4, 8, 16]
        input_size = [1024, 4096, 8192]
        output_file = open("dataset_parsec.csv", "a")
        shell_command = subprocess.Popen('sudo /home/rachit/parsec-3.0/bin/parsecmgmt -a build -p ' + module,
                                         shell=True, stdout=subprocess.PIPE)

        for size in input_size:
            for thread in threads:
                startTime = time.time()
                # Running command twice since perf slows down computation
                # Actual speedup_1 is not captured
                shell_command = subprocess.Popen(
                    'sudo /home/rachit/parsec-3.0/bin/parsecmgmt -a run -p ' + module + ' -n ' + str(
                        thread) + ' -i ' + size, shell=True, stdout=subprocess.PIPE)
                shell_command.wait()
                endTime = time.time()
                speedup_n = endTime - startTime
                if (thread == 1):
                    speedup_1 = speedup_n
                speed_up = speedup_1 / speedup_n
                output_file.write(module + "," + str(thread) + "," + str(size) + ",")
                shell_command = subprocess.Popen(
                    'sudo perf stat -o output_parsec.txt --field-separator=, -e branch-instructions,branch-misses,cache-misses,cache-references,cycles,instructions,cpu-clock,page-faults,L1-dcache-loads,L1-icache-load-misses,LLC-load-misses -- /home/rachit/parsec-3.0/bin/parsecmgmt -a run -p ' + module + ' -n ' + str(
                        thread) + ' -i ' + size, shell=True, stdout=subprocess.PIPE)
                shell_command.wait()
                with open("output_parsec.txt", "r") as f:
                    lines = f.readlines()
                for line in lines[2:]:
                    values = line.split(",")
                    value = values[0]
                    name = values[2]
                    if (name.startswith('branch-misses')):
                        branch_miss_rate = values[5]
                        output_file.write(branch_miss_rate + ",")
                    elif (name.startswith('instructions')):
                        IPC = values[5]
                        output_file.write(value + "," + IPC + ",")
                    elif (name.startswith('cache-misses')):
                        cache_miss_rate = values[5]
                        output_file.write(value + "," + cache_miss_rate + ",")
                    else:
                        output_file.write(value + ",")
                output_file.write(str(speedup_n) + "," + str(speed_up) + "\n")

        output_file.close()


if __name__ == "__main__":
    main()
