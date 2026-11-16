#include "rules/rule502.hpp"
#include <clang/Basic/SourceManager.h>
#include <iostream>

bool rule502::isUnparenthesizedComparison(const clang::Expr *expr)
{
    if (!expr)
        return false;

    const clang::Expr *cleanExpr = expr->IgnoreImpCasts();

    if (const auto *binOp = llvm::dyn_cast<clang::BinaryOperator>(cleanExpr))
    {
        return binOp->isComparisonOp();
    }

    return false;
}

void rule502::check(clang::BinaryOperator *op, clang::ASTContext *context)
{
    if (!op || !context)
        return;

    clang::SourceManager &sm = context->getSourceManager();
    clang::SourceLocation loc = op->getOperatorLoc();

    if (sm.isInSystemHeader(loc))
    {
        return;
    }

    if (op->isBitwiseOp())
    {
        if (isUnparenthesizedComparison(op->getLHS()) ||
            isUnparenthesizedComparison(op->getRHS()))
        {
            unsigned lineNo = sm.getSpellingLineNumber(loc);
            std::cout << "\033[1;31m[WARNING 502]\033[0m Suspicious precedence between bitwise '"
                      << op->getOpcodeStr().str() << "' and comparison operator "
                      << "at line " << lineNo << std::endl;
        }
    }
}