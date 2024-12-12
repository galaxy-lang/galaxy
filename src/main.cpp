#include <iostream>
#include <exception>
#include <vector>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/CodeGen/CommandFlags.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/Error.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/Support/CodeGen.h>

#include "backend/generator/generate_ir.hpp"
#include "backend/generator/symbols/symbol_stack.hpp"

extern "C" {
    #include "frontend/lexer/core.h"
    #include "utils.h"
    #include "frontend/lexer/freeTokens.h"
    #include "frontend/ast/definitions.h"
    #include "frontend/parser/core.h"
    #include "frontend/parser/printer/print_ast.h"
    #include "frontend/getTokenTypeName.h"
    #include "frontend/freeTokens.h"
    #include "args/definitions.h"
}


/**
 * @brief Entry point for the program.
 *
 * Processes a source file by tokenizing its contents, generating an abstract syntax tree (AST),
 * and cleaning up allocated resources.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line arguments: program name and source file path.
 * @return 0 on success, or a non-zero error code on failure.
 */
int main(int argc, char **argv) {
  ArgParseResult args = arg_parse(argc, argv);

  //TODO: implement arg_parse function for flags

  free_arg_parse(&args);

  if (argc < 2) {
      printf("Usage: %s <source_file>\n", argv[0]);
      return 1;
  }

  char * arg_source_file = argv[1];

  FILE *sourceFile;
  if (!fopen_safe(sourceFile, argv[1], "r")) {
      fprintf(stderr, "Error opening file '%s'\n", arg_source_file);
      return 1;
  }

  int count = 0;
  Token *tokens = tokenize(sourceFile, arg_source_file, &count);

  Parser parser = parser_new();
  AstNode *ast = produce_ast(&parser, tokens, count);

  // Initialize LLVM target-related components (needed to generate machine code)
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();

  // Create the LLVM context, module, and IR builder
  llvm::LLVMContext TheContext;
  llvm::Module TheModule("GalaxyJIT", TheContext);
  llvm::IRBuilder<> Builder(TheContext);

  // Create the global scope
  enter_scope();

  // Generate the LLVM IR from the AST
  std::vector<llvm::Value*> values = generate_ir(ast, TheContext, TheModule, Builder);

  // Print out the generated LLVM IR for debugging
  for (size_t i = 0; i < values.size(); ++i) {
      if (values[i]) {
          values[i]->print(llvm::errs());  // Print IR to error stream
          std::cout << "\n";
      } else {
          std::cerr << "Valor IR nulo encontrado no índice " << i << "\n";  // Handle NULL IR value
      }
  }

  // Verify the generated module for correctness
  std::string errorMsg;
  llvm::raw_string_ostream errorStream(errorMsg);
  if (llvm::verifyModule(TheModule, &errorStream)) {
      llvm::errs() << "Erro ao verificar o módulo:\n" << errorStream.str();
      free_ast_node(ast);  // Free the AST if there's an error
      freeTokens(tokens, count);  // Free the tokens
      fclose(sourceFile);  // Close the source file
      return 1;
  }

  // Set the target triple (the target machine details) for code generation
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  TheModule.setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  if (!Target) {
      llvm::errs() << Error;
      return 1;
  }

  // Specify the target CPU and features (e.g., generic)
  auto CPU = "generic";
  auto Features = "";

  // Set the target options and create the target machine
  llvm::TargetOptions opt;
  auto TheTargetMachine = Target->createTargetMachine(
      TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

  // Set the data layout for the module based on the target machine
  TheModule.setDataLayout(TheTargetMachine->createDataLayout());

  // Write the generated IR to a file
  auto IRFilename = "output.ll";
  std::error_code EC;
  llvm::raw_fd_ostream IRFile(IRFilename, EC, llvm::sys::fs::OF_None);

  if (EC) {
      llvm::errs() << "Could not open file: " << EC.message();
      return 1;
  }

  TheModule.print(IRFile, nullptr);
  IRFile.flush();
  llvm::outs() << "Wrote IR to " << IRFilename << "\n";

  // Generate an object file from the IR using the target machine
  auto ObjFilename = "output.o";
  llvm::raw_fd_ostream ObjFile(ObjFilename, EC, llvm::sys::fs::OF_None);

  if (EC) {
      llvm::errs() << "Could not open file: " << EC.message();
      return 1;
  }

  llvm::legacy::PassManager pass;
  auto FileType = llvm::CodeGenFileType::ObjectFile;

  // Add passes to generate the object file
  if (TheTargetMachine->addPassesToEmitFile(pass, ObjFile, nullptr, FileType)) {
      llvm::errs() << "TheTargetMachine can't emit a file of this type";
      return 1;
  }

  // Execute the passes to generate the object file
  pass.run(TheModule);
  ObjFile.flush();

  llvm::outs() << "Wrote object file to " << ObjFilename << "\n";

  free_ast_node(ast);
  freeTokens(tokens, count);
  fclose(sourceFile);
  return 0;
}
