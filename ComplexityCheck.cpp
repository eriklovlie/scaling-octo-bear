#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <sstream>

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp("\nThis tool outputs some useful software metrics for C++ code in JSON format.\n");

DeclarationMatcher FunctionMatcher =
	functionDecl(isDefinition()).bind("functionDef");

// This class outputs the metrics to stdout in the form of a JSON array.
class FunctionPrinter : public MatchFinder::MatchCallback {

public:

	virtual void run(const MatchFinder::MatchResult &Result) {
		if (const FunctionDecl *fun = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDef")){
			if ( fun->isDefaulted() )
				return; // ignore implicit functions
			FullSourceLoc startLocation = Result.Context->getFullLoc(fun->getLocStart());
			if ( startLocation.isInSystemHeader() )
				return;
			FullSourceLoc endLocation = Result.Context->getFullLoc(fun->getLocEnd());
			if (startLocation.isValid() && endLocation.isValid() ){
				flush();
				ss << "{ ";
				ss << "\"file_name\": \"";
				ss << Result.Context->getSourceManager().getFilename(startLocation).str();
				ss << "\", ";
				ss << "\"fun_name\": \"";
				ss << fun->getQualifiedNameAsString();
				ss << "\", ";
				ss << "\"line_start\": ";
				ss << startLocation.getSpellingLineNumber();
				ss << ", ";
				ss << "\"line_end\": ";
				ss << endLocation.getSpellingLineNumber();
				ss << " }";
			}

			//llvm::outs() << "max_depth = " << max_nesting_depth(fun) << "\n";
		}
	}

	void flush( bool last=false ){
		if ( !ss.str().empty() ){
			llvm::outs() << ss.str();
			if ( !last ){
				llvm::outs() << ",";
			}
			llvm::outs() << "\n";	
			ss.str("");
		}
	}

private:
	std::stringstream ss;

	/*
	int max_nesting_depth ( const FunctionDecl *fun ){
		Stmt *body = fun->getBody();
		return max_nesting_depth_recursive( body, 0 );
	}

	int max_nesting_depth_recursive ( const Stmt *stmt, int depth ){
		// FIXME It is a bit unclear to me how to cleanly separate actual statements from Expr which inherits Stmt
		// Also there are some implicitly added statements that we don't want to include.
		int max = depth;
		for ( ConstStmtIterator child = stmt->child_begin(); child != stmt->child_end(); child++ ){
			if ( is_compound( *child) )
				max = std::max( max, max_nesting_depth_recursive( *child, depth + 1 ) );
		}
		return max;
	}

	bool is_compound ( const Stmt *stmt ){
		return dynamic_cast<const CompoundStmt*>(stmt) != NULL;
	}
*/
};

int main(int argc, const char **argv) {
	CommonOptionsParser OptionsParser(argc, argv);
	ClangTool Tool(OptionsParser.getCompilations(), OptionsParser.getSourcePathList());

	FunctionPrinter Printer;
	MatchFinder Finder;
	Finder.addMatcher(FunctionMatcher, &Printer);

	llvm::outs() << "[\n";
	int retcode = Tool.run(newFrontendActionFactory(&Finder));
	Printer.flush(true);
	llvm::outs() << "]\n";

	return retcode;
}
