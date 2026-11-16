#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>

class rule606
{
public:
    static void check(clang::BinaryOperator *op, clang::ASTContext *context);

private:
    static bool extractVarAndConstant(const clang::Expr *expr,
                                      clang::BinaryOperatorKind expectedOp,
                                      const clang::Expr *&outVar,
                                      llvm::APSInt &outVal,
                                      clang::ASTContext *context);
    static bool areEquivalentVars(const clang::Expr *lhs,
                                  const clang::Expr *rhs,
                                  clang::ASTContext *context);
};