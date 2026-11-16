#include "rules/rule606.hpp"
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/FoldingSet.h>
#include <iostream>

bool rule606::areEquivalentVars(const clang::Expr *lhs,
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

bool rule606::extractVarAndConstant(const clang::Expr *expr,
                                    clang::BinaryOperatorKind expectedOp,
                                    const clang::Expr *&outVar,
                                    llvm::APSInt &outVal,
                                    clang::ASTContext *context)
{
    if (!expr || !context)
        return false;

    const auto *binOp = llvm::dyn_cast<clang::BinaryOperator>(expr->IgnoreParenImpCasts());
    if (!binOp || binOp->getOpcode() != expectedOp)
    {
        return false;
    }

    const clang::Expr *lhs = binOp->getLHS()->IgnoreParenImpCasts();
    const clang::Expr *rhs = binOp->getRHS()->IgnoreParenImpCasts();

    if (const auto *intLit = llvm::dyn_cast<clang::IntegerLiteral>(rhs))
    {
        outVar = lhs;
        outVal = llvm::APSInt(intLit->getValue(), intLit->getType()->isUnsignedIntegerType());
        return true;
    }

    if (const auto *intLit = llvm::dyn_cast<clang::IntegerLiteral>(lhs))
    {
        outVar = rhs;
        outVal = llvm::APSInt(intLit->getValue(), intLit->getType()->isUnsignedIntegerType());
        return true;
    }

    return false;
}

void rule606::check(clang::BinaryOperator *op, clang::ASTContext *context)
{
    if (!op || !context)
        return;

    clang::SourceManager &sm = context->getSourceManager();
    clang::SourceLocation loc = op->getOperatorLoc();

    if (sm.isInSystemHeader(loc))
    {
        return;
    }

    clang::BinaryOperatorKind outerOp = op->getOpcode();
    clang::BinaryOperatorKind targetCmpOp;

    if (outerOp == clang::BO_LAnd)
    {
        targetCmpOp = clang::BO_EQ;
    }
    else if (outerOp == clang::BO_LOr)
    {
        targetCmpOp = clang::BO_NE;
    }
    else
    {
        return;
    }

    const clang::Expr *leftVar = nullptr;
    const clang::Expr *rightVar = nullptr;
    llvm::APSInt leftVal, rightVal;

    if (extractVarAndConstant(op->getLHS(), targetCmpOp, leftVar, leftVal, context) &&
        extractVarAndConstant(op->getRHS(), targetCmpOp, rightVar, rightVal, context))
    {

        if (areEquivalentVars(leftVar, rightVar, context) &&
            llvm::APSInt::compareValues(leftVal, rightVal) != 0)
        {
            unsigned lineNo = sm.getSpellingLineNumber(loc);
            std::cout << "\033[1;31m[WARNING 606]\033[0m Expression is always "
                      << (outerOp == clang::BO_LAnd ? "false" : "true")
                      << " at line " << lineNo << std::endl;
        }
    }
}