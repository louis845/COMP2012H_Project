#include "parser.h"
#include "math/linear/LinearOperations.h"
using std::cout;
using std::endl;
using std::string;

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

    for (auto element : var_table)
        delete element.second;

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
    while (cur_tok && cur_tok->get_name() == TokName::ENDL)
    {
        delete cur_tok;
        cur_tok = tokenizer.getNextToken();
    }

    if (cur_tok && cur_tok->get_name() == TokName::EQUAL)
        throw std::invalid_argument("expression contains invalid symbol '='");
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
//          ::= '[' bracketexpr ( ',' bracketexpr )* ']'
//          ::= '(' bracketexpr ( ',' bracketexpr )* ')'
MatrixExprAst* Parser::parseMatrix()
{
    if (cur_tok == nullptr || (cur_tok->get_name() != TokName::LSB && cur_tok->get_name() != TokName::LP))
        return nullptr;

    TokName expected_paren_type = cur_tok->get_name() == TokName::LP ? TokName::RP : TokName::RSB;
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
    }

    if (cur_tok == nullptr || cur_tok->get_name() != expected_paren_type)
    {
        delete matrix;
        if (expected_paren_type == TokName::RP)
            throw std::invalid_argument("missing right parenthesis for the matrix");
        else
            throw std::invalid_argument("missing right square bracket for the matrix");
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
    if (cur_tok == nullptr || cur_tok->get_type() == TokType::ERR)     return nullptr;

    if (cur_tok->get_name() == TokName::LP)
    {
        getNextToken();
        ExprAst* temp = parseExpr();
        if (cur_tok == nullptr || cur_tok->get_name() != TokName::RP)
        {
            delete temp;
            throw std::invalid_argument("missing right parenthesis");
        }

        getNextToken();
        return temp;
    }
    
    if (cur_tok->get_name() == TokName::TEXTBAR)
    {
        getNextToken();
        ExprAst* temp = parseExpr(true);
        if (cur_tok == nullptr || cur_tok->get_name() != TokName::TEXTBAR)
        {
            delete temp;
            throw std::invalid_argument("missing closing textbar");
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
        var_table.emplace(cur_tok->get_raw_value(), nullptr);
        getNextToken();
        return var;
    }
    if (cur_tok->get_type() != TokType::OP || getTokPrecedence() != -1)     
        return nullptr;     // binops are not functions

    FunctionExprAst* func = new FunctionExprAst(cur_tok->get_name(), cur_tok->get_raw_value());
    getNextToken();

    if (cur_tok == nullptr)
    {
        delete func;
        return nullptr;
    }

    if (cur_tok->get_type() == TokType::NUM)
    {
        func->args.emplace_back(parseNum());
        return func;
    }

    if (cur_tok->get_type() == TokType::ID)
    {
        ExprAst* var = new VariableExprAst(cur_tok->get_raw_value());
        var_table.emplace(cur_tok->get_raw_value(), nullptr);
        func->args.emplace_back(var);
        getNextToken();
        return func;
    }

    string backup = tokenizer.get_input();
    if (cur_tok->get_name() == TokName::LP)
    {
        getNextToken();
        try
        {
            ExprAst* expr = parseExpr();
            if (expr != nullptr)
            {
                func->args.emplace_back(expr);
                while (cur_tok && cur_tok->get_name() == TokName::COMMA)
                {
                    getNextToken();
                    expr = parseExpr();
                    if (expr == nullptr)
                    {
                        delete func;
                        return nullptr;
                    }
                    func->args.emplace_back(expr);
                }

                if (cur_tok == nullptr || cur_tok->get_name() != TokName::RP)
                    throw std::invalid_argument("missing right parenthesis for function");

                getNextToken();
                return func;
            }
            else 
            {
                tokenizer.reset_input(backup);      // use time traval to resolve undetermined parsing state
                delete cur_tok;
                cur_tok = new TokOp("(");
            }
        }
        catch (...)
        {               
            tokenizer.reset_input(backup);
            delete cur_tok;
            cur_tok = new TokOp("(");
        }
    }

    if (cur_tok->get_name() == TokName::LSB || cur_tok->get_name() == TokName::LP)
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
        return parseNum();

    if (cur_tok->get_name() == TokName::TEXTBAR) 
        return parseParen();

    string backup = tokenizer.get_input();
    if (cur_tok->get_name() == TokName::LP)
    {
        try
        {
            ExprAst* parenexpr = parseParen();
            if (parenexpr)  return parenexpr;
            tokenizer.reset_input(backup);          // time travel
            delete cur_tok;
            cur_tok = new TokOp("(");
        }
        catch (...)
        {
            tokenizer.reset_input(backup);          // time travel
            delete cur_tok;
            cur_tok = new TokOp("(");
        } 
    }

    if (cur_tok->get_name() == TokName::LSB || cur_tok->get_name() == TokName::LP)
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
        std::string cur_raw_str;

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
            if (cur_tok->get_name() == TokName::MINUS)         // unitary minus
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
                if (cur_tok->get_name() == TokName::PLUS)   getNextToken();     // unitary plus
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
    if (cur_tok == nullptr) return nullptr;
    switch (cur_tok->get_name())
    {
        // decomposition needs special handling
        case TokName::SVD: case TokName:: SCHUR: case TokName::QR: case TokName::EIGEN:
        {
            FunctionExprAst* decomp = new FunctionExprAst(cur_tok->get_name(), cur_tok->get_raw_value());
            getNextToken();
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
        if (!expr)   
        {   
            delete neg_expr;
            throw std::runtime_error("Fatal error, unexpected exception occurs.");
        }
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
            case TokName::INV: case TokName::CHAR_POLY: case TokName::NEG:
            case TokName::ORTH:
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

    if (engine_type == 3 || input.find('=') != std::string::npos)
    {
        res.engine_used = 3;
        try
        {
            evalLinearSystem(parseLinearSystem());
            return res;
        }
        catch (const std::exception& err)
        {
            res.success = false;
            res.err = new std::invalid_argument("Error: " + string(err.what()));
            return res;
        }
    }

    /*
    if (input.find('=') != std::string::npos)
    {
        res.success = false;
        res.err = new std::invalid_argument("Error: equal sign detected but computational engine is not on linear system mode \n you may want to manually select the correct engine");
        return res;
    }
    */

    try
    {
        delete root;
        root = parseExpr();
        if (root == nullptr)    return res;
        res.interpreted_input = getAsciiMath();
    }
    catch (const std::invalid_argument& err)
    {
        string err_msg = "Error: parsing failed, " + string(err.what());
        res.err = new std::invalid_argument(err_msg.c_str());
        // delete root;
        res.success = false;
        return res;
    }
    catch (...)
    {
        res.err = new std::runtime_error("Fatal error: unexpected exception thrown during parsing");
        // delete root;
        res.success = false;
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
        if (typeid(*root) == typeid(MatrixExprAst)) 
            res.addMat(result, TokName::NA);
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
        res.eval_result = R"(Schur~decomposition: \\ )";
        res.eval_result += arg.genTex() + " = " + ArmaOperand(U).genTex() + ArmaOperand(S).genTex() 
                            + ArmaOperand(U).genTex() + R"(^*)";
        return;
    }

    if (func->op == TokName::QR)
    {
        arma::cx_mat Q, R;
        arma::qr(Q, R, arg.mat);
        res.eval_result = R"(QR~decomposition: \\ )";
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


void Parser::modifyCoeffs(const std::string& name, size_t row, bool neg,
                        std::map<std::string, std::map<int, ROperand*>>& coeffs, ROperand delta)
{
    if (neg) delta = -delta;
    if (coeffs[name].count(row))
        *coeffs[name][row] += delta;
    else
        coeffs[name][row] = new ROperand(delta);
}



// EXPERIMENTAL: dedicated parser for parsing linear systems
// it is a feature added at the very end of the development
// therefore no major grammar modification on the main parser can be adopted for compatibility
// EXTREMELY UNRELIABLE, I myself do not believe this will work

// it works, how come??
std::vector<std::string> Parser::parseLinearSystem()
{
    string lines = input;
    size_t pos = lines.find(R"(\\)"), row_cnt = 0;
    std::map<string, std::map<int, ROperand*>> coeffs;
    while (true)
    {
        pos = pos == string::npos ? lines.size() : pos;
        string line = lines.substr(0, pos);
        size_t eq_pos = line.find('=');
        if (eq_pos == string::npos) 
            throw std::invalid_argument("invalid linear equation, missing '='");
        
        try
        {
            parseLine(line.substr(0, eq_pos), row_cnt, false, coeffs);
            parseLine(line.substr(eq_pos + 1), row_cnt++, true, coeffs);
        }
        catch (...)
        {
            for (auto var : coeffs)
                for (auto row : var.second)
                    delete row.second;
            throw;
        }

        if (pos == lines.size()) break;
        lines = lines.substr(pos + 2);
        pos = lines.find(R"(\\)");
    }

    vector<string> var_names;
    for (auto var : coeffs)
    {
        if (var.first == "constant") continue;
        var_names.emplace_back(var.first);
    }
    var_names.emplace_back("constant");

    R** matR = new R* [row_cnt];
    for (size_t i = 0; i < row_cnt; ++i)
    {
        matR[i] = new R [coeffs.size()];
        for (size_t j = 0; j < var_names.size() - 1; ++j)
        {
            if (coeffs[var_names[j]].count(i))    
                matR[i][j] = coeffs[var_names[j]].at(i)->value;
            else matR[i][j] = newInt(0L);
        }
        if (coeffs["constant"].at(i))
            matR[i][var_names.size() - 1] = -coeffs["constant"].at(i)->value;
        else
            matR[i][var_names.size() - 1] = newInt(0L);
    }

    res.parsed_mat.emplace_back(TokName::SOLVE, matR);
    res.mat_size.emplace_back(std::make_pair(row_cnt, var_names.size()));

    for (auto var : coeffs)
        for (auto row : var.second)
            delete row.second;

    return var_names;
}


// I assert that all the nodes containing variables will be in the right subtree
// except for the left-most leaf, if the input is well formatted
void Parser::parseLine(std::string line, size_t row, bool neg, 
                        std::map<std::string, std::map<int, ROperand*>>& coeffs)
{
    Parser parser(line);
    ExprAst* ast_root = parser.parseExpr(), *cur_root = ast_root;
    if (ast_root == nullptr)    throw std::invalid_argument("invalid linear system");
    Info dummy_info;
    dummy_info.engine_used = 3;
    while (true)
    {
        if (cur_root == nullptr)    
        {
            delete ast_root;
            throw std::invalid_argument("the equation is not linear or not in valid form");
        }
        if (typeid(*cur_root) == typeid(NumberExprAst))
        {
            modifyCoeffs("constant", row, neg, coeffs, ROperand(cur_root->evalR(dummy_info)));
            break;
        }
        else if (typeid(*cur_root) == typeid(VariableExprAst))
        {
            VariableExprAst* temp = dynamic_cast<VariableExprAst*>(cur_root);
            if (temp->name != "t")
                modifyCoeffs(temp->name, row, neg, coeffs, ROperand(newInt(1L)));
            else
                modifyCoeffs("constant", row, neg, coeffs, ROperand(newTerm(1)));

            break;
        }
        else if (typeid(*cur_root) == typeid(BinaryExprAst))
        {
            BinaryExprAst* temp = dynamic_cast<BinaryExprAst*>(cur_root);
            if (temp->op != TokName::PLUS && temp->op != TokName::MINUS)
            {
                try
                {
                    string lhs = findVar(temp->lhs), rhs = findVar(temp->rhs);
                    if (lhs.empty() && rhs.empty())
                    {
                        modifyCoeffs("constant", row, neg, coeffs, ROperand(temp->evalR(dummy_info)));
                        break;
                    }
                    if (!lhs.empty() && !rhs.empty())
                    {
                        delete ast_root;
                        throw std::invalid_argument("the equation is not linear or not in valid form");
                    }
                    if (lhs.empty()) swap(lhs, rhs);
                    modifyCoeffs(lhs, row, neg, coeffs, ROperand(temp->evalR(dummy_info)));
                    break;
                }
                catch (...)
                {
                    delete ast_root;
                    throw;
                }
            }
            
            bool local_neg = neg == (temp->op == TokName::PLUS);
            ExprAst* lhs = temp->lhs, *rhs = temp->rhs;
            string var_name = findVar(rhs);

            if (var_name.empty())   
                modifyCoeffs("constant", row, local_neg, coeffs, ROperand(rhs->evalR(dummy_info)));
            else
                modifyCoeffs(var_name, row, local_neg, coeffs, ROperand(rhs->evalR(dummy_info)));

            cur_root = lhs;
        }
        else if (typeid(*cur_root) == typeid(FunctionExprAst))
        {
            FunctionExprAst* temp = dynamic_cast<FunctionExprAst*>(cur_root);
            if (temp->op != TokName::NEG)
            { 
                delete ast_root;
                throw std::invalid_argument("the equation is not linear or not in valid form");
            }
            cur_root = temp->args[0];
            neg = !neg;
        }
        else
        {
            delete ast_root;
            throw std::invalid_argument("the equation is not linear or not in valid form");
        }
    }
    delete ast_root;
}


string Parser::findVar(ExprAst* root)
{
    if (typeid(*root) == typeid(BinaryExprAst))
    {
        BinaryExprAst* temp = dynamic_cast<BinaryExprAst*>(root);
        string lhs = findVar(temp->lhs), rhs = findVar(temp->rhs);
        if (lhs != rhs && !lhs.empty() && !rhs.empty())
            throw std::invalid_argument("the equation is not linear or not in valid form, e.g. group two unknowns with parentheses");
        if (!lhs.empty())  return lhs;
        return rhs;
    }
    
    if (typeid(*root) == typeid(FunctionExprAst))
    {
        FunctionExprAst* temp = dynamic_cast<FunctionExprAst*>(root);
        if (temp->op == TokName::NEG)   return findVar(temp->args[0]);
        for (auto arg : temp->args)
            if (!findVar(arg).empty())
                throw std::invalid_argument("the equation is not linear or not in valid form");
    }

    if (typeid(*root) == typeid(VariableExprAst))
    {
        VariableExprAst* temp = dynamic_cast<VariableExprAst*>(root);
        return temp->name == "t" ? "" : temp->name;
    }

    return "";
}


void Parser::evalLinearSystem(vector<string> var_names)
{
    StepsHistory* steps{nullptr};
    LinearOperationsFunc::solve(res.parsed_mat.back().second, res.mat_size.back().first,
                                res.mat_size.back().second, steps);
    if (steps == nullptr)
        throw std::invalid_argument("fail to solve the linear system, please check the input");
    R** matR{nullptr};
    int row, col;
    steps->getAnswer(matR, row, col);
    if (matR == nullptr)
    {
        delete steps;
        throw std::invalid_argument("fail to solve the linear system, the input may be invalid or it has not solution");
    }
    
    res.eval_result = "&";
    for (int i = 0; i < row; ++i)
    {
        res.eval_result += var_names[i] + " = " + matR[i][0].to_latex() + R"( \quad )";
        if (i != 0 && i % 3 == 0 && i != row - 1) res.eval_result += R"( \\ &)";
    }

    res.interpreted_input = R"(text(please check the step-by-step section for the interpreted augmented matrix))";
    res.interpreted_input += R"(text(the columns, from left to right, correspond to ))";
    for (size_t i = 0; i < var_names.size() - 1; ++i)
        res.interpreted_input += var_names[i] + " ";

    for (int i = 0; i < row; ++i)
        delete [] matR[i];
    delete [] matR;
    delete steps;
}


void Parser::print() const
{
    printAst(root);
}


string Parser::getAsciiMath() const
{
    if (root == nullptr)    return "";
    return root->genAsciiMath();
}