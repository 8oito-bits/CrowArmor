#include <linux/preempt.h>
#include <asm/cpufeatures.h>
#include <asm/cpuid.h>
#include <asm/processor.h>
#include <linux/smp.h>
#include <asm/vmx.h>
#include "hyperv.h"

static bool __hyperv_is_vmx_supported(void)
{
	int cpu = smp_processor_id();

	if (!(cpuid_ecx(1) << 5)) {
		pr_err("crowarmor: VMX not supported by CPU %d\n", cpu);
		return false;
	}

	if (!this_cpu_has(X86_FEATURE_MSR_IA32_FEAT_CTL) ||
	    !this_cpu_has(X86_FEATURE_VMX)) {
		pr_err("crowarmor: VMX not enabled (by BIOS) in MSR_IA32_FEAT_CTL on CPU %d\n", cpu);
		return false;
	}

	return true;
}

bool hyperv_is_vmx_supported(void)
{
	bool supported;

	migrate_disable();
	supported = __hyperv_is_vmx_supported();
	migrate_enable();

	return supported;
}
