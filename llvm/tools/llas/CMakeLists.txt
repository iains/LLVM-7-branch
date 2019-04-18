set(LLVM_LINK_COMPONENTS
  AllTargetsAsmPrinters
  AllTargetsAsmParsers
  AllTargetsDescs
  AllTargetsInfos
  MC
  MCParser
  Support
  )

add_llvm_tool(llas
  llas.cpp
  )
