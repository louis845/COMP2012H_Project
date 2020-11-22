#include "math_wrapper.h"


ROperand ROperand::operator+(ROperand rhs)             // use PBV here since complexify_if_needed() may modify the value
{
    if (type == Type::MAT && rhs.type == Type::MAT)     // matrix-matrix addition
    {
        if (mat.size() != rhs.mat.size() || mat[0].size() != rhs.mat[0].size())
            throw std::runtime_error("Error: attempting to add two matrices with different sizes.");
        ROperand result(std::move(rhs));            // use synthesized move constructor
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
        ROperand result(std::move(rhs));
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

    ROperand result(std::move(rhs));
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
        for (size_t i = 0; i < mat.size(); ++i)         // primitive implementation, use Armadillo if handle large matrices
            for (size_t j = 0; j < rhs.mat[0].size(); ++j)
                for (size_t k = 0; k < rhs.mat.size(); ++k)
                {
                    if (R::complexify_if_needed(mat[i][k], rhs.mat[k][j]))
                        mat[i].push_back(mat[i][k] * rhs.mat[k][j]);
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
        ROperand result(std::move(rhs));
        for (size_t i = 0; i < result.mat.size(); ++i)
            for (size_t j = 0; j < result.mat[i].size(); ++j)
            {
                if (R::complexify_if_needed(result.mat[i][j], value))
                    result.mat[i][j] = result.mat[i][j] * value;
                else throw std::runtime_error("Error: unsupported multiplication due to type mismatch.");
            }
        return result;
    }

    ROperand result(std::move(rhs));        // scalar * scalar
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


ROperand ROperand::operator^(int rhs)
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