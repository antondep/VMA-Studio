#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>

class rule570
{
public:
    static void check(clang::BinaryOperator *op, clang::ASTContext *context);

private:
    static bool areEquivalentSubExprs(const clang::Expr *lhs,
                                      const clang::Expr *rhs,
                                      clang::ASTContext *context);
};