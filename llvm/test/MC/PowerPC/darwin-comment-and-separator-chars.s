# RUN: llvm-mc -triple powerpc-darwin --show-encoding %s | \
# RUN:         FileCheck -check-prefix=CHECK %s
# RUN: llvm-mc -triple powerpc64-darwin --show-encoding %s | \
# RUN:         FileCheck -check-prefix=CHECK %s

# This tests that we accept the in-line comment and separator chars that
# were used by cctools for compatibility with existing code.

# Passing thus far shows that '#' is OK for a line start comment.

# Here's a function on one line with delimiters.

	.globl _foo @ _foo: @ addi r3,r3,1 @ blr
# CHECK: blr ; encoding: [0x4e,0x80,0x00,0x20]

# Here's a use for the in-line comment marker.
_bar:
	addi r3,r3,1 ; this is OK for an in-line comment
# CHECK: addi r3, r3, 1 ; encoding: [0x38,0x63,0x00,0x01]
	blr
