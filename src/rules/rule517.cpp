#include "rules/rule517.hpp"
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/FoldingSet.h>
#include <iostream>

bool rule517::areEquivalentConds(const clang::Expr *lhs,
                                 const clang::Expr *rhs,
                                 clang::ASTContext *context)
{
    if (!lhs || !rhs || !context)
        return false;

    const clang::Expr *cleanLHS = lhs->IgnoreParenImpCasts();
    const clang::Expr *cleanRHS = rhs->IgnoreParenImpCasts();

    llvm::FoldingSetNodeID idLHS, idRHS;
    cleanLHS->Profile(idLHS, *context, true);
    cleanRHS->Profile(idRHS, *context, true);

    return idLHS == idRHS;
}

void rule517::check(clang::IfStmt *stmt, clang::ASTContext *context)
{
    if (!stmt || !context)
        return;

    clang::SourceManager &sm = context->getSourceManager();
    clang::SourceLocation loc = stmt->getIfLoc();

    if (sm.isInSystemHeader(loc))
    {
        return;
    }

    const clang::Expr *firstCond = stmt->getCond();
    const clang::Stmt *elseBranch = stmt->getElse();

    while (const auto *nestedIf = llvm::dyn_cast_or_null<clang::IfStmt>(elseBranch))
    {
        if (areEquivalentConds(firstCond, nestedIf->getCond(), context))
        {
            unsigned lineNo = sm.getSpellingLineNumber(nestedIf->getIfLoc());
            std::cout << "\033[1;31m[WARNING 517]\033[0m Identical condition in 'if' and 'else if' "
                      << "at line " << lineNo << std::endl;
        }
        elseBranch = nestedIf->getElse();
    }
}