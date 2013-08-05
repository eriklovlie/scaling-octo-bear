// Declares clang::SyntaxOnlyAction.
#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
// Declares llvm::cl::extrahelp.
#include "llvm/Support/CommandLine.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...");

DeclarationMatcher FunctionMatcher =
	functionDecl(isDefinition()).bind("functionDef");

class FunctionPrinter : public MatchFinder::MatchCallback {
public :

virtual void run(const MatchFinder::MatchResult &Result) {
	if (const FunctionDecl *fun = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDef")){
		FullSourceLoc startLocation = Result.Context->getFullLoc(fun->getLocStart());
		FullSourceLoc endLocation = Result.Context->getFullLoc(fun->getLocEnd());
		if (startLocation.isValid() && endLocation.isValid() ){
			llvm::outs() << "Found declaration at ("
				<< startLocation.getSpellingLineNumber() << ":"
				<< startLocation.getSpellingColumnNumber() << ") - ("
				<< endLocation.getSpellingLineNumber() << ":"
				<< endLocation.getSpellingColumnNumber() << ")\n";
		}
	}
}

};

int main(int argc, const char **argv) {
	CommonOptionsParser OptionsParser(argc, argv);
	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

	FunctionPrinter Printer;
	MatchFinder Finder;
	Finder.addMatcher(FunctionMatcher, &Printer);

	return Tool.run(newFrontendActionFactory(&Finder));
}
