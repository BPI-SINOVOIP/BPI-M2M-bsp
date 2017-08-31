/*
 * tools/power/cpupower/lib/sysfs.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
/* General */
extern unsigned int sysfs_cpu_exists(unsigned int cpu);

/* CPUfreq */
extern unsigned long sysfs_get_freq_kernel(unsigned int cpu);
extern unsigned long sysfs_get_freq_hardware(unsigned int cpu);
extern unsigned long sysfs_get_freq_transition_latency(unsigned int cpu);
extern int sysfs_get_freq_hardware_limits(unsigned int cpu,
					unsigned long *min, unsigned long *max);
extern char *sysfs_get_freq_driver(unsigned int cpu);
extern struct cpufreq_policy *sysfs_get_freq_policy(unsigned int cpu);
extern struct cpufreq_available_governors *sysfs_get_freq_available_governors(
	unsigned int cpu);
extern struct cpufreq_available_frequencies *sysfs_get_available_frequencies(
	unsigned int cpu);
extern struct cpufreq_affected_cpus *sysfs_get_freq_affected_cpus(
	unsigned int cpu);
extern struct cpufreq_affected_cpus *sysfs_get_freq_related_cpus(
	unsigned int cpu);
extern struct cpufreq_stats *sysfs_get_freq_stats(unsigned int cpu,
						unsigned long long *total_time);
extern unsigned long sysfs_get_freq_transitions(unsigned int cpu);
extern int sysfs_set_freq_policy(unsigned int cpu,
				struct cpufreq_policy *policy);
extern int sysfs_modify_freq_policy_min(unsigned int cpu,
					unsigned long min_freq);
extern int sysfs_modify_freq_policy_max(unsigned int cpu,
					unsigned long max_freq);
extern int sysfs_modify_freq_policy_governor(unsigned int cpu, char *governor);
extern int sysfs_set_frequency(unsigned int cpu,
			unsigned long target_frequency);
