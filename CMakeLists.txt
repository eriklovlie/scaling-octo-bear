set(LLVM_LINK_COMPONENTS support)
set(LLVM_USED_LIBS clangTooling clangBasic clangAST)

add_clang_executable(complexity-check
  ComplexityCheck.cpp
  )
target_link_libraries(complexity-check
  clangTooling
  clangBasic
  clangASTMatchers
  )

