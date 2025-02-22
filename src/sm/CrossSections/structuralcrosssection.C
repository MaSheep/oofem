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

#include "sm/CrossSections/structuralcrosssection.h"
#include "sm/Materials/structuralmaterial.h"
#include "sm/Materials/structuralms.h"
#include "gausspoint.h"
#include "element.h"
#include "floatarray.h"

namespace oofem {
FloatArray
StructuralCrossSection :: giveRealStresses(const FloatArray &strain, GaussPoint *gp, TimeStep *tStep) const
{
    MaterialMode mode = gp->giveMaterialMode();
    if ( mode == _2dBeam ) {
        return this->giveGeneralizedStress_Beam2d(strain, gp, tStep);
    } else if ( mode == _3dBeam ) {
        return this->giveGeneralizedStress_Beam3d(strain, gp, tStep);
    } else if ( mode == _2dPlate ) {
        return this->giveGeneralizedStress_Plate(strain, gp, tStep);
    } else if ( mode == _3dShell ) {
        return this->giveGeneralizedStress_Shell(strain, gp, tStep);
    } else if ( mode == _3dShellRot ) {
      return this->giveGeneralizedStress_ShellRot(strain, gp, tStep);
    } else if ( mode == _3dMat ) {
        return this->giveRealStress_3d(strain, gp, tStep);
    } else if ( mode == _PlaneStrain ) {
        return this->giveRealStress_PlaneStrain(strain, gp, tStep);
    } else if ( mode == _PlaneStress ) {
        return this->giveRealStress_PlaneStress(strain, gp, tStep);
    } else if ( mode == _1dMat ) {
        return this->giveRealStress_1d(strain, gp, tStep);
    } else if ( mode == _Warping ) {
        return this->giveRealStress_Warping(strain, gp, tStep);
    } else {
        // This should never happen ?
        ///@todo this part only works for simple cross section and will be removed soon when new interface elements are done /JB
        auto mat = dynamic_cast< StructuralMaterial * >( this->giveMaterial(gp) );
        if ( mat->hasMaterialModeCapability( mode ) ) {
            FloatArray answer;
            mat->giveRealStressVector(answer, gp, strain, tStep);
            return answer;
        } else {
            OOFEM_ERROR("unsupported mode");
        }
    }
}


FloatArray *
StructuralCrossSection :: imposeStressConstrainsOnGradient(GaussPoint *gp,
                                                           FloatArray *gradientStressVector3d)
//
// returns modified gradient of stress vector, which is used to
// bring stresses back to yield surface.
//
// imposes zeros on places, where zero stress occurs. if energetically connected
// strain is zero, we do not impose zero there, because stress exist and
// must be taken into account when computing yeld function. In such case
// a problem is assumed to be full 3d with some explicit strain equal to 0.
//
// On the other hand, if some stress is imposed to be zero, we understand
// such case as subspace of 3d case (like a classical plane stess problem, with no
// tracing of ez, sigma_z)
//
{
    MaterialMode mode = gp->giveMaterialMode();
    if ( gradientStressVector3d->giveSize() != 6 ) {
        OOFEM_ERROR("gradientStressVector3d size mismatch");
    }

    if ( mode == _3dMat ) {
        return gradientStressVector3d;
    }


    switch ( mode ) {
    case _PlaneStress:
        gradientStressVector3d->at(3) = 0.;
        gradientStressVector3d->at(4) = 0.;
        gradientStressVector3d->at(5) = 0.;
        break;
    case _PlaneStrain:
        // gradientStressVector3d ->at(3) = 0.;
        gradientStressVector3d->at(4) = 0.;
        gradientStressVector3d->at(5) = 0.;
        break;
    case _1dMat:
        for ( int i = 2; i <= 6; i++ ) {
            gradientStressVector3d->at(i) = 0.;
        }

        break;
    default:
        OOFEM_ERROR("unknown mode (%s)", __MaterialModeToString(mode) );
        break;
    }

    return gradientStressVector3d;
}

FloatArrayF<6>
StructuralCrossSection :: giveGeneralizedStress_3dBeamSubSoil(const FloatArrayF<6> &generalizedStrain, GaussPoint *gp, TimeStep *tStep) const
{
    auto mat = static_cast< StructuralMaterial * >( this->giveMaterial(gp) );
    return mat->giveRealStressVector_3dBeamSubSoil(generalizedStrain, gp, tStep);
}

FloatMatrixF<6,6>
StructuralCrossSection :: give3dBeamSubSoilStiffMtrx(MatResponseMode rMode, GaussPoint *gp, TimeStep *tStep) const
{
    auto mat = dynamic_cast< StructuralMaterial * >( this->giveMaterial(gp) );
    return mat->give3dBeamSubSoilStiffMtrx(ElasticStiffness, gp, tStep);
}

  

FloatArray *
StructuralCrossSection :: imposeStrainConstrainsOnGradient(GaussPoint *gp,
                                                           FloatArray *gradientStrainVector3d)
//
// returns modified gradient of strain vector, which is used to
// compute plastic strain increment.
//
// imposes zeros on places, where zero strain occurs or energetically connected stress
// is prescribed to be zero.
//
{
    MaterialMode mode = gp->giveMaterialMode();
    if ( gradientStrainVector3d->giveSize() != 6 ) {
        OOFEM_ERROR("gradientStrainVector3d size mismatch");
    }

    if ( mode == _3dMat ) {
        return gradientStrainVector3d;
    }


    switch ( mode ) {
    case _PlaneStress:
        gradientStrainVector3d->at(3) = 0.;
        gradientStrainVector3d->at(4) = 0.;
        gradientStrainVector3d->at(5) = 0.;
        break;
    case _PlaneStrain:
        gradientStrainVector3d->at(3) = 0.;
        gradientStrainVector3d->at(4) = 0.;
        gradientStrainVector3d->at(5) = 0.;
        break;
    case _1dMat:
        for ( int i = 2; i <= 6; i++ ) {
            gradientStrainVector3d->at(i) = 0.;
        }

        break;
    default:
        OOFEM_ERROR("unknown mode (%s)", __MaterialModeToString(mode) );
        break;
    }

    return gradientStrainVector3d;
}

} // end namespace oofem
