#include "rules/rule523.hpp"
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/FoldingSet.h>
#include <iostream>

bool rule523::areEquivalentStmts(const clang::Stmt *lhs,
                                 const clang::Stmt *rhs,
                                 clang::ASTContext *context)
{
    if (!lhs || !rhs || !context)
        return false;

    if (const auto *compoundL = llvm::dyn_cast<clang::CompoundStmt>(lhs))
    {
        if (compoundL->size() == 1)
        {
            lhs = compoundL->body_front();
        }
    }

    if (const auto *compoundR = llvm::dyn_cast<clang::CompoundStmt>(rhs))
    {
        if (compoundR->size() == 1)
        {
            rhs = compoundR->body_front();
        }
    }

    llvm::FoldingSetNodeID idLHS, idRHS;
    lhs->Profile(idLHS, *context, true);
    rhs->Profile(idRHS, *context, true);

    return idLHS == idRHS;
}

void rule523::check(clang::IfStmt *stmt, clang::ASTContext *context)
{
    if (!stmt || !context)
        return;

    clang::SourceManager &sm = context->getSourceManager();
    clang::SourceLocation loc = stmt->getIfLoc();

    if (sm.isInSystemHeader(loc))
    {
        return;
    }

    const clang::Stmt *thenBranch = stmt->getThen();
    const clang::Stmt *elseBranch = stmt->getElse();

    if (!thenBranch || !elseBranch)
    {
        return;
    }

    if (llvm::isa<clang::IfStmt>(elseBranch))
    {
        return;
    }

    if (areEquivalentStmts(thenBranch, elseBranch, context))
    {
        unsigned lineNo = sm.getSpellingLineNumber(loc);
        std::cout << "\033[1;31m[WARNING 523]\033[0m The 'then' statement is equivalent to the 'else' statement "
                  << "at line " << lineNo << std::endl;
    }
}