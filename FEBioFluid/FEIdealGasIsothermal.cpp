//
//  FEIdealGasIsothermal.cpp
//  FEBioFluid
//
//  Created by Gerard Ateshian on 3/2/19.
//  Copyright © 2019 febio.org. All rights reserved.
//

#include "FEIdealGasIsothermal.h"
#include "FECore/FEModel.h"
#include "FECore/FECoreKernel.h"
#include <FECore/fecore_error.h>

// define the material parameters
BEGIN_FECORE_CLASS(FEIdealGasIsothermal, FEFluid)
ADD_PARAMETER(m_M    , FE_RANGE_GREATER(0.0), "M"    );
END_FECORE_CLASS();

//============================================================================
// FEIdealGasIsothermal
//============================================================================

//-----------------------------------------------------------------------------
//! FEIdealGasIsothermal constructor

FEIdealGasIsothermal::FEIdealGasIsothermal(FEModel* pfem) : FEFluid(pfem)
{
    m_rhor = 0;
    m_k = 0;
    m_M = 0;
}

//-----------------------------------------------------------------------------
//! initialization
bool FEIdealGasIsothermal::Init()
{
    m_R  = GetFEModel()->GetGlobalConstant("R");
    m_Tr = GetFEModel()->GetGlobalConstant("T");
    m_pr = GetFEModel()->GetGlobalConstant("p");
    
    if (m_R <= 0) return fecore_error("A positive universal gas constant R must be defined in Globals section");
    if (m_pr <= 0) return fecore_error("A positive ambient absolute pressure p must be defined in Globals section");
    
    m_rhor = m_M*m_pr/(m_R*m_Tr);
    
    return true;
}

//-----------------------------------------------------------------------------
//! elastic pressure from dilatation
double FEIdealGasIsothermal::Pressure(const double e)
{
    double J = 1 + e;
    return m_pr*(1./J - 1);
}

//-----------------------------------------------------------------------------
//! tangent of elastic pressure with respect to strain J
double FEIdealGasIsothermal::Tangent_Pressure_Strain(FEMaterialPoint& mp)
{
    FEFluidMaterialPoint& fp = *mp.ExtractData<FEFluidMaterialPoint>();
    double J = fp.m_Jf;
    double dp = -m_pr/J;
    return dp;
}

//-----------------------------------------------------------------------------
//! 2nd tangent of elastic pressure with respect to strain J
double FEIdealGasIsothermal::Tangent_Pressure_Strain_Strain(FEMaterialPoint& mp)
{
    FEFluidMaterialPoint& fp = *mp.ExtractData<FEFluidMaterialPoint>();
    double J = fp.m_Jf;
    double d2p = 2*m_pr/(J*J);
    return d2p;
}

//-----------------------------------------------------------------------------
//! evaluate temperature
double FEIdealGasIsothermal::Temperature(FEMaterialPoint& mp)
{
    return m_Tr;
}

//-----------------------------------------------------------------------------
//! calculate free energy density (per reference volume)
double FEIdealGasIsothermal::StrainEnergyDensity(FEMaterialPoint& mp)
{
    FEFluidMaterialPoint& fp = *mp.ExtractData<FEFluidMaterialPoint>();
    double J = fp.m_Jf;
    double sed = m_pr*(J-1-log(J));
    return sed;
}

//-----------------------------------------------------------------------------
//! invert pressure-dilatation relation
double FEIdealGasIsothermal::Dilatation(const double p)
{
    double J = m_pr/(p+m_pr);
    return J - 1;
}
