#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp("\nThis tool outputs some useful software metrics for C++ code in YAML.\n");

DeclarationMatcher FunctionMatcher =
	functionDecl(isDefinition()).bind("functionDef");

// This class outputs the metrics to stdout in YAML. To keep things simple the YAML is output
// using the normal diagnostic output stuff. Can use the llvm YAML library later if there's
// a benefit to that (e.g. if we need to read a configuration YAML file).
class FunctionPrinter : public MatchFinder::MatchCallback {
public :

virtual void run(const MatchFinder::MatchResult &Result) {
	if (const FunctionDecl *fun = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDef")){
		if ( fun->isDefaulted() )
			return; // ignore implicit functions
		FullSourceLoc startLocation = Result.Context->getFullLoc(fun->getLocStart());
		if ( startLocation.isInSystemHeader() )
			return;
		FullSourceLoc endLocation = Result.Context->getFullLoc(fun->getLocEnd());
		if (startLocation.isValid() && endLocation.isValid() ){
			llvm::outs() << "- { "
				<< "file_name: " << Result.Context->getSourceManager().getFilename(startLocation) << ", "
				<< "fun_name: " << fun->getNameInfo().getName().getAsString() << ", "
				<< "line_start: " << startLocation.getSpellingLineNumber() << ", "
				<< "line_end: " << endLocation.getSpellingLineNumber()
				<< " }\n";
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
