#include "SyntaxAnalyse.hpp"
#include "cstring"

extern ast::SyntaxTree syntax_tree;

// 补充函数，二元全部情况一键识别
void SynataxAnalyseBinopExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    auto result = new ast::binop_expr_syntax;
    // 相等返回 false
    if (!strcmp(op, "+"))
    {
        result->op = binop::plus;
    }
    else if (!strcmp(op, "-"))
    {
        result->op = binop::minus;
    }
    else if (!strcmp(op, "*"))
    {
        result->op = binop::multiply;
    }
    else if (!strcmp(op, "/"))
    {
        result->op = binop::divide;
    }
    else
    {
        result->op = binop::modulo;
    }
    // result->restype = vartype::INT;
    result->lhs = std::shared_ptr<ast::expr_syntax>(exp1);
    result->rhs = std::shared_ptr<ast::expr_syntax>(exp2);
    self = static_cast<ast::expr_syntax *>(result);
}

// 补充函数，relop 逻辑情况全部情况一键识别
void SynataxAnalyseRelopLogicExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    auto result = new ast::logic_cond_syntax;
    // 相等返回 false
    if (!strcmp(op, "&&"))
    {
        result->op = relop::op_and;
    }
    else
    {
        result->op = relop::op_or;
    }
    // result->restype = vartype::INT;
    result->lhs = std::shared_ptr<ast::expr_syntax>(exp1);
    result->rhs = std::shared_ptr<ast::expr_syntax>(exp2);
    self = static_cast<ast::expr_syntax *>(result);
}

// 补充函数，relop 全部情况一键识别
void SynataxAnalyseRelopExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    auto result = new ast::rel_cond_syntax;
    // 相等返回 false
    if (!strcmp(op, "=="))
    {
        result->op = relop::equal;
    }
    else if (!strcmp(op, "!="))
    {
        result->op = relop::non_equal;
    }
    else if (!strcmp(op, "<"))
    {
        result->op = relop::less;
    }
    else if (!strcmp(op, "<="))
    {
        result->op = relop::less_equal;
    }
    else if (!strcmp(op, ">"))
    {
        result->op = relop::greater;
    }
    else if (!strcmp(op, ">="))
    {
        result->op = relop::greater_equal;
    }
    else if (!strcmp(op, "&&"))
    {
        result->op = relop::op_and;
    }
    else
    {
        result->op = relop::op_or;
    }
    // result->restype = vartype::INT;
    result->lhs = std::shared_ptr<ast::expr_syntax>(exp1);
    result->rhs = std::shared_ptr<ast::expr_syntax>(exp2);
    self = static_cast<ast::expr_syntax *>(result);
}

void SyntaxAnalyseCompUnit(ast::compunit_syntax *&self, ast::compunit_syntax *compunit, ast::func_def_syntax *func_def)
{
    if (compunit)
    {
        self = new ast::compunit_syntax;
        for (auto i : compunit->global_defs)
        {
            self->global_defs.emplace_back(std::shared_ptr<ast::func_def_syntax>(i));
        }
        self->global_defs.emplace_back(func_def);
    }
    else
    {
        self = new ast::compunit_syntax;
        self->global_defs.emplace_back(func_def);
    }
    syntax_tree.root = self;
}

void SyntaxAnalyseFuncDef(ast::func_def_syntax *&self, vartype var_type, char *Ident, ast::block_syntax *block)
{
    self = new ast::func_def_syntax;
    self->name = Ident;
    self->rettype = var_type;
    self->body = std::shared_ptr<ast::block_syntax>(block);
}

void SynataxAnalyseFuncType(vartype &self, char *type)
{
    self = (!strcmp(type, "int") ? vartype::INT : vartype::VOID);
}

void SynataxAnalyseBlock(ast::block_syntax *&self, ast::block_syntax *block_items)
{
    self = new ast::block_syntax;
    if (block_items)
    {
        for (auto i : block_items->body)
        {
            self->body.emplace_back(i);
        }
    }
}

void SynataxAnalyseBlockItems(ast::block_syntax *&self, ast::block_syntax *block_items, ast::stmt_syntax *stmt)
{
    self = new ast::block_syntax;
    if (block_items && stmt)
    {
        for (auto i : block_items->body)
        {
            self->body.emplace_back(std::shared_ptr<ast::stmt_syntax>(i));
        }
        self->body.emplace_back(std::shared_ptr<ast::stmt_syntax>(stmt));
    }
    else if (!stmt && !block_items)
    {
        self = nullptr;
    }
    else
    {
        self->body.emplace_back(std::shared_ptr<ast::stmt_syntax>(stmt));
    }
}

void SynataxAnalyseStmtReturn(ast::stmt_syntax *&self, ast::expr_syntax *exp)
{
    auto syntax = new ast::return_stmt_syntax;
    if (exp)
        syntax->exp = std::shared_ptr<ast::expr_syntax>(exp);
    self = static_cast<ast::stmt_syntax *>(syntax);
}

void SynataxAnalysePrimaryExpIntConst(ast::expr_syntax *&self, char *current_symbol)
{
    auto result = new ast::literal_syntax;
    result->intConst = std::stoi(current_symbol);
    self = static_cast<ast::expr_syntax *>(result);
}

void SynataxAnalyseStmtBlock(ast::stmt_syntax *&self, ast::block_syntax *block)
{
    auto result = new ast::block_syntax;
    if (block)
    {
        for (auto i : block->body)
        {
            result->body.insert(result->body.cbegin(), i);
        }
    }
    self = static_cast<ast::stmt_syntax *>(result);
}

// t1 完成，负责 t1 中的 return a 部分
void SynataxAnalysePrimaryExpVar(ast::expr_syntax *&self, char *current_symbol)
{
    auto result = new ast::lval_syntax;
    result->name = current_symbol;
    self = static_cast<ast::expr_syntax *>(result);
}

// t1 完成，负责变量赋值，在编写时测试了多赋值的情况，使用反插
void SynataxAnalyseVarDecl(ast::stmt_syntax *&self, ast::var_def_stmt_syntax *var_def, ast::var_decl_stmt_syntax *var_def_group)
{
    auto result = new ast::var_decl_stmt_syntax;
    if (var_def_group)
    {
        for (auto i : var_def_group->var_def_list)
        {
            // TODO: 反向插入
            result->var_def_list.insert(result->var_def_list.cbegin(), std::shared_ptr<ast::var_def_stmt_syntax>(i));
        }
    }
    if (var_def)
    {
        result->var_def_list.insert(result->var_def_list.cbegin(), std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
    }
    self = static_cast<ast::stmt_syntax *>(result);
}

// 同上
void SynataxAnalyseVarDefGroup(ast::var_decl_stmt_syntax *&self, ast::var_def_stmt_syntax *var_def, ast::var_decl_stmt_syntax *var_def_group)
{
    auto result = new ast::var_decl_stmt_syntax;
    if (self)
    {
        for (auto i : self->var_def_list)
        {
            result->var_def_list.insert(result->var_def_list.cbegin(), std::shared_ptr<ast::var_def_stmt_syntax>(i));
        }
    }
    if (var_def_group)
    {
        for (auto i : var_def_group->var_def_list)
        {
            result->var_def_list.insert(result->var_def_list.cbegin(), std::shared_ptr<ast::var_def_stmt_syntax>(i));
        }
    }
    if (var_def)
    {
        result->var_def_list.insert(result->var_def_list.cbegin(), std::shared_ptr<ast::var_def_stmt_syntax>(var_def));
    }
    self = result;
}

// t1 完成
void SynataxAnalyseVarDef(ast::var_def_stmt_syntax *&self, char *ident, ast::expr_syntax *init)
{
    auto result = new ast::var_def_stmt_syntax;
    result->name = ident;
    result->initializer = std::shared_ptr<ast::expr_syntax>(init);
    self = result;
}

void SynataxAnalyseAddExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    SynataxAnalyseBinopExp(self, exp1, op, exp2);
}

void SynataxAnalyseMulExp(ast::expr_syntax *&self, ast::expr_syntax *exp1, char *op, ast::expr_syntax *exp2)
{
    SynataxAnalyseBinopExp(self, exp1, op, exp2);
}

void SynataxAnalyseStmtAssign(ast::stmt_syntax *&self, ast::lval_syntax *target, ast::expr_syntax *value)
{
    auto result = new ast::assign_stmt_syntax;
    result->target = std::shared_ptr<ast::lval_syntax>(target);
    result->value = std::shared_ptr<ast::expr_syntax>(value);
    self = static_cast<ast::stmt_syntax *>(result);
}

// t2 完成，负责 a = 10 中的 a 部分
void SynataxAnalyseLval(ast::lval_syntax *&self, char *ident)
{
    auto result = new ast::lval_syntax;
    result->name = ident;
    result->restype = self->restype;
    self = result;
}

void SynataxAnalyseStmtIf(ast::stmt_syntax *&self, ast::expr_syntax *cond, ast::stmt_syntax *then_body, ast::stmt_syntax *else_body)
{
    auto result = new ast::if_stmt_syntax;
    result->pred = std::shared_ptr<ast::expr_syntax>(cond);
    result->then_body = std::shared_ptr<ast::stmt_syntax>(then_body);
    result->else_body = std::shared_ptr<ast::stmt_syntax>(else_body);
    self = static_cast<ast::stmt_syntax *>(result);
}

void SynataxAnalyseLOrExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, ast::expr_syntax *cond2)
{
    SynataxAnalyseRelopLogicExp(self, cond1, "||", cond2);
}

void SynataxAnalyseLAndExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, ast::expr_syntax *cond2)
{
    SynataxAnalyseRelopLogicExp(self, cond1, "&&", cond2);
}

void SynataxAnalyseEqExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, char *op, ast::expr_syntax *cond2)
{
    SynataxAnalyseRelopExp(self, cond1, op, cond2);
}

void SynataxAnalyseRelExp(ast::expr_syntax *&self, ast::expr_syntax *cond1, char *op, ast::expr_syntax *exp)
{
    SynataxAnalyseRelopExp(self, cond1, op, exp);
}

void SynataxAnalyseUnaryExp(ast::expr_syntax *&self, char *op, ast::expr_syntax *exp)
{
    auto result = new ast::unaryop_expr_syntax;
    if (!strcmp(op, "+"))
    {
        result->op = unaryop::plus;
    }
    else if (!strcmp(op, "-"))
    {
        result->op = unaryop::minus;
    }
    else
    {
        result->op = unaryop::op_not;
    }
    result->rhs = std::shared_ptr<ast::expr_syntax>(exp);
    // result->restype = vartype::INT;
    self = static_cast<ast::expr_syntax *>(result);
}
