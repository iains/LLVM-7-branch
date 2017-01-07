# RUN: llvm-mc -triple powerpc-darwin --show-encoding %s | \
# RUN:         FileCheck -check-prefix=CHECK %s
# RUN: llvm-mc -triple powerpc64-darwin --show-encoding %s | \
# RUN:         FileCheck -check-prefix=CHECK %s

# Test that we can parse and produce output for quoted symbols as required.

# Needed to support Objective-C
	b "+[someObject someMethod:]"
# CHECK: fixup A - offset: 0, value: +[someObject someMethod:], kind: fixup_ppc_br24

# Needed to support assembly of .s files produced by Apple gcc-4.2.1
	bcl 20, 31, "L000001$pb"
# CHECK: fixup A - offset: 0, value: L000001$pb, kind: fixup_ppc_brcond14
"L000001$pb":
	nop
	blr

# CHECK: blr ; encoding: [0x4e,0x80,0x00,0x20]
