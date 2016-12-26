# RUN:llvm-mc -triple=powerpc-apple-darwin -filetype=obj %s | llvm-objdump -s - | \
# RUN:        FileCheck %s --check-prefix=CHECK
# RUN:llvm-mc -triple=powerpc64-apple-darwin -filetype=obj %s | llvm-objdump -s - | \
# RUN:        FileCheck %s --check-prefix=CHECK

# This checks that we correctly accept both '.' and '$' as representing "current
# PC".  In principle, it would be enough to pass parsing, but we might as well
# check for correct output too.

	.text

Lfoo:
	.long 5
	.long .-Lfoo
	.long $-Lfoo
	.long Lfoo-$
	.long Lfoo-.

# CHECK: 00000005 00000004 00000008 fffffff4

