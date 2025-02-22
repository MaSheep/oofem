/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "fei3dwedgequad.h"
#include "floatarray.h"
#include "floatmatrix.h"
#include "floatarrayf.h"
#include "floatmatrixf.h"
#include "intarray.h"
#include "gaussintegrationrule.h"

namespace oofem {

FloatArrayF<15>
FEI3dWedgeQuad :: evalN(const FloatArrayF<3> &lcoords)
{
    double x = lcoords[0];
    double y = lcoords[1];
    double z = lcoords[2];
    return {
        0.5 * ( ( 1. - x - y ) * ( 2. * ( 1. - x - y ) - 1. ) * ( 1. - z ) - ( 1. - x - y ) * ( 1. - z * z ) ),
        0.5 * ( x * ( 2. * x - 1. ) * ( 1. - z ) - x * ( 1. - z * z ) ),
        0.5 * ( y * ( 2. * y - 1. ) * ( 1. - z ) - y * ( 1. - z * z ) ),
        0.5 * ( ( 1. - x - y ) * ( 2. * ( 1. - x - y ) - 1. ) * ( 1. + z ) - ( 1. - x - y ) * ( 1. - z * z ) ),
        0.5 *  ( x * ( 2. * x - 1. ) * ( 1. + z ) - x * ( 1. - z * z ) ),
        0.5 *  ( y * ( 2. * y - 1. ) * ( 1. + z ) - y * ( 1. - z * z ) ),
        2. * ( 1. - x - y ) * x * ( 1. - z ),
        2. * x * y * ( 1. - z ),
        2. * y * ( 1. - x - y ) * ( 1. - z ),
        2. *  x * ( 1. - x - y ) * ( 1. + z ),
        2. * x * y * ( 1. + z ),
        2. * y * ( 1. - x - y ) * ( 1. + z ),
        ( 1. - x - y ) * ( 1. - z * z ),
        x * ( 1. - z * z ),
        y * ( 1. - z * z ),
    };
}

void
FEI3dWedgeQuad :: evalN(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    double x, y, z;
    answer.resize(15);

    x = lcoords.at(1);
    y = lcoords.at(2);
    z = lcoords.at(3);

    answer.at(1) = 0.5 * ( ( 1. - x - y ) * ( 2. * ( 1. - x - y ) - 1. ) * ( 1. - z ) - ( 1. - x - y ) * ( 1. - z * z ) );
    answer.at(2) = 0.5 * ( x * ( 2. * x - 1. ) * ( 1. - z ) - x * ( 1. - z * z ) );
    answer.at(3) = 0.5 * ( y * ( 2. * y - 1. ) * ( 1. - z ) - y * ( 1. - z * z ) );
    answer.at(4) = 0.5 * ( ( 1. - x - y ) * ( 2. * ( 1. - x - y ) - 1. ) * ( 1. + z ) - ( 1. - x - y ) * ( 1. - z * z ) );
    answer.at(5) = 0.5 *  ( x * ( 2. * x - 1. ) * ( 1. + z ) - x * ( 1. - z * z ) );
    answer.at(6) = 0.5 *  ( y * ( 2. * y - 1. ) * ( 1. + z ) - y * ( 1. - z * z ) );
    answer.at(7) = 2. * ( 1. - x - y ) * x * ( 1. - z );
    answer.at(8) = 2. * x * y * ( 1. - z );
    answer.at(9) = 2. * y * ( 1. - x - y ) * ( 1. - z );
    answer.at(10) = 2. *  x * ( 1. - x - y ) * ( 1. + z );
    answer.at(11) = 2. * x * y * ( 1. + z );
    answer.at(12) = 2. * y * ( 1. - x - y ) * ( 1. + z );
    answer.at(13) = ( 1. - x - y ) * ( 1. - z * z );
    answer.at(14) = x * ( 1. - z * z );
    answer.at(15) = y * ( 1. - z * z );
}


FloatMatrixF<3,15>
FEI3dWedgeQuad :: evaldNdxi(const FloatArrayF<3> &lcoords)
{
    double x = lcoords[0];
    double y = lcoords[1];
    double z = lcoords[2];

    return {
        1. / 2. - ( z - 1. ) * ( x + y - 1. ) - z * z / 2. - ( ( z - 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2., // 1
        1. / 2. - ( z - 1. ) * ( x + y - 1. ) - z * z / 2. - ( ( z - 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2.,
        -z * ( x + y - 1. ) - ( ( 2. * x + 2. * y - 1. ) * ( x + y - 1. ) ) / 2.,
        z * z / 2. - x * ( z - 1. ) - ( ( 2. * x - 1. ) * ( z - 1. ) ) / 2. - 1 / 2., // 2
        0.,
        x * z - ( x * ( 2. * x - 1. ) ) / 2.,
        0., // 3
        z * z / 2. - y * ( z - 1. ) - ( ( 2. * y - 1. ) * ( z - 1. ) ) / 2. - 1. / 2.,
        y * z - ( y * ( 2 * y - 1. ) ) / 2.,
        ( ( z + 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2. + ( z + 1. ) * ( x + y - 1. ) - z * z / 2. + 1. / 2., // 4
        ( ( z + 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2. + ( z + 1. ) * ( x + y - 1. ) - z * z / 2. + 1. / 2.,
        ( ( 2. * x + 2. * y - 1. ) * ( x + y - 1. ) ) / 2. - z * ( x + y - 1. ),
        ( ( 2. * x - 1. ) * ( z + 1. ) ) / 2. + x * ( z + 1. ) + z * z / 2. - 1. / 2., 0., ( x * ( 2. * x - 1. ) ) / 2. + x * z, // 5
        0., ( ( 2. * y - 1. ) * ( z + 1. ) ) / 2. + y * ( z + 1. ) + z * z / 2. - 1. / 2., ( y * ( 2. * y - 1. ) ) / 2. + y * z, // 6
        ( z - 1. ) * ( 2. * x + 2. * y - 2. ) + 2. * x * ( z - 1. ), 2. * x * ( z - 1. ), x * ( 2. * x + 2. * y - 2. ), // 7
        -2. * y * ( z - 1. ), -2. * x * ( z - 1. ), -2. * x * y, // 8
        2. * y * ( z - 1. ), 2. * ( z - 1. ) * ( x + y - 1. ) + 2. * y * ( z - 1. ), 2. * y * ( x + y - 1. ), // 9
        -2. * ( z + 1. ) * ( x + y - 1. ) - 2. * x * ( z + 1. ), -2. * x * ( z + 1. ), -2. * x * ( x + y - 1. ), // 10
        2. * y * ( z + 1. ), 2. * x * ( z + 1. ), 2. * x * y, // 11
        -2. * y * ( z + 1. ), -2. * ( z + 1. ) * ( x + y - 1. ) - 2. * y * ( z + 1. ), -2. * y * ( x + y - 1. ), // 12
        z * z - 1., z * z - 1., 2. * z * ( x + y - 1. ), // 13
        1. - z * z, 0., -2. * x * z, // 14
        0., 1. - z * z, -2. * y * z, // 15
    };
}


void
FEI3dWedgeQuad :: evaldNdxi(FloatMatrix &dN, const FloatArray &lcoords, const FEICellGeometry &)
{
    double x, y, z;
    x = lcoords.at(1);
    y = lcoords.at(2);
    z = lcoords.at(3);

    dN.resize(15, 3);

    dN.at(1, 1)  =  1. / 2. - ( z - 1. ) * ( x + y - 1. ) - z * z / 2. - ( ( z - 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2.;
    dN.at(2, 1)  =  z * z / 2. - x * ( z - 1. ) - ( ( 2. * x - 1. ) * ( z - 1. ) ) / 2. - 1 / 2.;
    dN.at(3, 1)  =  0.;
    dN.at(4, 1)  = ( ( z + 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2. + ( z + 1. ) * ( x + y - 1. ) - z * z / 2. + 1. / 2.;
    dN.at(5, 1)  = ( ( 2. * x - 1. ) * ( z + 1. ) ) / 2. + x * ( z + 1. ) + z * z / 2. - 1. / 2.;
    dN.at(6, 1)  =  0.;
    dN.at(7, 1)  = ( z - 1. ) * ( 2. * x + 2. * y - 2. ) + 2. * x * ( z - 1. );
    dN.at(8, 1)  = -2. * y * ( z - 1. );
    dN.at(9, 1)  =  2. * y * ( z - 1. );
    dN.at(10, 1) = -2. * ( z + 1. ) * ( x + y - 1. ) - 2. * x * ( z + 1. );
    dN.at(11, 1) =  2. * y * ( z + 1. );
    dN.at(12, 1) = -2. * y * ( z + 1. );
    dN.at(13, 1) =  z * z - 1.;
    dN.at(14, 1) =  1. - z * z;
    dN.at(15, 1) =  0.;

    dN.at(1, 2) =   1. / 2. - ( z - 1. ) * ( x + y - 1. ) - z * z / 2. - ( ( z - 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2.;
    dN.at(2, 2) =   0.;
    dN.at(3, 2) =   z * z / 2. - y * ( z - 1. ) - ( ( 2. * y - 1. ) * ( z - 1. ) ) / 2. - 1. / 2.;
    dN.at(4, 2) = ( ( z + 1. ) * ( 2. * x + 2. * y - 1. ) ) / 2. + ( z + 1. ) * ( x + y - 1. ) - z * z / 2. + 1. / 2.;
    dN.at(5, 2) =   0.;
    dN.at(6, 2) = ( ( 2. * y - 1. ) * ( z + 1. ) ) / 2. + y * ( z + 1. ) + z * z / 2. - 1. / 2.;
    dN.at(7, 2) =   2. * x * ( z - 1. );
    dN.at(8, 2) =  -2. * x * ( z - 1. );
    dN.at(9, 2) =   2. * ( z - 1. ) * ( x + y - 1. ) + 2. * y * ( z - 1. );
    dN.at(10, 2) = -2. * x * ( z + 1. );
    dN.at(11, 2) =  2. * x * ( z + 1. );
    dN.at(12, 2) = -2. * ( z + 1. ) * ( x + y - 1. ) - 2. * y * ( z + 1. );
    dN.at(13, 2) =  z * z - 1.;
    dN.at(14, 2) =  0.;
    dN.at(15, 2) =  1. - z * z;

    dN.at(1, 3) =  -z * ( x + y - 1. ) - ( ( 2. * x + 2. * y - 1. ) * ( x + y - 1. ) ) / 2.;
    dN.at(2, 3) =   x * z - ( x * ( 2. * x - 1. ) ) / 2.;
    dN.at(3, 3) =   y * z - ( y * ( 2 * y - 1. ) ) / 2.;
    dN.at(4, 3) =  ( ( 2. * x + 2. * y - 1. ) * ( x + y - 1. ) ) / 2. - z * ( x + y - 1. );
    dN.at(5, 3) =  ( x * ( 2. * x - 1. ) ) / 2. + x * z;
    dN.at(6, 3) =  ( y * ( 2. * y - 1. ) ) / 2. + y * z;
    dN.at(7, 3) =   x * ( 2. * x + 2. * y - 2. );
    dN.at(8, 3) =  -2. * x * y;
    dN.at(9, 3) =   2. * y * ( x + y - 1. );
    dN.at(10, 3) = -2. * x * ( x + y - 1. );
    dN.at(11, 3) =  2. * x * y;
    dN.at(12, 3) = -2. * y * ( x + y - 1. );
    dN.at(13, 3) =  2. * z * ( x + y - 1. );
    dN.at(14, 3) = -2. * x * z;
    dN.at(15, 3) = -2. * y * z;
}


std::pair<double, FloatMatrixF<3,15>>
FEI3dWedgeQuad :: evaldNdx(const FloatArrayF<3> &lcoords, const FEICellGeometry &cellgeo)
{
    auto dNduvw = evaldNdxi(lcoords);
    FloatMatrixF<3,15> coords;
    for ( int i = 0; i < 15; i++ ) {
        coords.setColumn(cellgeo.giveVertexCoordinates(i+1), i);
    }
    auto jacT = dotT(dNduvw, coords);
    return {det(jacT), dot(inv(jacT), dNduvw)};
}


double
FEI3dWedgeQuad :: evaldNdx(FloatMatrix &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatMatrix jacobianMatrix, inv, dNduvw, coords;
    this->evaldNdxi(dNduvw, lcoords, cellgeo);
    coords.resize(3, 15);
    for ( int i = 1; i <= 15; i++ ) {
        coords.setColumn(cellgeo.giveVertexCoordinates(i), i);
    }
    jacobianMatrix.beProductOf(coords, dNduvw);
    inv.beInverseOf(jacobianMatrix);

    answer.beProductOf(dNduvw, inv);
    return jacobianMatrix.giveDeterminant();
}


void
FEI3dWedgeQuad :: giveLocalNodeCoords(FloatMatrix &answer)
{

    answer.resize(3,15);
    answer.at(1,1)  = 1.0; answer.at(2,1)  = 0.0; answer.at(3,1)  = -1.0;
    answer.at(1,2)  = 0.0; answer.at(2,2)  = 1.0; answer.at(3,2)  = -1.0;
    answer.at(1,3)  = 0.0; answer.at(2,3)  = 0.0; answer.at(3,3)  = -1.0;
    answer.at(1,4)  = 1.0; answer.at(2,4)  = 0.0; answer.at(3,4)  =  1.0;
    answer.at(1,5)  = 0.0; answer.at(2,5)  = 1.0; answer.at(3,5)  =  1.0; 
    answer.at(1,6)  = 0.0; answer.at(2,6)  = 0.0; answer.at(3,6)  =  1.0;
    answer.at(1,7)  = 0.5; answer.at(2,7)  = 0.5; answer.at(3,7)  = -1.0;
    answer.at(1,8)  = 0.0; answer.at(2,8)  = 0.5; answer.at(3,8)  = -1.0;
    answer.at(1,9)  = 0.5; answer.at(2,9)  = 0.0; answer.at(3,9)  = -1.0;
    answer.at(1,10) = 0.5; answer.at(2,10) = 0.5; answer.at(3,10) =  1.0;
    answer.at(1,11) = 0.0; answer.at(2,11) = 0.5; answer.at(3,11) =  1.0;
    answer.at(1,12) = 0.5; answer.at(2,12) = 0.0; answer.at(3,12) =  1.0;
    answer.at(1,13) = 1.0; answer.at(2,13) = 0.0; answer.at(3,13) =  0.0;
    answer.at(1,14) = 0.0; answer.at(2,14) = 1.0; answer.at(3,14) =  0.0;
    answer.at(1,15) = 0.0; answer.at(2,15) = 0.0; answer.at(3,15) =  0.0;

}


void
FEI3dWedgeQuad :: local2global(FloatArray &answer, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatArray n;
    this->evalN(n, lcoords, cellgeo);

    answer.resize(3);
    answer.zero();
    for ( int i = 1; i <= 15; i++ ) {
        answer.at(1) += n.at(i) * cellgeo.giveVertexCoordinates(i).at(1);
        answer.at(2) += n.at(i) * cellgeo.giveVertexCoordinates(i).at(2);
        answer.at(3) += n.at(i) * cellgeo.giveVertexCoordinates(i).at(3);
    }
}

#define POINT_TOL 1.e-3

int
FEI3dWedgeQuad :: global2local(FloatArray &answer, const FloatArray &gcoords, const FEICellGeometry &cellgeo)
{
    FloatArray res, delta, guess;
    FloatMatrix jac;
    double convergence_limit, error = 0.0;

    // find a suitable convergence limit
    convergence_limit = 1e-6 * this->giveCharacteristicLength(cellgeo);

    // setup initial guess
    answer.resize( gcoords.giveSize() );
    answer.zero();

    // apply Newton-Raphson to solve the problem
    for ( int nite = 0; nite < 40; nite++ ) {
        // compute the residual
        this->local2global(guess, answer, cellgeo);
        res.beDifferenceOf(gcoords, guess);

        // check for convergence
        error = res.computeNorm();
        if ( error < convergence_limit ) {
            break;
        }

        // compute the corrections
        this->giveJacobianMatrixAt(jac, answer, cellgeo);
        jac.solveForRhs(res, delta);

        // update guess
        answer.add(delta);
    }
    if ( error > convergence_limit ) { // Imperfect, could give false negatives.
        //OOFEM_ERROR("no convergence after 10 iterations");
        answer.zero();
        return false;
    }

    // check limits for each local coordinate [-1,1] for quadrilaterals. (different for other elements, typically [0,1]).
    bool inside = true;
    for ( int i = 1; i <= answer.giveSize(); i++ ) {
        if ( answer.at(i) < ( -1. - POINT_TOL ) ) {
            answer.at(i) = -1.;
            inside = false;
        } else if ( answer.at(i) > ( 1. + POINT_TOL ) ) {
            answer.at(i) = 1.;
            inside = false;
        }
    }

    return inside;
}

double FEI3dWedgeQuad :: giveCharacteristicLength(const FEICellGeometry &cellgeo) const
{
    const auto &n1 = cellgeo.giveVertexCoordinates(1);
    const auto &n2 = cellgeo.giveVertexCoordinates(4);
    ///@todo Change this so that it is not dependent on node order.
    return distance(n1, n2);
}



double
FEI3dWedgeQuad :: giveTransformationJacobian(const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatMatrix jacobianMatrix;

    this->giveJacobianMatrixAt(jacobianMatrix, lcoords, cellgeo);
    return jacobianMatrix.giveDeterminant();
}


void
FEI3dWedgeQuad :: giveJacobianMatrixAt(FloatMatrix &jacobianMatrix, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
// Returns the jacobian matrix  J (x,y,z)/(ksi,eta,dzeta)  of the receiver.
{
    FloatMatrix dNduvw, coords;
    this->evaldNdxi(dNduvw, lcoords, cellgeo);
    coords.resize(3, 15);
    for ( int i = 1; i <= 15; i++ ) {
        coords.setColumn(cellgeo.giveVertexCoordinates(i), i);
    }
    jacobianMatrix.beProductOf(coords, dNduvw);
}


void FEI3dWedgeQuad :: edgeEvalN(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    double ksi = lcoords.at(1);
    answer.resize(3);
    answer.at(1) = ksi * ( ksi - 1. ) * 0.5;
    answer.at(2) = ksi * ( 1. + ksi ) * 0.5;
    answer.at(3) = ( 1. - ksi * ksi );
}


void FEI3dWedgeQuad :: edgeEvaldNdx(FloatMatrix &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    OOFEM_ERROR("not implemented");
}


void FEI3dWedgeQuad :: edgeLocal2global(FloatArray &answer, int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatArray n;

    const auto &nodes = this->computeLocalEdgeMapping(iedge);
    this->edgeEvalN(n, iedge, lcoords, cellgeo);

    answer.clear();
    for ( int i = 1; i <= n.giveSize(); ++i ) {
        answer.add( n.at(i), cellgeo.giveVertexCoordinates( nodes.at(i) ) );
    }
}


IntArray
FEI3dWedgeQuad :: computeLocalEdgeMapping(int iedge) const
{
    if ( iedge == 1 ) {
        return {1, 2, 7};
    } else if ( iedge == 2 ) {
        return {2, 3, 8};
    } else if ( iedge == 3 ) {
        return {3, 1, 9};
    } else if ( iedge == 4 ) {
        return {4, 5, 10};
    } else if ( iedge == 5 ) {
        return {5, 6, 11};
    } else if ( iedge == 6 ) {
        return {6, 4, 12};
    } else if ( iedge == 7 ) {
        return {1, 4, 13};
    } else if ( iedge == 8 ) {
        return {2, 5, 14};
    } else if ( iedge == 9 ) {
        return {3, 6, 15};
    } else {
        throw std::range_error("invalid edge number");
        return {};
    }
}


double FEI3dWedgeQuad :: edgeGiveTransformationJacobian(int iedge, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    OOFEM_ERROR("not implemented");
    return 0.0;
}


void
FEI3dWedgeQuad :: surfaceEvalN(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    if ( isurf <= 2 ) {
        double l1, l2, l3;
        l1 = lcoords.at(1);
        l2 = lcoords.at(2);
        l3 = 1.0 - l1 - l2;

        answer.resize(6);
        answer.at(1) = ( 2. * l1 - 1. ) * l1;
        answer.at(2) = ( 2. * l2 - 1. ) * l2;
        answer.at(3) = ( 2. * l3 - 1. ) * l3;
        answer.at(4) = 4. * l1 * l2;
        answer.at(5) = 4. * l2 * l3;
        answer.at(6) = 4. * l3 * l1;
    } else {
        double ksi = lcoords.at(1);
        double eta = lcoords.at(2);

        answer.resize(8);
        answer.at(1) = ( 1. + ksi ) * ( 1. + eta ) * 0.25 * ( ksi + eta - 1. );
        answer.at(2) = ( 1. - ksi ) * ( 1. + eta ) * 0.25 * ( -ksi + eta - 1. );
        answer.at(3) = ( 1. - ksi ) * ( 1. - eta ) * 0.25 * ( -ksi - eta - 1. );
        answer.at(4) = ( 1. + ksi ) * ( 1. - eta ) * 0.25 * ( ksi - eta - 1. );
        answer.at(5) = 0.5 * ( 1. - ksi * ksi ) * ( 1. + eta );
        answer.at(6) = 0.5 * ( 1. - ksi ) * ( 1. - eta * eta );
        answer.at(7) = 0.5 * ( 1. - ksi * ksi ) * ( 1. - eta );
        answer.at(8) = 0.5 * ( 1. + ksi ) * ( 1. - eta * eta );
    }
}


void
FEI3dWedgeQuad :: surfaceLocal2global(FloatArray &answer, int isurf,
                                      const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatArray n;

    const auto &nodes = this->computeLocalSurfaceMapping(isurf);
    this->surfaceEvalN(n, isurf, lcoords, cellgeo);

    answer.clear();
    for ( int i = 1; i <= n.giveSize(); ++i ) {
        answer.add( n.at(i), cellgeo.giveVertexCoordinates( nodes.at(i) ) );
    }
}


IntArray
FEI3dWedgeQuad :: computeLocalSurfaceMapping(int isurf) const
{
    if ( isurf == 1 ) {
        return {1, 2, 3, 7, 8, 9};
    } else if ( isurf == 2 ) {
        return {4, 5, 6, 10, 11, 12};
    } else if ( isurf == 3 ) {
        return {1, 2, 5, 4, 7, 14, 10, 13};
    } else if ( isurf == 4 ) {
        return {2, 3, 6, 5, 8, 15, 11, 14};
    } else if ( isurf == 5 ) {
        return {3, 1, 4, 6, 9, 13, 12, 15};
    } else {
        OOFEM_ERROR("Surface %d doesn't exist.\n", isurf);
        return {};
    }
}


double
FEI3dWedgeQuad :: surfaceEvalNormal(FloatArray &answer, int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatArray a, b, dNdksi, dNdeta;

    const auto &snodes = this->computeLocalSurfaceMapping(isurf);

    if ( snodes.giveSize() == 6 ) {
        double l1, l2, l3;
        l1 = lcoords.at(1);
        l2 = lcoords.at(2);
        l3 = 1.0 - l1 - l2;

        dNdksi.resize(6);
        dNdksi(0) = 4.0 * l1 - 1.0;
        dNdksi(1) = 0.0;
        dNdksi(2) = -1.0 * ( 4.0 * l3 - 1.0 );
        dNdksi(3) = 4.0 * l2;
        dNdksi(4) = -4.0 * l2;
        dNdksi(5) = 4.0 * l3 - 4.0 * l1;

        dNdeta.resize(6);
        dNdeta(0) = 0.0;
        dNdeta(1) = 4.0 * l2 - 1.0;
        dNdeta(2) = -1.0 * ( 4.0 * l3 - 1.0 );
        dNdeta(3) = 4.0 * l1;
        dNdeta(4) = 4.0 * l3 - 4.0 * l2;
        dNdeta(5) = -4.0 * l1;
    } else {
        double ksi, eta;
        ksi = lcoords.at(1);
        eta = lcoords.at(2);

        dNdksi.resize(8);
        dNdksi.at(1) =  0.25 * ( 1. + eta ) * ( 2.0 * ksi + eta );
        dNdksi.at(2) = -0.25 * ( 1. + eta ) * ( -2.0 * ksi + eta );
        dNdksi.at(3) = -0.25 * ( 1. - eta ) * ( -2.0 * ksi - eta );
        dNdksi.at(4) =  0.25 * ( 1. - eta ) * ( 2.0 * ksi - eta );
        dNdksi.at(5) = -ksi * ( 1. + eta );
        dNdksi.at(6) = -0.5 * ( 1. - eta * eta );
        dNdksi.at(7) = -ksi * ( 1. - eta );
        dNdksi.at(8) =  0.5 * ( 1. - eta * eta );

        dNdeta.resize(8);
        dNdeta.at(1) =  0.25 * ( 1. + ksi ) * ( 2.0 * eta + ksi );
        dNdeta.at(2) =  0.25 * ( 1. - ksi ) * ( 2.0 * eta - ksi );
        dNdeta.at(3) = -0.25 * ( 1. - ksi ) * ( -2.0 * eta - ksi );
        dNdeta.at(4) = -0.25 * ( 1. + ksi ) * ( -2.0 * eta + ksi );
        dNdeta.at(5) =  0.5 * ( 1. - ksi * ksi );
        dNdeta.at(6) = -eta * ( 1. - ksi );
        dNdeta.at(7) = -0.5 * ( 1. - ksi * ksi );
        dNdeta.at(8) = -eta * ( 1. + ksi );
    }

    for ( int i = 1; i <= snodes.giveSize(); ++i ) {
        a.add( dNdksi.at(i), cellgeo.giveVertexCoordinates( snodes.at(i) ) );
        b.add( dNdeta.at(i), cellgeo.giveVertexCoordinates( snodes.at(i) ) );
    }

    answer.beVectorProductOf(a, b);
    return answer.normalize();
}


double
FEI3dWedgeQuad :: surfaceGiveTransformationJacobian(int isurf, const FloatArray &lcoords, const FEICellGeometry &cellgeo)
{
    FloatArray normal;
    return this->surfaceEvalNormal(normal, isurf, lcoords, cellgeo);
}


std::unique_ptr<IntegrationRule>
FEI3dWedgeQuad :: giveIntegrationRule(int order)
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    ///@todo This function below isn't supported. We must decide on how to deal with wedges.
    //int points = iRule->getRequiredNumberOfIntegrationPoints(_Wedge, order);
    OOFEM_WARNING("Warning.. ignoring 'order' argument: FIXME");
    int pointsZeta = 1;
    int pointsTriangle = 1;
    iRule->SetUpPointsOnWedge(pointsTriangle, pointsZeta, _Unknown);
    return std::move(iRule);
}

std::unique_ptr<IntegrationRule>
FEI3dWedgeQuad :: giveBoundaryIntegrationRule(int order, int boundary)
{
    auto iRule = std::make_unique<GaussIntegrationRule>(1, nullptr);
    if ( boundary <= 2 ) {
        int points = iRule->getRequiredNumberOfIntegrationPoints(_Triangle, order + 2);
        iRule->SetUpPointsOnTriangle(points, _Unknown);
    } else {
        ///@todo Is +2 correct for dealing with "detJ" on this surface?
        int points = iRule->getRequiredNumberOfIntegrationPoints(_Square, order + 2);
        iRule->SetUpPointsOnSquare(points, _Unknown);
    }
    return std::move(iRule);
}
} // end namespace oofem
