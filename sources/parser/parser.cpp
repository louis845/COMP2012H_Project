#include "parser.h"
using std::cout;
using std::endl;

typedef Token::TokName TokName;
typedef Token::TokType TokType;

// TODO: add custom exception classes and refine exception handling

/*  
 *  number      ::= [(\+|-)?(\.[0-9]+|[0-9]+\.?[0-9]*)((e|E)(\+|-)?[0-9]+)?] | constants
 *  variable    ::= [A-Za-z] | greek letters
 *  function    ::= functions
 *  binop       ::= + | - | * | ** | xx | // | / | ^ | % 
 */


// TODO: add support for intermediate expressions with super/subscripts
std::unordered_map<TokName, int> Parser::bin_op_precedence = 
{
    {TokName::PLUS, 100}, {TokName::MINUS, 100}, 
    {TokName::AST, 200}, {TokName::CDOT, 200}, {TokName::DIV, 200}, {TokName::FRAC, 200}, {TokName::PERCENT, 200}, 
    {TokName::CROSS, 300}, {TokName::SUP, 400}
};


Parser::Parser(const std::string& input): input(input), tokenizer(input) { getNextToken(); }


Parser::~Parser() { delete root; delete cur_tok; }


void Parser::reset_input(const std::string& input)
{
    delete root;
    delete cur_tok;
    var_table.clear();
    root = nullptr;
    cur_tok = nullptr;
    this->input = input;
    tokenizer.reset_input(input);
    getNextToken();
}


// only return precedence of binary operators
// any other tokens which are not in {')', ']', '}', '|', ',', '='} will have precedence -1
// ')', ']', '}', ',', '=' have precedence -2
// '|' has precedence -3
int Parser::getTokPrecedence()
{
    if (cur_tok == nullptr)    return -1;
    switch (cur_tok->get_name())
    {
        case TokName::COMMA: case TokName::RP: case TokName::RSB: 
        case TokName::RCB: case TokName::EQUAL:
            return -2;
        case TokName::TEXTBAR: 
            return -3;
    }

    TokName name = cur_tok->get_name();
    return bin_op_precedence.count(name) == 0 ? -1 : bin_op_precedence[name];
}


void Parser::getNextToken()
{
    delete cur_tok;
    cur_tok = tokenizer.getNextToken();        // caller will check whether cur_tok is a nullptr
}


// numberexpr      
//          ::= number
NumberExprAst* Parser::parseNum()
{
    if (cur_tok == nullptr || cur_tok->get_type() != TokType::NUM) return nullptr;
    NumberExprAst* num = new NumberExprAst(cur_tok->get_name(), cur_tok->get_raw_value());
    getNextToken();
    return num;
}


// matrixexpr      
//          ::= '[' bracketexpr (',' bracketexpr)* ']'
MatrixExprAst* Parser::parseMatrix()
{
    if (cur_tok == nullptr || cur_tok->get_name() != TokName::LSB)  return nullptr;
    getNextToken();

    MatrixExprAst* matrix = new MatrixExprAst;
    std::vector<ExprAst*> row;
    try
    {
        row = parseBracket();
    }
    catch(const std::invalid_argument& e)
    {
        std::cerr << "Invalid matrix: " << e.what() << '\n';
        delete matrix;
        throw std::invalid_argument("Invalid matrix.");
    }
    
    if (row.empty())
    {
        delete matrix;
        throw std::invalid_argument("Empty entries in matrix.");
    }
    
    matrix->entries.push_back(row);
    size_t col_size = row.size();

    while (cur_tok != nullptr && cur_tok->get_name() == TokName::COMMA)
    {
        getNextToken();
        try
        {
            row = parseBracket();
        }
        catch(const std::invalid_argument& e)
        {
            std::cerr << "Invalid matrix: " << e.what() << '\n';
            delete matrix;
            throw std::invalid_argument("Invalid matrix.");
        }
        
        if (row.size() != col_size)
        {
            delete matrix;
            throw std::invalid_argument("Column sizes mismatch.");
        }
        matrix->entries.push_back(row);
        // getNextToken();
    }

    if (cur_tok->get_name() != TokName::RSB)
    {
        delete matrix;
        throw std::invalid_argument("Missing right square bracket for the matrix.");
    }
    getNextToken();
    return matrix;
}


// bracketexpr     
//          ::= '[' expression (',' expression)* ']'
std::vector<ExprAst*> Parser::parseBracket()
{
    std::vector<ExprAst*> row;
    if (cur_tok == nullptr || cur_tok->get_name() != TokName::LSB)  return row;
    getNextToken();

    try 
    {
        ExprAst* entry = parseExpr();
        row.push_back(entry);

        while (cur_tok != nullptr && cur_tok->get_name() == TokName::COMMA)
        {
            getNextToken();
            entry = parseExpr();
            row.push_back(entry);
        }

        if (cur_tok == nullptr || cur_tok->get_name() != TokName::RSB)
            throw std::invalid_argument("Incomplete matrix, expecting ']'.");

        getNextToken();
        return row;
    }
    catch (...)     // catch all exceptions but do not rethrow
    {
        throw std::invalid_argument("Invalid entries in the matrix.");     // only let top-level parseMatrix() handle exceptions
    } 
}


// parenexpr       
//          ::= '(' expression ')'
//          ::= '|' expression '|'
ExprAst* Parser::parseParen()
{
    if (cur_tok == nullptr)     return nullptr;

    if (cur_tok->get_name() == TokName::LP)
    {
        getNextToken();
        ExprAst* temp = parseExpr();
        if (cur_tok->get_name() != TokName::RP)
        {
            delete temp;
            throw std::invalid_argument("Missing right parenthesis.");
        }

        getNextToken();
        return temp;
    }
    
    if (cur_tok->get_name() == TokName::TEXTBAR)
    {
        getNextToken();
        ExprAst* temp = parseExpr(true);
        if (cur_tok->get_name() != TokName::TEXTBAR)
        {
            delete temp;
            throw std::invalid_argument("Missing closing textbar.");
        }

        FunctionExprAst* abs = new FunctionExprAst(TokName::ABS, cur_tok->get_raw_value());
        abs->args.push_back(temp);
        getNextToken();
        return abs;
    }

    return nullptr;
}


// identifierexpr  
//          ::= variable
//          ::= function ( number | variable )
//          ::= function '(' expression (',' expression)* ')'
//          ::= function matrixexpr
ExprAst* Parser::parseId()
{
    if (cur_tok == nullptr)     return nullptr;
    if (cur_tok->get_type() == TokType::ID)
    {
        ExprAst* var = new VariableExprAst(cur_tok->get_raw_value());
        var_table.emplace(cur_tok->get_raw_value());
        getNextToken();
        return var;
    }
    if (cur_tok->get_type() != TokType::OP || getTokPrecedence() != -1)     
        return nullptr;     // binops are not functions

    FunctionExprAst* func = new FunctionExprAst(cur_tok->get_name(), cur_tok->get_raw_value());
    getNextToken();

    if (cur_tok->get_type() == TokType::NUM)
    {
        func->args.emplace_back(parseNum());
        // getNextToken();
        return func;
    }

    if (cur_tok->get_type() == TokType::ID)
    {
        ExprAst* var = new VariableExprAst(cur_tok->get_raw_value());
        var_table.emplace(cur_tok->get_raw_value());
        func->args.emplace_back(var);
        getNextToken();
        return func;
    }

    if (cur_tok->get_name() == TokName::LP)
    {
        getNextToken();
        try
        {
            ExprAst* expr = parseExpr();
            func->args.emplace_back(expr);
            while (cur_tok && cur_tok->get_name() == TokName::COMMA)
            {
                getNextToken();
                expr = parseExpr();
                func->args.emplace_back(expr);
            }

            if (cur_tok->get_name() != TokName::RP)
                throw std::invalid_argument("Missing right parenthesis for function.");

            getNextToken();
            return func;
        }
        catch (...)
        {
            delete func;
            throw;      // rethrow, main driver function will handle
        }
    }

    if (cur_tok->get_name() == TokName::LSB)
    {
        try
        {
            MatrixExprAst* mat = parseMatrix();
            func->args.emplace_back(mat);
            return func;
        }
        catch (...)
        {
            delete func;
            throw;
        }
    }

    delete func;
    return nullptr;
}


// primaryexpr 
//          ::= numberexpr
//          ::= parenexpr
//          ::= matrixexpr
//          ::= identifierexpr
ExprAst* Parser::parsePrimary(bool inside_textbar)
{
    if (cur_tok == nullptr)     return nullptr;
    if (cur_tok->get_type() == TokType::NUM)
    {
        ExprAst* numexpr = parseNum();
        return numexpr;
    }

    if (cur_tok->get_name() == TokName::LP || cur_tok->get_name() == TokName::TEXTBAR)
    {
        ExprAst* parenexpr = parseParen();
        return parenexpr;
    }

    if (cur_tok->get_name() == TokName::LSB)
    {
        ExprAst* matrixexpr = parseMatrix();
        return matrixexpr;
    }

    ExprAst* expr = parseId();
    if (!expr)  throw std::invalid_argument("Unknown expression.");
    return expr;
}


// binoprhs    
//      ::= ( binop ( '+' | '-' )? primaryexpr | ( '+' | '-' )? primaryexpr )*
ExprAst* Parser::parseBinOpRhs(int min_precedence, ExprAst* lhs, bool inside_textbar)
{
    // loop will terminate when 
    // 1) hitting right closing brackets i.e. ) ] } 
    // 2) hitting a pairing textbar when parsing a primaryexpr
    // 3) hitting EOF or tokenizer encountered an error   
    while (true)
    {
        if (!cur_tok)   return lhs;
        int cur_precedence = getTokPrecedence();
        TokName cur_op{TokName::CDOT};              // if not a binary op, presume to be multiplication
        std::string cur_raw_str{""};

        switch (cur_precedence)
        {
            case -1:
                cur_precedence = bin_op_precedence[TokName::CDOT];      // CDOT is OK since matrix multiplication has associativity
                if (cur_precedence < min_precedence)    return lhs;
                break;

            case -2:
                return lhs;

            case -3:
                if (inside_textbar)     return lhs;
                break;

            default:
                if (cur_precedence < min_precedence)    return lhs;
                cur_op = cur_tok->get_name();
                cur_raw_str = cur_tok->get_raw_value();
                getNextToken();
                break;
        }
        
        ExprAst* rhs = nullptr;
        try
        {
            if (cur_tok->get_name() == TokName::PLUS)   getNextToken();
            else if (cur_tok->get_name() == TokName::MINUS)
            {
                FunctionExprAst* neg_rhs = new FunctionExprAst(TokName::NEG, cur_tok->get_raw_value());
                getNextToken();

                rhs = parsePrimary(inside_textbar);
                if (!rhs)   throw std::invalid_argument("Incomplete expression, missing right hand side operand.");
                neg_rhs->args.emplace_back(rhs);
                rhs = neg_rhs;
            }
            else
            {
                rhs = parsePrimary(inside_textbar);
                if (!rhs)   throw std::invalid_argument("Incomplete expression, missing right hand side operand.");
            }
        }
        catch (...)
        {
            delete rhs;
            throw;      // rethrow
        }
        
        int next_precedence = getTokPrecedence();       // no binop then assume multiplication
        next_precedence = next_precedence == -1 ? bin_op_precedence[TokName::CDOT] : next_precedence;
        if (next_precedence > cur_precedence)        // the next binary group binds more tightly           
        {
            // all operators with precedence greater than cur_precedence should be merged as the rhs
            try
            {
                rhs = parseBinOpRhs(cur_precedence + 1, rhs, inside_textbar);
                if (!rhs)   throw std::invalid_argument("Incomplete expression, missing right hand side operand.");
            }
            catch (...)
            {
                delete rhs;
                throw;      // rethrow
            }
        }

        lhs = new BinaryExprAst(cur_op, cur_raw_str, lhs, rhs);      // merging
    }
}


// expression  
//      ::= ( '+' | '-' )? primaryexpr binoprhs
ExprAst* Parser::parseExpr(bool inside_textbar)
{
    switch (cur_tok->get_name())
    {
        // decomposition needs special handling
        case TokName::SVD: case TokName:: SCHUR: case TokName::QR: case TokName::EIGEN:
        {
            FunctionExprAst* decomp = new FunctionExprAst(cur_tok->get_name(), cur_tok->get_raw_value());
            ExprAst* expr = parsePrimary(inside_textbar);
            decomp->args.emplace_back(expr);
            return decomp;          // all remaining expression will be dumped
        }
        default:    break;
    }

    if (cur_tok->get_name() == TokName::PLUS)   getNextToken();     // uniary positive operator
    else if (cur_tok->get_name() == TokName::MINUS)                 // uniary negative operator
    {
        FunctionExprAst* neg_expr = new FunctionExprAst(TokName::NEG, cur_tok->get_raw_value());
        getNextToken();
        
        ExprAst* expr = parsePrimary(inside_textbar);
        if (!expr)   throw std::runtime_error("Fatal error, unexpected exception occurs.");
        neg_expr->args.emplace_back(expr);
        return parseBinOpRhs(0, neg_expr, inside_textbar);
    }

    ExprAst* expr = parsePrimary(inside_textbar);
    if (!expr)   throw std::runtime_error("Fatal error, unexpected exception occurs.");   // should not occur during normal exception handling
    return parseBinOpRhs(0, expr, inside_textbar);
}


// auto detect which engine to use
void Parser::autoDetect(ExprAst* root)
{
    if (root == nullptr || !res.success)    return;
    if (typeid(*root) == typeid(NumberExprAst))
    {
        NumberExprAst* temp = dynamic_cast<NumberExprAst*>(root);
        if (temp->name == TokName::FLOAT || temp->name == TokName::PI || temp->name == TokName::E)
        {
            res.float_exists = true;
            if (res.engine_chosen == 1)
            {
                res.success = false;
                return;
            }
            res.engine_chosen = 2;
        }
    }
    
    if (typeid(*root) == typeid(MatrixExprAst))
    {
        MatrixExprAst* temp = dynamic_cast<MatrixExprAst*>(root);
        for (auto row : temp->entries)
            for (auto entry : row)
            {
                autoDetect(entry);
                if (!res.success)   return;
            }
    }

    if (typeid(*root) == typeid(BinaryExprAst))
    {
        BinaryExprAst* temp = dynamic_cast<BinaryExprAst*>(root);
        autoDetect(temp->lhs);
        if (!res.success)   return;
        autoDetect(temp->rhs);
        if (!res.success)   return;
    }

    if (typeid(*root) == typeid(FunctionExprAst))
    {
        FunctionExprAst* temp = dynamic_cast<FunctionExprAst*>(root);
        switch (temp->op)
        {
            case TokName::RREF: case TokName::SOLVE: case TokName::DET: 
            case TokName::INV: case TokName::ORTH: case TokName::NEG:
                break;
            
            default:
                res.func_exists = true;
                if (res.engine_chosen == 1)
                {
                    res.success = false;
                    return;
                }
                res.engine_chosen = 2;
        }

        for (auto arg : temp->args)
        {
            autoDetect(arg);
            if (!res.success)   return;
        }
    }
}


// main driver function, handles all exceptions
const Info& Parser::parse(int engine_type)
{
    res.clear();
    res.engine_chosen = engine_type;

    try
    {
        delete root;
        root = parseExpr();
        res.interpreted_input = getAsciiMath();
    }
    catch (const std::invalid_argument& err)
    {
        string err_msg = "Error: parsing failed, " + string(err.what());
        res.err = new std::invalid_argument(err_msg.c_str());
        // delete root;
        return res;
    }
    catch (...)
    {
        res.err = new std::runtime_error("Fatal error: unexpected exception thrown during parsing");
        // delete root;
        return res;
    }

    if (engine_type == 0)   autoDetect(this->root);
    if (res.success)
    {
        res.engine_used = res.engine_chosen ? res.engine_chosen : res.engine_used;
        if (res.engine_used == 1) evalR();
        if (res.engine_used == 2) eval();
        return res;
    }
    else
    {
        res.err = new std::domain_error("Error: auto detection failed, please specify engine type");
        // delete root;
        return res;
    }
}


void Parser::evalR()
{
    if (var_table.size() > 1)
    {  
        res.var_exists = true;
        res.success = false;
        res.warning += "Warning: more than one variable with unknown value in the expression.\n";
        res.warning += "You may want to assign values to some of the variables.\n";
        return;
    }

    try
    {
        ROperand result = root->evalR(res);
        res.eval_result = result.genTex();
        return;
    }
    catch (const std::invalid_argument& err)
    {
        res.success = false;
        string err_msg = "Error: evaluation failed, " + string(err.what());
        res.err = new std::invalid_argument(err_msg.c_str());
        return;
    }
    catch (...)
    {
        res.success = false;
        res.err = new std::runtime_error("Fatal error: unexpected exception thrown during evaluation");
        return;
    }

    /*        only for debugging or console output
    if (result.type == ROperand::Type::NOR) cout << result.value;
    else
    {
        cout << "[";
        for (auto row : result.mat)
        {
            cout << "[";
            for (auto col : row)
                cout << col << ", ";
            cout << "]"; 
        }
        cout << "]";
    }
    */
}


void Parser::eval()
{
    if (var_table.size() > 0)
    {
        res.var_exists = true;
        res.success = false;
        res.err = new std::invalid_argument("Warning: variables with unknown values are not supported in Armadillo.\n");
        res.warning += "You may want to assign values to the variables.\n";
        return;
    }

    try
    {
        if (typeid(*root) == typeid(FunctionExprAst))
        {
            FunctionExprAst* temp = dynamic_cast<FunctionExprAst*>(root);
            switch (temp->op)
            {
                case TokName::QR: case TokName::EIGEN: case TokName::SCHUR: case TokName::SVD:
                    evalDecomp();
                    return;
                default: break;
            }
        }

        ArmaOperand result = root->eval();
        // if (result.type == ArmaOperand::Type::NOR)  cout << result.value;
        // else if (result.type == ArmaOperand::Type::MAT) cout << result.mat;
        // else cout << "(" << result.value << ")" << "I";
        res.eval_result = result.genTex();
        return;
    }
    catch (const std::logic_error& err)     // both evaluator and Armadillo's exception will be caught here
    {
        string err_msg = "Error: evaluation failed, " + string(err.what());
        res.err = new std::invalid_argument(err_msg.c_str());
        res.success = false;
        return;
    }
    catch (const std::runtime_error& err)   // part of Armadillo's exception and some fatal error
    {
        string err_msg = "Fatal error: " + string(err.what());
        res.err = new std::runtime_error(err_msg.c_str());
        res.success = false;
        return;
    }
    catch (...)
    {
        res.success = false;
        res.err = new std::runtime_error("Fatal error: unexpected exception thrown during evaluation");
        return;
    }
}


void Parser::evalDecomp()
{
    FunctionExprAst* func = dynamic_cast<FunctionExprAst*>(root);
    ArmaOperand arg = func->args[0]->eval();
    if (arg.type != ArmaOperand::Type::MAT)
    {
        res.success = false;
        throw std::logic_error("the operand of " + func->raw + " decomposition should be a matrix");
    }

    if (func->op == TokName::EIGEN)
    {
        arma::cx_mat eigvec;
        arma::cx_vec eigval;
        arma::eig_gen(eigval, eigvec, arg.mat);
        res.eval_result = "eigenvalues: " + ArmaOperand(arma::cx_mat(eigval)).genTex() 
                            + R"( \\ eigenvectors: )" + ArmaOperand(eigvec).genTex();
        return;
    }

    if (func->op == TokName::SCHUR)
    {
        arma::cx_mat U, S;
        arma::schur(U, S, arg.mat);
        res.eval_result = R"(Schur decomposition: \\ )";
        res.eval_result += arg.genTex() + " = " + ArmaOperand(U).genTex() + ArmaOperand(S).genTex() 
                            + ArmaOperand(U).genTex() + R"(^*)";
        return;
    }

    if (func->op == TokName::QR)
    {
        arma::cx_mat Q, R;
        arma::qr(Q, R, arg.mat);
        res.eval_result = R"(QR decomposition: \\ )";
        res.eval_result += arg.genTex() + " = " + ArmaOperand(Q).genTex() + ArmaOperand(R).genTex();
        return;
    }

    if (func->op == TokName::SVD)
    {
        arma::cx_mat U, V;
        arma::vec s;
        arma::svd(U, s, V, arg.mat);
        arma::cx_mat S(arma::size(arg.mat), arma::fill::zeros);
        s.resize(arma::size(arg.mat.diag()));
        S.diag() = arma::cx_vec(s, arma::vec(arma::size(s), arma::fill::zeros));
        res.eval_result = R"(SVD: \\ )";
        res.eval_result += arg.genTex() + " = " + ArmaOperand(U).genTex() + ArmaOperand(S).genTex() 
                            + ArmaOperand(V).genTex() + R"(^T)";
        return;
    }
}


// do pre-order traversal on the AST and print the infix notation of the expression
// only used in pure CLI mode
void Parser::printAst(ExprAst* root) const
{
    if (!root)  return;
    if (typeid(*root) == typeid(BinaryExprAst))
    {
        BinaryExprAst* temp = dynamic_cast<BinaryExprAst*>(root);

        cout << "(";
        printAst(temp->lhs);
        
        cout << " " << temp->raw << " ";

        printAst(temp->rhs);
        cout << ")";
    }
    else if (typeid(*root) == typeid(NumberExprAst))
    {
        NumberExprAst* temp = dynamic_cast<NumberExprAst*>(root);
        if (temp->name == TokName::INTEGRAL)
            cout << stol(temp->raw);
        else if (temp->name == TokName::FLOAT)
            cout << stod(temp->raw);
        else cout << temp->raw;
    }
    else if (typeid(*root) == typeid(VariableExprAst))
    {
        VariableExprAst* temp = dynamic_cast<VariableExprAst*>(root);
        cout << temp->name;
    }
    else if (typeid(*root) == typeid(MatrixExprAst))
    {
        MatrixExprAst* temp = dynamic_cast<MatrixExprAst*>(root);
        cout << "[";
        for (auto row_it = temp->entries.begin(); row_it != temp->entries.end() - 1; ++row_it)
        {
            cout << "[";
            for (auto col_it = row_it->begin(); col_it != row_it->end() - 1; ++col_it)
            {
                printAst(*col_it);
                cout << ", ";
            }
            printAst(*(row_it->end() - 1));
            cout << "], ";
        }

        cout << "[";
        auto last_row = (temp->entries.end() - 1);
        for (auto col_it = last_row->begin(); col_it != last_row->end() - 1; ++col_it)
        {
            printAst(*col_it);
            cout << ", ";
        }
        printAst(*(last_row->end() - 1));
        cout << "]]";
    }
    else if (typeid(*root) == typeid(FunctionExprAst))
    {
        FunctionExprAst* temp = dynamic_cast<FunctionExprAst*>(root);
        if (temp->raw == "|")
        {
            cout << "|";
            printAst((temp->args[0]));
            cout << "|";
        }
        else
        {
            cout << temp->raw << "(";
            for (auto it = temp->args.begin(); it != temp->args.end() - 1; ++it)
            {
                printAst(*it);
                cout << ", ";
            }
            printAst(*(temp->args.end() - 1));
            cout << ")";
        }        
    }
    else throw std::runtime_error("Fail to print AST, exiting.");
}


void Parser::print() const
{
    printAst(root);
}