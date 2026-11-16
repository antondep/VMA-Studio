#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Expr.h>

class rule502
{
public:
    static void check(clang::BinaryOperator *op, clang::ASTContext *context);

private:
    static bool isUnparenthesizedComparison(const clang::Expr *expr);
};