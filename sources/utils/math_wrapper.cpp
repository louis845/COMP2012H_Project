#include "math_wrapper.h"
#include <sstream>
using std::string;
using std::stringstream;

ROperand ROperand::operator-()
{
    if (type == Type::MAT)
    {
        ROperand result(*this);
        for (auto row : result.mat)
            for (auto entry : row)
                entry = -entry;
        return result;
    }
    return ROperand(-value);
}


ROperand ROperand::operator+(ROperand rhs)             // use PBV here since complexify_if_needed() may modify the value
{
    if (type == Type::MAT && rhs.type == Type::MAT)     // matrix-matrix addition
    {
        if (mat.size() != rhs.mat.size() || mat[0].size() != rhs.mat[0].size())
            throw std::runtime_error("Error: attempting to add two matrices with different sizes.");
        ROperand result(rhs);            // use synthesized move constructor
        for (size_t i = 0; i < mat.size(); ++i)
            for (size_t j = 0; j < mat[i].size(); ++j)
            {
                if (R::complexify_if_needed(result.mat[i][j], mat[i][j]))
                    result.mat[i][j] = result.mat[i][j] + mat[i][j];         // no += overloaded for R class
                else throw std::runtime_error("Error: unsupported matrix addition due to type mismatch.");
            }
        return result;
    }

    if (rhs.type == Type::MAT)      // scalar + matrix
    {
        ROperand result(rhs);
        for (size_t i = 0; i < result.mat.size(); ++i)
            for (size_t j = 0; j < result.mat[i].size(); ++j)
            {
                if (R::complexify_if_needed(result.mat[i][j], value))
                    result.mat[i][j] = result.mat[i][j] + value;
                else throw std::runtime_error("Error: unsupported addition due to type mismatch.");
            }
        return result;
    }

    if (type == Type::MAT)          // matrix + scalar
    {
        return rhs * (*this);       // huge time & space complexity here
    }

    ROperand result(rhs);
    if (R::complexify_if_needed(result.value, value))
        result.value = result.value + value;
    else throw std::runtime_error("Error: unsupported addition due to type mismatch.");
    return result;
}


ROperand ROperand::operator*(ROperand rhs)
{
    if (type == Type::MAT && rhs.type == Type::MAT)     // matrix-matrix multiplication
    {
        if (mat[0].size() != rhs.mat.size())
            throw std::runtime_error("Error: attempting to multiply two matrices with different sizes.");
        ROperand result(mat.size());
        for (size_t i = 0; i < mat.size(); ++i)
            for (size_t j = 0; j < rhs.mat[0].size(); ++j)
                result.mat[i].emplace_back(newInt(0L));

        for (size_t i = 0; i < mat.size(); ++i)         // primitive implementation, use Armadillo if handle large matrices
            for (size_t j = 0; j < rhs.mat[0].size(); ++j)
                for (size_t k = 0; k < rhs.mat.size(); ++k)
                {
                    if (R::complexify_if_needed(mat[i][k], rhs.mat[k][j]))
                        result.mat[i][j] = result.mat[i][j] + (mat[i][k] * rhs.mat[k][j]);
                    else throw std::runtime_error("Error: unsupported matrix multiplication due to type mismatch.");
                }
        return result;
    }
    
    if (type == Type::MAT)          // matrix * scalar
    {
        return rhs * (*this);
    }

    if (rhs.type == Type::MAT)      // scalar * matrix
    {
        ROperand result(rhs);
        for (size_t i = 0; i < result.mat.size(); ++i)
            for (size_t j = 0; j < result.mat[i].size(); ++j)
            {
                if (R::complexify_if_needed(result.mat[i][j], value))
                    result.mat[i][j] = result.mat[i][j] * value;
                else throw std::runtime_error("Error: unsupported multiplication due to type mismatch.");
            }
        return result;
    }

    ROperand result(rhs);        // scalar * scalar
    if (R::complexify_if_needed(result.value, value))
        result.value = result.value * value;
    else throw std::runtime_error("Error: unsupported multiplication due to type mismatch.");
    return result;
}


ROperand ROperand::operator-(ROperand rhs)
{
    return ROperand(newInt(-1)) * rhs + (*this);        // sacrifice the performance for simplicity
}


ROperand ROperand::operator/(ROperand rhs)
{
    if (type == Type::NOR && rhs.type == Type::NOR)
    {
        if (rhs.value.is_zero())  throw std::overflow_error("Divide by zero.");
        if (!rhs.value.is_field())  rhs = ROperand(newFrac(rhs.value, rhs.value.promote_one()));
        if (R::complexify_if_needed(value, rhs.value))  return ROperand(value / rhs.value);
        else throw std::runtime_error("Error: unsupported division due to type mismatch.");
    }
    throw std::runtime_error("Error: matrix does not support division.");
}


ROperand ROperand::operator^(int rhs)           // fast exponentiation
{
    if (rhs < 0)    throw std::invalid_argument("Negative exponential is not supported");

    ROperand result(newInt(1L)), cur_base(*this);
    while (rhs)
    {
        if (rhs & 1)    result = result * cur_base;
        cur_base = cur_base * cur_base;
        rhs >>= 1;
    }
    return result;
}


ROperand ROperand::operator+=(ROperand rhs)
{
    *this = *this + rhs;
    return *this;
}


ROperand ROperand::operator-=(ROperand rhs)
{
    *this = *this - rhs;
    return *this;
}


string ROperand::genTex() const
{
    switch (type)
    {
        case Type::NOR: 
            return value.to_latex();

        case Type::IMAT:
        {
            string coeff = value.to_latex();
            for (unsigned char c : coeff)
                if ((c < '0' || c > '9') && c != '.') return "(" + coeff + ")I";
            return coeff + "I";
        }

        case Type::MAT:
        {
            string tex{R"(\begin{bmatrix} )"};
            for (auto row_it = mat.begin(); row_it != mat.end(); ++row_it)
            {
                for (auto col_it = row_it->begin(); col_it != row_it->end() - 1; ++col_it)
                    tex += col_it->to_latex() + " & ";
                if (row_it != mat.end() - 1)
                    tex += (row_it->end() - 1)->to_latex() + R"(\\ )";
                else tex += (row_it->end() - 1)->to_latex() + " ";
            }
            tex += R"(\end{bmatrix})";
            return tex;
        }
    }
}


ArmaOperand ArmaOperand::operator+(const ArmaOperand& rhs) const
{
    if (type == rhs.type)
    {
        if (type == Type::MAT)  return ArmaOperand(mat + rhs.mat);      // Armadillo will throw exceptions if dimensions mismatch
        if (type == Type::NOR)  return ArmaOperand(value + rhs.value);
        return ArmaOperand(Type::IMAT, value + rhs.value);           // two identity matrices
    }

    if (type == Type::IMAT && rhs.type == Type::NOR)
        return ArmaOperand(Type::IMAT, value + rhs.value);

    if (type == Type::NOR && rhs.type == Type::IMAT)
        return ArmaOperand(Type::IMAT, value + rhs.value);

    if (type == Type::IMAT && rhs.type == Type::MAT)
    {   
        if (!rhs.mat.is_square())   
            throw std::runtime_error("Error: attempting to add a square matrix to a non-square matrix.");
        return ArmaOperand(rhs.mat + arma::cx_mat(rhs.mat.n_rows, rhs.mat.n_cols, arma::fill::eye) * value);
    }

    if (type == Type::MAT && rhs.type == Type::IMAT)
        return rhs + (*this);

    if (type == Type::NOR && rhs.type == Type::MAT)
        return ArmaOperand(rhs.mat + value);

    return ArmaOperand(mat + rhs.value);
}


ArmaOperand ArmaOperand::operator-(const ArmaOperand& rhs) const
{
    if (type == rhs.type)
    {
        if (type == Type::MAT)  return ArmaOperand(mat - rhs.mat);     
        if (type == Type::NOR)  return ArmaOperand(value - rhs.value);
        return ArmaOperand(Type::IMAT, value - rhs.value);           
    }

    if (type == Type::IMAT && rhs.type == Type::NOR)
        return ArmaOperand(Type::IMAT, value - rhs.value);

    if (type == Type::NOR && rhs.type == Type::IMAT)
        return ArmaOperand(Type::IMAT, value - rhs.value);

    if (type == Type::IMAT && rhs.type == Type::MAT)
    {   
        if (!rhs.mat.is_square())   
            throw std::runtime_error("Error: attempting to subtract a non-square matrix from a square matrix.");
        return ArmaOperand(rhs.mat - arma::cx_mat(rhs.mat.n_rows, rhs.mat.n_cols, arma::fill::eye) * value);
    }

    if (type == Type::MAT && rhs.type == Type::IMAT)
        return rhs - (*this);

    if (type == Type::NOR && rhs.type == Type::MAT)
        return ArmaOperand(rhs.mat - value);

    return ArmaOperand(mat - rhs.value);
}


ArmaOperand ArmaOperand::operator*(const ArmaOperand& rhs) const
{
    if (type == rhs.type)
    {
        if (type == Type::MAT)  return ArmaOperand(mat * rhs.mat);     
        if (type == Type::NOR)  return ArmaOperand(value * rhs.value);
        return ArmaOperand(Type::IMAT, value * rhs.value);           
    }

    if (type == Type::IMAT && rhs.type == Type::NOR)
        return ArmaOperand(Type::IMAT, value * rhs.value);

    if (type == Type::NOR && rhs.type == Type::IMAT)
        return ArmaOperand(Type::IMAT, value * rhs.value);

    if (type == Type::IMAT && rhs.type == Type::MAT)
        return ArmaOperand(rhs.mat * arma::cx_mat(rhs.mat.n_rows, rhs.mat.n_cols, arma::fill::eye) * value);

    if (type == Type::MAT && rhs.type == Type::IMAT)
        return rhs * (*this);

    if (type == Type::NOR && rhs.type == Type::MAT)
        return ArmaOperand(rhs.mat * value);

    return ArmaOperand(mat * rhs.value);
}


ArmaOperand ArmaOperand::operator/(const ArmaOperand& rhs) const
{
    if (rhs.type != Type::NOR)
        throw std::logic_error("Error: divided by a matrix.");
    
    if (type == Type::NOR)  return ArmaOperand(value / rhs.value);
    if (type == Type::MAT)  return ArmaOperand(mat / rhs.value);
    if (type == Type::IMAT) return ArmaOperand(Type::IMAT, value / rhs.value); 
}


ArmaOperand ArmaOperand::operator^(const ArmaOperand& rhs) const
{
    if (rhs.type != ArmaOperand::Type::NOR)
        throw std::logic_error("Error: matrix is not a valid exponential.");
    if (type == ArmaOperand::Type::NOR)
        return ArmaOperand(std::pow(value, rhs.value));
    
    if (!rhs.almostReal())    
        throw std::logic_error("matrix does not support complex exponential");
    if (type == ArmaOperand::Type::MAT)
        return ArmaOperand(arma::powmat(mat, rhs.value.real()));
    if (type == ArmaOperand::Type::IMAT)
        return ArmaOperand(ArmaOperand::Type::IMAT, pow(value, rhs.value.real()));
}


arma::cx_mat ArmaOperand::fromImat(size_t row) const 
{
    if (type != Type::IMAT) throw std::runtime_error("operand type is not identity matrix");
    return arma::cx_mat(row, row, arma::fill::eye) * value;
}


string ArmaOperand::genTex() const
{
    stringstream ss;
    ss.flags(cout.flags());
    ss.imbue(cout.getloc());
    // ss.precision(cout.precision());
    ss.precision(4);
    ss.setf(std::ios::fixed, std::ios::floatfield);

    switch (type)
    {
        case Type::NOR:
            if (almostReal())   return std::to_string(value.real());
            ss << value.real();
            if (value.imag() > 0)   ss << "+";
            ss << value.imag() << "i";
            return ss.str();

        case Type::IMAT:
            if (almostReal())   return std::to_string(value.real()) + "I";
            ss << "(" << value.real();
            if (value.imag() > 0)   ss << "+";
            ss << value.imag() << "i)I";
            return ss.str();

        case Type::MAT:
            ss << R"(\begin{bmatrix} )";
            for (size_t i = 0; i < mat.n_rows; ++i) 
            {
                for (size_t j = 0; j < mat.n_cols; ++j)
                {
                    if (fabs(mat(i, j).imag()) <= std::numeric_limits<double>::epsilon())
                        ss << std::to_string(mat(i, j).real());
                    else 
                    {
                        ss << mat(i, j).real();
                        if (mat(i, j).imag() > 0)   ss << "+";
                        ss << mat(i, j).imag() << "i";
                    }
                    
                    if (j != mat.n_cols - 1)    ss << R"( & )";
                    if (j == mat.n_cols - 1 && i != mat.n_rows - 1)    ss << R"(\\ )";
                }
            }
            ss << R"(\end{bmatrix})";
            return ss.str();
    }
}