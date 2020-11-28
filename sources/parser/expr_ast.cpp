#include "expr_ast.h"
using std::vector;
using std::string;

typedef Token::TokName TokName;
typedef Token::TokType TokType;


// cannot simply use the bin_op_precedence in Parser class due to cross-referencing
std::unordered_map<TokName, int> BinaryExprAst::precedence = 
{
    {TokName::PLUS, 100}, {TokName::MINUS, 100}, 
    {TokName::AST, 200}, {TokName::CDOT, 200}, {TokName::DIV, 200}, {TokName::FRAC, 200}, {TokName::PERCENT, 200}, 
    {TokName::CROSS, 300}, {TokName::SUP, 400}
};


ROperand NumberExprAst::evalR()
{
    switch (name)
    {
        case TokName::INTEGRAL:
            return ROperand(newInt(static_cast<long>(int_value)));
        
        case TokName::FLOAT:
        {                // not really recommended, use Armadillo for approximated solutions
            long int_part = static_cast<long>(float_value);
            long float_part = static_cast<long>((float_value - int_part) * 1e6);    // naive implementation, avoid to use it
            return ROperand(newFrac(newInt(int_part), newInt(1L)) + newFrac(newInt(float_part), newInt(1e6L)));
        }

        case TokName::E:
            return ROperand(newInt(E));

        case TokName::PI:
            return ROperand(newInt(PI));

        case TokName::I:
            return ROperand(newComplex(0L, 1L));

        case TokName::IDENTITY_MATRIX:
            return ROperand();

        default:
            throw std::runtime_error("Error: unknown numerical value.");
    }
}


ArmaOperand NumberExprAst::eval()
{
    switch (name)
    {
        case TokName::INTEGRAL: 
            return ArmaOperand(static_cast<double>(int_value));
        
        case TokName::FLOAT:
            return ArmaOperand(float_value);

        case TokName::E: 
            return ArmaOperand(arma::datum::e);

        case TokName::PI:
            return ArmaOperand(arma::datum::pi);

        case TokName::I: 
            return ArmaOperand(0.0, 1.0);

        case TokName::IDENTITY_MATRIX:
            return ArmaOperand();

        default:
            throw std::runtime_error("Error: unknown numerical value.");
    }
}


ROperand MatrixExprAst::evalR()
{
    ROperand matrix(entries.size());
    for (size_t row = 0; row < entries.size(); ++row)
        for (size_t col = 0; col < entries[row].size(); ++col)
        {
            ROperand entry = entries[row][col]->evalR();
            
            if (entry.type != ROperand::Type::NOR)
                throw std::runtime_error("Nested matrices are not supported currently.");

            matrix.mat[row].push_back(entry.value);
        }
    return matrix;
}


ArmaOperand MatrixExprAst::eval()
{
    ArmaOperand matrix(entries.size(), entries[0].size());
    for (size_t row = 0; row < entries.size(); ++row)
        for (size_t col = 0; col < entries[row].size(); ++col)
        {
            ArmaOperand entry = entries[row][col]->eval();

            if (entry.type != ArmaOperand::Type::NOR)
                throw std::runtime_error("Nested matrices are not supported currently.");

            matrix.mat(row, col) = entry.value;
        }
    return matrix;
}


string MatrixExprAst::genAsciiMath() const
{
    string result = "[";
    for (auto row_it = entries.begin(); row_it != entries.end(); ++row_it)
    {
        result += "[";
        for (auto col_it = row_it->begin(); col_it != row_it->end(); ++col_it)
        {
            result += (*col_it)->genAsciiMath();
            if (col_it != row_it->end() - 1)    result += ", ";
        }
        if (row_it != entries.end() - 1) result += "], ";
        else result += "]]";
    }
    return result;
}


ROperand VariableExprAst::evalR()
{   
    return ROperand(newTerm(1));
}


ArmaOperand VariableExprAst::eval()
{
    // TODO
    return ArmaOperand(1);
}


ROperand BinaryExprAst::evalR()
{
    ROperand lhsR = lhs->evalR();
    ROperand rhsR = rhs->evalR();
    
    switch (op)
    {
        case TokName::PLUS:
            return lhsR + rhsR;

        case TokName::MINUS:
            return lhsR - rhsR;

        case TokName::AST: case TokName::CDOT: case TokName::CROSS:         // so far do not distinguish cross cdot and *
            return lhsR * rhsR;

        case TokName::DIV: case TokName::FRAC:
            return lhsR / rhsR;

        case TokName::SUP:              // exponential, may be enhanced in the future
            if (typeid(*rhs) == typeid(NumberExprAst))
            {
                NumberExprAst* temp = dynamic_cast<NumberExprAst*>(rhs);
                if (temp->name == TokName::INTEGRAL)    return lhsR ^ (static_cast<int>(temp->int_value));
            }
            else throw std::runtime_error("Error: unsupported exponential operation, expecting a natural number as exponential.");       
            
        default:
            throw std::runtime_error("Error: unsupported binary operation.");   // mod may be added soon
    }
}


ArmaOperand BinaryExprAst::eval()
{
    ArmaOperand left_operand = lhs->eval();
    ArmaOperand right_operand = rhs->eval();
    
    switch (op)
    {
        case TokName::PLUS:
            return left_operand + right_operand;

        case TokName::MINUS:
            return left_operand - right_operand;

        case TokName::AST: case TokName::CDOT: case TokName::CROSS:         
            return left_operand * right_operand;

        case TokName::DIV: case TokName::FRAC:
            return left_operand / right_operand;

        case TokName::SUP:              
            return left_operand ^ right_operand;
            
        default:
            throw std::runtime_error("Error: unsupported binary operation.");
    }
}


string BinaryExprAst::genAsciiMath() const
{
    string lhs_str = lhs->genAsciiMath(), rhs_str = rhs->genAsciiMath();

    // add back parentheses to suitable places
    if (typeid(*lhs) == typeid(BinaryExprAst))
    {
        BinaryExprAst* temp = dynamic_cast<BinaryExprAst*>(lhs);
        if (precedence[temp->op] < precedence[op])  
            lhs_str = "(" + lhs_str + ")";          // left hand side binary expression has lower precedence but grouped together
    }

    if (typeid(*rhs) == typeid(BinaryExprAst))
    {
        BinaryExprAst* temp = dynamic_cast<BinaryExprAst*>(rhs);
        if (precedence[temp->op] <= precedence[op]) rhs_str = "(" + rhs_str + ")";          // similar as above
    }
    
    return lhs_str + raw + rhs_str;
}


ROperand FunctionExprAst::evalR()
{
    // TODO: need to check the implementation for row reduction, etc

    switch (op)
    {
        case TokName::NEG:  return -(args[0]->evalR());
        default: throw std::runtime_error("function " + raw + " is not supported yet");
    }
}


// supported functions
// SIN, COS, TAN, ARCSIN, ARCCOS, ARCTAN, EXP, LN,
// DET, DIM, RAN, COL, KER, MOD, TR,
// RREF, NORM, ABS, SQRT, ROOT
ArmaOperand FunctionExprAst::eval()
{
    typedef ArmaOperand::Type Type;

    int num_args = args.size();
    switch (op)
    {
        case TokName::SOLVE:
            if (num_args != 1 && num_args != 2) 
                throw std::runtime_error("expect one or two arguments for " + raw + "()");

        case TokName::MIN: case TokName::MAX: case TokName::ROOTS:
            break;          // min, max, roots can have arbitrary number of arguments

        case TokName::ROOT: case TokName::LOG: case TokName::GCD: case TokName::LCM:
            if (num_args != 2)  throw std::runtime_error("expect two arguments for " + raw + "()");
            break;

        default:
            if (num_args != 1)  throw std::runtime_error("expect one argument for " + raw + "()");
    }

    vector<ArmaOperand> arg_vals;
    for (auto arg : args)
        arg_vals.emplace_back(arg->eval());

    if (arg_vals.size() == 2)
        switch (op)
        {
            case TokName::ROOT:
                if (arg_vals[0].type == Type::IMAT)
                    throw std::runtime_error("root funtion for indentity matrix is invalid");
                if (arg_vals[0].type == Type::MAT)
                {
                    if (!arg_vals[1].almostReal())  
                        throw std::logic_error("complex value for root is unsupported");
                    return ArmaOperand(arma::powmat(arg_vals[0].mat, 1 / arg_vals[1].value.real()));
                }
                return ArmaOperand(pow(arg_vals[0].value, std::complex<double>(1, 0) / arg_vals[1].value));

            case TokName::SOLVE:
            {
                if (arg_vals[0].type != Type::MAT || arg_vals[1].type != Type::MAT)
                    throw std::logic_error("both arguments for function solve() should be matrices");
                if (arg_vals[0].type == Type::IMAT && arg_vals[1].type == Type::IMAT)
                    throw std::logic_error("two arguments for function solve() cannot be identity matrix with no size simultaneously");

                arma::cx_mat A = arg_vals[0].mat, B = arg_vals[1].mat;
                if (arg_vals[0].type == Type::IMAT)
                    A = arg_vals[0].fromImat(B.n_rows);
                if (arg_vals[1].type == Type::IMAT)
                    B = arg_vals[1].fromImat(A.n_rows);
                
                return ArmaOperand(arma::solve(A, B));
            }

            default: throw std::runtime_error("unsupported function: " + raw);
        }


    if (arg_vals[0].type == Type::MAT)
        switch (op)
        {
            // using Armadillo's built-in element-wise function and linear operations
            case TokName::NEG:  return ArmaOperand(-arg_vals[0].mat);

            case TokName::SIN:  return ArmaOperand(arma::sin(arg_vals[0].mat));

            case TokName::COS:  return ArmaOperand(arma::cos(arg_vals[0].mat));
            
            case TokName::TAN:  return ArmaOperand(arma::tan(arg_vals[0].mat));

            case TokName::ARCSIN:   return ArmaOperand(arma::asin(arg_vals[0].mat));

            case TokName::ARCCOS:   return ArmaOperand(arma::acos(arg_vals[0].mat));

            case TokName::ARCTAN:   return ArmaOperand(arma::atan(arg_vals[0].mat));

            case TokName::EXP:  return ArmaOperand(arma::exp(arg_vals[0].mat));

            case TokName::LN:   return ArmaOperand(arma::log(arg_vals[0].mat));

            case TokName::DET:  return ArmaOperand(arma::det(arg_vals[0].mat));

            case TokName::RANK: return ArmaOperand(arma::rank(arg_vals[0].mat));

            case TokName::ORTH: return ArmaOperand(arma::orth(arg_vals[0].mat));

            case TokName::KER:  return ArmaOperand(arma::null(arg_vals[0].mat));

            case TokName::TR:   return ArmaOperand(arma::trace(arg_vals[0].mat));

            case TokName::NORM: return ArmaOperand(arma::norm(arg_vals[0].mat));

            case TokName::INV:  return ArmaOperand(arma::inv(arg_vals[0].mat));

            case TokName::PINV: return ArmaOperand(arma::pinv(arg_vals[0].mat));

            case TokName::SOLVE:
            {
                size_t num_cols = arg_vals[0].mat.n_cols;
                arma::cx_vec B = arg_vals[0].mat.col(num_cols - 1);
                return ArmaOperand(arma::solve(arg_vals[0].mat.cols(1, num_cols -2), B));
            }

            case TokName::ABS:  
                return ArmaOperand(arma::cx_mat(arma::abs(arg_vals[0].mat), arma::mat(arma::size(arg_vals[0].mat), arma::fill::zeros)));

            case TokName::SQRT: return ArmaOperand(arma::sqrt(arg_vals[0].mat));

            default: throw std::runtime_error("unsupported function: " + raw);
        }

    switch (op) 
    {
        case TokName::NEG:  return ArmaOperand(-arg_vals[0].value);

        case TokName::SIN:  return ArmaOperand(std::sin(arg_vals[0].value));
        
        case TokName::COS:  return ArmaOperand(std::cos(arg_vals[0].value));

        case TokName::TAN:  return ArmaOperand(std::tan(arg_vals[0].value));

        case TokName::ARCSIN:   return ArmaOperand(std::asin(arg_vals[0].value));

        case TokName::ARCCOS:   return ArmaOperand(std::acos(arg_vals[0].value));

        case TokName::ARCTAN:   return ArmaOperand(std::atan(arg_vals[0].value));

        case TokName::EXP:  return ArmaOperand(std::exp(arg_vals[0].value));

        case TokName::LN:   return ArmaOperand(std::log(arg_vals[0].value));

        case TokName::ABS:  return ArmaOperand(std::abs(arg_vals[0].value));

        case TokName::SQRT: 
            if (arg_vals[0].type == Type::IMAT)
                throw std::logic_error("square root cannot be applied to identity matrix");
            return ArmaOperand(std::sqrt(arg_vals[0].value));

        case TokName::NORM:
            if (arg_vals[0].type == Type::IMAT)
                throw std::logic_error("norm function cannot be applied to identity matrix");
            return ArmaOperand(std::norm(arg_vals[0].value));

        default: throw std::runtime_error("unsupported function " + raw);
    }
}


string FunctionExprAst::genAsciiMath() const
{
    string result = raw + "(";
    for (auto it = args.begin(); it != args.end() - 1; ++it)
        result += (*it)->genAsciiMath() + ", ";
    return result + (*(args.end() - 1))->genAsciiMath() + ")";
}