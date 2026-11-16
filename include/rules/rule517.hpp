#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>

class rule517
{
public:
    static void check(clang::IfStmt *stmt, clang::ASTContext *context);

private:
    static bool areEquivalentConds(const clang::Expr *lhs,
                                   const clang::Expr *rhs,
                                   clang::ASTContext *context);
};