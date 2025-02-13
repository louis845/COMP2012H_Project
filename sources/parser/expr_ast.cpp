#include "expr_ast.h"
#include "math/linear/LinearOperations.h"
using std::vector;
using std::string;

typedef Token::TokName TokName;
typedef Token::TokType TokType;


// Used in BinaryExprAst::genAsciiMath()
// Cannot simply use the bin_op_precedence in Parser class due to cross-referencing
std::unordered_map<TokName, int> BinaryExprAst::precedence = 
{
    {TokName::PLUS, 100}, {TokName::MINUS, 100}, 
    {TokName::AST, 200}, {TokName::CDOT, 200}, {TokName::DIV, 200}, {TokName::FRAC, 200}, {TokName::PERCENT, 200},
    {TokName::CROSS, 300}, {TokName::SUP, 400}
};


Info::~Info()
{
    for (size_t i = 0; i < parsed_mat.size(); ++i)
    {
        R** matR = parsed_mat[i].second;
        size_t n_rows = mat_size[i].first, n_cols = mat_size[i].second;
        for (size_t j = 0; j < n_rows; ++j) delete [] matR[j];
        delete [] matR;
        matR = nullptr;
    }
}


void Info::clear()
{
    success = true;
    engine_used = 1;
    for (size_t i = 0; i < parsed_mat.size(); ++i)
    {
        R** mat = parsed_mat[i].second;
        size_t row = mat_size[i].first;
        for (size_t j = 0; j < row; ++j)    delete [] mat[j];
    }
    mat_size.clear();
    parsed_mat.clear();
    interpreted_input = eval_result = err_msg = "";
    float_exists = var_exists = func_exists = false;
}


void Info::addMat(ROperand operand, TokName op)
{
    if (operand.type != ROperand::Type::MAT)   return;
    auto &mat = operand.mat;
    auto n_rows = mat.size(), n_cols = mat[0].size();

    R** matR = new R* [n_rows];
    for (auto i = 0; i < n_rows; ++i)   matR[i] = new R [n_cols];

    for (auto i = 0; i < n_rows; ++i)
        for (auto j = 0; j < n_cols; ++j)
            matR[i][j] = mat[i][j];

    parsed_mat.emplace_back(std::make_pair(op, matR));
    mat_size.emplace_back(std::make_pair(n_rows, n_cols));
}


// Check whether the raw input contains 'e' or 'E' as a sign of scientific notation
NumberExprAst::NumberExprAst(TokName name, const string& raw): name(name), raw(raw)
{
    if (raw.find('e') != string::npos || raw.find('E') != string::npos) 
        scientific = true;
}


string NumberExprAst::genAsciiMath() const
{
    if (raw.find('e') != string::npos || raw.find('E') != string::npos)
        return "text(" + raw + ")";
    return raw;
}


ROperand NumberExprAst::evalR(Info& res)
{
    switch (name)
    {
        case TokName::INTEGRAL:
        {
            // mpz_wrapper does not support converting strings with
            // scientific notation to their integral types
            // the conversion needs to be done using stol() instead
            //
            // It means that when using scientific notation to represent
            // a large number exceeding the range of long type, there will
            // be a std::out_of_range exception thrown
            if (scientific)
            {
                try 
                { 
                    long value = stol(raw); 
                    return ROperand(newInt(value));
                }
                catch (const std::invalid_argument& err)
                {
                    throw std::invalid_argument(raw + " is not a valid numerical value");
                }
                catch (const std::out_of_range& err)
                {
                    throw std::out_of_range(raw + " contains scientific notation but is out of range of built-in long type");
                }
            }
            else return ROperand(newInt(raw));
        }
        
        // interpret floating point numbers, e and pi for R class
        // floats will be converted to fractions with truncated precision for R
        // as a result, users should avoid using R when tacking floats
        // unless the fraction representation is expected
        case TokName::FLOAT: case TokName::E: case TokName::PI:
        {
            try 
            { 
                double value = arma::datum::e;
                if (name == TokName::FLOAT) value = stod(raw);
                else if (name == TokName::PI) value = arma::datum::pi;

                // The naive implementation simply truncated the first six effective digits
                long int_part = static_cast<long>(value);
                long float_part = static_cast<long>((value - int_part) * 1e6);
                return ROperand(newFrac(newInt(int_part), newInt(1L)) + newFrac(newInt(float_part), newInt(1e6L))); 
            }
            catch (const std::invalid_argument& err)
            {
                throw std::invalid_argument(raw + " is not a valid numerical value");
            }
            catch (const std::out_of_range& err)
            {
                throw std::invalid_argument(raw + " contains scientific notation but is out of range of built-in double type");
            }
        }

        // I is the imaginary number i
        case TokName::I:
            return ROperand(newComplex(0L, 1L));

        // Return an identity matrix with coefficient one
        case TokName::IDENTITY_MATRIX:
            return ROperand();

        // You know why
        case TokName::ANSWER:
            return ROperand(newInt(42));

        default:
            throw std::runtime_error("unknown numerical value.");
    }
}


ArmaOperand NumberExprAst::eval()
{
    switch (name)
    {
        case TokName::INTEGRAL: case TokName::FLOAT: 
            try 
            {
                return ArmaOperand(stod(raw)); 
            }
            catch (const std::invalid_argument& err)
            {
                throw std::invalid_argument(raw + " is not a valid numerical value");
            }
            catch (const std::out_of_range& err)
            {
                throw std::invalid_argument(raw + " is out of range of built-in double type");
            }

        case TokName::E: 
            return ArmaOperand(arma::datum::e);

        case TokName::PI:
            return ArmaOperand(arma::datum::pi);

        case TokName::I: 
            return ArmaOperand(0.0, 1.0);

        case TokName::IDENTITY_MATRIX:
            return ArmaOperand();

        case TokName::ANSWER:
            return ArmaOperand(42.0);

        default:
            throw std::runtime_error("unknown numerical value.");
    }
}


// This function will first evaluate every AST node in the matrix
// if any result of them is not a scalar, a std::invalid_argument
// will be thrown
ROperand MatrixExprAst::evalR(Info& res)
{
    ROperand matrix(entries.size());
    for (size_t row = 0; row < entries.size(); ++row)
        for (size_t col = 0; col < entries[row].size(); ++col)
        {
            ROperand entry = entries[row][col]->evalR(res);
            
            if (entry.type != ROperand::Type::NOR)
                throw std::invalid_argument("nested matrices are not supported currently.");

            matrix.mat[row].push_back(entry.value);
        }
    return matrix;
}


// It works similarly to eval(), but uses the internal data type
// from Armadillo
// 
// We do not use template here because the type it accepts is highly
// specialised and it actually add more code if use template
ArmaOperand MatrixExprAst::eval()
{
    ArmaOperand matrix(entries.size(), entries[0].size());
    for (size_t row = 0; row < entries.size(); ++row)
        for (size_t col = 0; col < entries[row].size(); ++col)
        {
            ArmaOperand entry = entries[row][col]->eval();

            if (entry.type != ArmaOperand::Type::NOR)
                throw std::invalid_argument("nested matrices are not supported currently.");

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


ROperand VariableExprAst::evalR(Info& res)
{   
    if (res.engine_used == 3 && name == "t") return ROperand(newTerm(1));
    if (res.engine_used == 3)   return ROperand(newInt(1L));   
    auto iter = r_table->find(name);
    if (iter != r_table->end()) return iter->second;

    if (arma_table->find(name) != arma_table->end())
        throw std::invalid_argument("variables evaluated using Armadillo cannot be used when using R");
        
    return ROperand(newTerm(1));
}


ArmaOperand VariableExprAst::eval()
{
    auto iter = arma_table->find(name);
    if (iter != arma_table->end()) return iter->second;
    throw std::invalid_argument("variable with unknown value is not supported in Armadillo");
}


string VariableExprAst::genAsciiMath() const
{
    auto pos = name.find('_');
    if (pos == string::npos) return name;
    return name.substr(0, pos + 1) + "{" + name.substr(pos + 1) + "}";
}


// Use the binary operators overloaded by ROperand in math_wrapper.h
ROperand BinaryExprAst::evalR(Info& res)
{
    ROperand lhsR = lhs->evalR(res);
    ROperand rhsR = rhs->evalR(res);

    if (lhsR.type == ROperand::Type::MAT) res.addMat(lhsR, TokName::NA);
    if (rhsR.type == ROperand::Type::MAT) res.addMat(rhsR, TokName::NA);
    
    switch (op)
    {
        case TokName::PLUS:
            return lhsR + rhsR;

        case TokName::MINUS:
            return lhsR - rhsR;

        // So far, it does not distinguish cross product and dot product
        case TokName::AST: case TokName::CDOT: case TokName::CROSS:
            return lhsR * rhsR;

        case TokName::DIV: case TokName::FRAC:
            return lhsR / rhsR;

        case TokName::SUP:
            // Exponential only support the orginal integer value
            // hence we need to do RTTI and check
            if (typeid(*rhs) == typeid(NumberExprAst))
            {
                NumberExprAst* temp = dynamic_cast<NumberExprAst*>(rhs);
                if (temp->name == TokName::INTEGRAL)    
                    try { return lhsR ^ (stoi(temp->raw)); }        // may overflow
                    catch (const std::invalid_argument& err)
                    {
                        throw std::invalid_argument(temp->raw + " is not a valid numerical value");
                    }
                    catch (const std::out_of_range& err)
                    {
                        throw std::invalid_argument(temp->raw + " is out of range of built-in int type");
                    }
            }
            else throw std::invalid_argument("unsupported exponential operation, expecting a natural number as exponential.");       
            
        default:
            throw std::invalid_argument("unsupported binary operation.");   // mod may be added soon
    }
}


// Use the overloaded operators in Armadillo
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
            throw std::invalid_argument("unsupported binary operation.");
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


// Currently, R support RREF, solving augmented matrices, inverse
// determinant, characteristic polynomials and orthogonalization
ROperand FunctionExprAst::evalR(Info& res)
{
    switch (op)
    {
        case TokName::NEG:  return -(args[0]->evalR(res));

        case TokName::RREF: case TokName::SOLVE: case TokName::INV: 
        case TokName::DET: case TokName::CHAR_POLY: case TokName::ORTH:
            break;

        default: throw std::invalid_argument("function " + raw + " is not supported in R yet");
    }

    if (args.size() != 1)
        throw std::invalid_argument(raw + " only accepts one matrix as argument, but got " + std::to_string(args.size()));

    ROperand operand = args[0]->evalR(res);
    if (operand.type == ROperand::Type::MAT) res.addMat(operand, op);
    else throw std::invalid_argument(raw + " operation only accept a matrix as argument");
    
    // Due to the implementation of linear operation
    // we have to first setup a StepsHistory* and use it to retrieve
    // the answers
    StepsHistory* steps{nullptr};
    switch (op)
    {
        case TokName::RREF:
            LinearOperationsFunc::row_reduce(res.parsed_mat.back().second, res.mat_size.back().first, 
                                            res.mat_size.back().second, steps);
            break;

        case TokName::SOLVE:
            LinearOperationsFunc::solve(res.parsed_mat.back().second, res.mat_size.back().first,
                                        res.mat_size.back().second, steps);
            break;

        case TokName::INV:
            LinearOperationsFunc::invert(res.parsed_mat.back().second, res.mat_size.back().first,
                                        res.mat_size.back().second, steps);
            break;

        case TokName::DET:
            LinearOperationsFunc::determinant(res.parsed_mat.back().second, res.mat_size.back().first,
                                            res.mat_size.back().second, steps);
            break;

        case TokName::CHAR_POLY:
            LinearOperationsFunc::char_poly(res.parsed_mat.back().second, res.mat_size.back().first,
                                            res.mat_size.back().second, steps);
            break;

        case TokName::ORTH:
            LinearOperationsFunc::orthogonalize(res.parsed_mat.back().second, res.mat_size.back().first,
                                            res.mat_size.back().second, steps);
            break;
    }

    if (steps == nullptr)   
        throw std::runtime_error("R operation " + raw + " failed, the matrix is not invertible/has no solution/is not square");

    R** matR{nullptr};
    int row, col;
    steps->getAnswer(matR, row, col);       // retrieve the final answer from the step history
    if (matR == nullptr)    
    {
        delete steps;
        throw std::runtime_error("R operation " + raw + " failed, the matrix is not invertible/has no solution/is not square");
    }

    if (row == 1 && col == 1)           // matrix degenerates to a scalar
    {
        ROperand result(matR[0][0]);
        delete [] matR[0];
        delete [] matR;
        delete steps;
        return result;
    }

    ROperand result(row);               // converting the R** to ROperand and deallocate R**
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
            result.mat[i].emplace_back(matR[i][j]);
        delete [] matR[i];
    }
    delete [] matR;
    delete steps;

    return result;
}


// Function evaluation when use Armadillo engine
// supported functions:
// sin, cos, tan and their inverse functions
// exp, ln, sqrt, root
// these are compatible with both scalars and matrices
// when applied to matrices, the operation is element-wise
// 
// linear operators:
// determinant, orthogonalization, transpose, kernel, trace
// RREF, norm, inverse, pseudo inverse, solve arugmented matrices
// find roots of polynomials
ArmaOperand FunctionExprAst::eval()
{
    typedef ArmaOperand::Type Type;

    int num_args = args.size();

    // Check if the number of arguments is valid
    switch (op)
    {
        case TokName::SOLVE:
            if (num_args != 1 && num_args != 2) 
                throw std::invalid_argument("expect one or two arguments for " + raw + "(), but got " + std::to_string(num_args));

        case TokName::MIN: case TokName::MAX: case TokName::ROOTS:
            break;          // min, max, roots can have arbitrary number of arguments

        case TokName::ROOT: case TokName::LOG: case TokName::GCD: case TokName::LCM:
            if (num_args != 2)  throw std::invalid_argument("expect two arguments for " + raw + "(), but got " + std::to_string(num_args));
            break;

        default:
            if (num_args != 1)  throw std::invalid_argument("expect one argument for " + raw + "(), but got " + std::to_string(num_args));
    }

    vector<ArmaOperand> arg_vals;
    for (auto arg : args)
        arg_vals.emplace_back(arg->eval());

    // Handle general root and solving augmented matrix
    // solve() can either accept one or two arguments
    // if one argument is given, it should be the augmented matrix
    // if two, the first argument should be the coefficient matrix (A)
    // while the second is the constants vector (B), Ax = B
    if (arg_vals.size() == 2)
        switch (op)
        {
            case TokName::ROOT:
                if (arg_vals[0].type == Type::IMAT)
                    throw std::invalid_argument("root funtion for indentity matrix is invalid");
                if (arg_vals[0].type == Type::MAT)
                {
                    if (!arg_vals[1].almostReal())  
                        throw std::invalid_argument("complex value for root is unsupported");
                    return ArmaOperand(arma::powmat(arg_vals[0].mat, 1 / arg_vals[1].value.real()));
                }
                return ArmaOperand(pow(arg_vals[0].value, std::complex<double>(1, 0) / arg_vals[1].value));

            case TokName::SOLVE:
            {
                if (arg_vals[0].type != Type::MAT || arg_vals[1].type != Type::MAT)
                    throw std::invalid_argument("both arguments for function solve() should be matrices");
                if (arg_vals[0].type == Type::IMAT && arg_vals[1].type == Type::IMAT)
                    throw std::invalid_argument("two arguments for function solve() cannot be identity matrix with no size simultaneously");

                arma::cx_mat A = arg_vals[0].mat, B = arg_vals[1].mat;
                if (arg_vals[0].type == Type::IMAT)
                    A = arg_vals[0].fromImat(B.n_rows);
                if (arg_vals[1].type == Type::IMAT)
                    B = arg_vals[1].fromImat(A.n_rows);
                
                // If no solution, solve will attempt to find an approximated one
                return ArmaOperand(arma::solve(A, B));
            }

            default: throw std::invalid_argument("unsupported function: " + raw);
        }


    // Handle linear operations and element-wise operations
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

            case TokName::DET:  case TokName::ABS: return ArmaOperand(arma::det(arg_vals[0].mat));

            case TokName::RANK: return ArmaOperand(arma::rank(arg_vals[0].mat));

            case TokName::ORTH: return ArmaOperand(arma::orth(arg_vals[0].mat));

            case TokName::KER:  return ArmaOperand(arma::null(arg_vals[0].mat));

            case TokName::TR:   return ArmaOperand(arma::trace(arg_vals[0].mat));

            case TokName::NORM: return ArmaOperand(arma::norm(arg_vals[0].mat));

            case TokName::INV:  return ArmaOperand(arma::inv(arg_vals[0].mat));

            case TokName::PINV: return ArmaOperand(arma::pinv(arg_vals[0].mat));

            // If no solution, solve will attempt to find an approximated one
            case TokName::SOLVE:
            {
                size_t num_cols = arg_vals[0].mat.n_cols;
                arma::cx_vec B = arg_vals[0].mat.col(num_cols - 1);
                return ArmaOperand(arma::solve(arg_vals[0].mat.cols(0, num_cols -2), B));
            }

            // case TokName::ABS:  
            //    return ArmaOperand(arma::cx_mat(arma::abs(arg_vals[0].mat), arma::mat(arma::size(arg_vals[0].mat), arma::fill::zeros)));

            case TokName::SQRT: return ArmaOperand(arma::sqrt(arg_vals[0].mat));

            default: throw std::invalid_argument("unsupported function: " + raw);
        }

    // Handle scalar functions
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
                throw std::invalid_argument("square root cannot be applied to identity matrix");
            return ArmaOperand(std::sqrt(arg_vals[0].value));

        case TokName::NORM:
            if (arg_vals[0].type == Type::IMAT)
                throw std::invalid_argument("norm function cannot be applied to identity matrix");
            return ArmaOperand(std::norm(arg_vals[0].value));

        default: throw std::invalid_argument("unsupported function " + raw);
    }
}


string FunctionExprAst::genAsciiMath() const
{
    string result;
    switch (op) 
    {
        case TokName::ORTH: result = "text(orth)("; break;

        case TokName::ABS:  
            result = "|" + args[0]->genAsciiMath() + "|";
            return result;

        case TokName::ROOT:
            result = "root(" + args[1]->genAsciiMath() + ")(" + args[0]->genAsciiMath() + ")";
            return result;

        case TokName::NEG:  result = "-("; break;

        case TokName::INV:  result = "text(inv)("; break;

        case TokName::PINV: result = "text(pinv)("; break;

        case TokName::EIGEN:    result = "text(eigen)("; break;

        case TokName::ROOTS:    result = "text(roots)("; break;

        default: result = raw + "("; break;
    }   

    for (auto it = args.begin(); it != args.end() - 1; ++it)
        result += (*it)->genAsciiMath() + ", ";
        
    return result + (*(args.end() - 1))->genAsciiMath() + ")";
}
