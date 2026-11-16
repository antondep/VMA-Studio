#pragma once

#include <clang/AST/ASTContext.h>
#include <clang/AST/Stmt.h>
#include <clang/AST/Expr.h>

class rule560
{
public:
    static void check(clang::IfStmt *stmt, clang::ASTContext *context);

private:
    static bool containsBoolLiteral(const clang::Expr *expr);
};