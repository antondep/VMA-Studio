#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>

class rule523
{
public:
    static void check(clang::IfStmt *stmt, clang::ASTContext *context);

private:
    static bool areEquivalentStmts(const clang::Stmt *lhs,
                                   const clang::Stmt *rhs,
                                   clang::ASTContext *context);
};