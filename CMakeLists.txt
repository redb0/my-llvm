cmake_minimum_required(VERSION 2.8)

project(my_llvm)

set(LLVM_INCLUDE_DIR ${LLVM_INCLUDE_DIR} CACHE PATH "YOUR LLVM INCLUDE DIRECTORY")
set(LLVM_LIB_DIR ${LLVM_LIB_DIR} CACHE PATH "YOUR LLVM libraries DIRECTORY")
set(CURSES_LLVM_LIBS
    LLVMDlltoolDriver LLVMObjectYAML LLVMCoverage
    LLVMLTO LLVMPasses LLVMObjCARCOpts LLVMLineEditor
    LLVMCoroutines LLVMLibDriver LLVMOption
    LLVMTableGen LLVMMCJIT LLVMMIRParser LLVMOrcJIT
    LLVMXCoreDisassembler LLVMXCoreCodeGen
    LLVMXCoreDesc LLVMXCoreInfo LLVMXCoreAsmPrinter
    LLVMX86Disassembler LLVMX86AsmParser
    LLVMX86CodeGen LLVMX86Desc LLVMX86Info
    LLVMX86AsmPrinter LLVMX86Utils LLVMSystemZDisassembler
    LLVMSystemZCodeGen LLVMSystemZAsmParser
    LLVMSystemZDesc LLVMSystemZInfo LLVMSystemZAsmPrinter
    LLVMSparcDisassembler LLVMSparcCodeGen
    LLVMSparcAsmParser LLVMSparcDesc LLVMSparcInfo
    LLVMSparcAsmPrinter LLVMPowerPCDisassembler
    LLVMPowerPCCodeGen LLVMPowerPCAsmParser
    LLVMPowerPCDesc LLVMPowerPCInfo LLVMPowerPCAsmPrinter
    LLVMNVPTXCodeGen LLVMNVPTXDesc LLVMNVPTXInfo
    LLVMNVPTXAsmPrinter LLVMMSP430CodeGen LLVMMSP430Desc
    LLVMMSP430Info LLVMMSP430AsmPrinter LLVMMipsDisassembler
    LLVMMipsCodeGen LLVMMipsAsmParser LLVMMipsDesc
    LLVMMipsInfo LLVMMipsAsmPrinter LLVMLanaiDisassembler
    LLVMLanaiCodeGen LLVMLanaiAsmParser LLVMLanaiDesc
    LLVMLanaiAsmPrinter LLVMLanaiInfo LLVMHexagonDisassembler
    LLVMHexagonCodeGen LLVMHexagonAsmParser LLVMHexagonDesc
    LLVMHexagonInfo LLVMBPFDisassembler LLVMBPFCodeGen
    LLVMBPFDesc LLVMBPFInfo LLVMBPFAsmPrinter
    LLVMARMDisassembler LLVMARMCodeGen LLVMARMAsmParser
    LLVMARMDesc LLVMARMInfo LLVMARMAsmPrinter
    LLVMAMDGPUDisassembler LLVMAMDGPUCodeGen
    LLVMipo LLVMInstrumentation LLVMVectorize
    LLVMLinker LLVMIRReader LLVMAsmParser
    LLVMAMDGPUAsmParser LLVMAMDGPUDesc LLVMAMDGPUInfo
    LLVMAMDGPUAsmPrinter LLVMAMDGPUUtils
    LLVMAArch64Disassembler LLVMMCDisassembler
    LLVMAArch64CodeGen LLVMGlobalISel LLVMSelectionDAG
    LLVMAsmPrinter LLVMAArch64AsmParser LLVMAArch64Desc
    LLVMAArch64Info LLVMAArch64AsmPrinter
    LLVMAArch64Utils LLVMSymbolize LLVMDebugInfoPDB
    LLVMDebugInfoCodeView LLVMDebugInfoMSF
    LLVMDebugInfoDWARF LLVMInterpreter
    LLVMExecutionEngine LLVMRuntimeDyld
    LLVMCodeGen LLVMTarget LLVMScalarOpts
    LLVMInstCombine LLVMTransformUtils LLVMBitWriter
    LLVMAnalysis LLVMProfileData LLVMObject LLVMMCParser
    LLVMMC LLVMBitReader LLVMCore LLVMBinaryFormat
    LLVMSupport LLVMDemangle
)

INCLUDE_DIRECTORIES(${LLVM_INCLUDE_DIR})
LINK_DIRECTORIES(${LLVM_LIB_DIR} )

set(SOURCES
    main.cpp
    tokenizer.cpp
    Lexer.cpp
    ast.cpp
    parser.cpp
    codegen.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

#target_link_libraries(${PROJECT_NAME}  )



target_link_libraries(${PROJECT_NAME}
    ${CURSES_LLVM_LIBS}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

