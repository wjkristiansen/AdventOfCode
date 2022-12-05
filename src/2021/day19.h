#pragma once

#include "CanvasMath.hpp"

namespace Day19
{
    using Vector = Canvas::Math::IntVector3;
    using Matrix = Canvas::Math::TMatrix<int, 3, 3>;

    const Matrix Rotations[24] =
    {
        Matrix(
            Vector( 1, 0, 0 ),
            Vector( 0, 1, 0 ),
            Vector( 0, 0, 1 )),
        Matrix(
            Vector( 1, 0, 0),
            Vector( 0, 0,-1),
            Vector( 0, 1, 0)),
        Matrix(
            Vector( 1, 0, 0),
            Vector( 0,-1, 0),
            Vector( 0, 0,-1)),
        Matrix(
            Vector( 1, 0, 0),
            Vector( 0, 0, 1),
            Vector( 0,-1, 0)),
        Matrix(
            Vector( 0,-1, 0),
            Vector( 1, 0, 0),
            Vector( 0, 0, 1)),
        Matrix(
            Vector( 0, 0, 1),
            Vector( 1, 0, 0),
            Vector( 0, 1, 0)),
        Matrix(
            Vector( 0, 1, 0),
            Vector( 1, 0, 0),
            Vector( 0, 0,-1)),
        Matrix(
            Vector( 0, 0,-1),
            Vector( 1, 0, 0),
            Vector( 0,-1, 0)),
        Matrix(
            Vector( -1, 0, 0),
            Vector(  0,-1, 0),
            Vector(  0, 0, 1)),
        Matrix(
            Vector( -1, 0, 0),
            Vector(  0, 0,-1),
            Vector(  0,-1, 0)),
        Matrix(
            Vector( -1, 0, 0),
            Vector(  0, 1, 0),
            Vector(  0, 0,-1)),
        Matrix(
            Vector( -1, 0, 0),
            Vector(  0, 0, 1),
            Vector(  0, 1, 0)),
        Matrix(
            Vector(  0, 1, 0),
            Vector( -1, 0, 0),
            Vector(  0, 0, 1)),
        Matrix(
            Vector(  0, 0, 1),
            Vector( -1, 0, 0),
            Vector(  0,-1, 0)),
        Matrix(
            Vector( 0,-1, 0),
            Vector(-1, 0, 0),
            Vector( 0, 0,-1)),
        Matrix(
            Vector( 0, 0,-1),
            Vector(-1, 0, 0),
            Vector( 0, 1, 0)),
        Matrix(
            Vector( 0, 0,-1),
            Vector( 0, 1, 0),
            Vector( 1, 0, 0)),
        Matrix(
            Vector( 0, 1, 0),
            Vector( 0, 0, 1),
            Vector( 1, 0, 0)),
        Matrix(
            Vector( 0, 0, 1),
            Vector( 0,-1, 0),
            Vector( 1, 0, 0)),
        Matrix(
            Vector( 0,-1, 0),
            Vector( 0, 0,-1),
            Vector( 1, 0, 0)),
        Matrix(
            Vector( 0, 0,-1),
            Vector( 0,-1, 0),
            Vector(-1, 0, 0)),
        Matrix(
            Vector( 0,-1, 0),
            Vector( 0, 0, 1),
            Vector(-1, 0, 0)),
        Matrix(
            Vector( 0, 0, 1),
            Vector( 0, 1, 0),
            Vector(-1, 0, 0)),
        Matrix(
            Vector( 0, 1, 0),
            Vector( 0, 0,-1),
            Vector(-1, 0, 0)),
    };

    extern void Execute();
}
