//
//  FEFluidFSIDomain.hpp
//  FEBioFluid
//
//  Created by Gerard Ateshian on 8/13/17.
//  Copyright © 2017 febio.org. All rights reserved.
//

#ifndef FEFluidFSIDomain_hpp
#define FEFluidFSIDomain_hpp

#include <vector>
using namespace std;

class FEModel;
class FESolver;
class FEBodyForce;
class FEGlobalVector;
struct FETimeInfo;

//-----------------------------------------------------------------------------
//! Abstract interface class for fluid-FSI domains.

//! A fluid-FSI domain is used by the fluid-FSI solver.
//! This interface defines the functions that have to be implemented by a
//! fluid-FSI domain. There are basically two categories: residual functions
//! that contribute to the global residual vector. And stiffness matrix
//! function that calculate contributions to the global stiffness matrix.
class FEFluidFSIDomain
{
public:
    FEFluidFSIDomain(FEModel* pfem);
    virtual ~FEFluidFSIDomain(){}
    
    // --- R E S I D U A L ---
    
    //! calculate the internal forces
    virtual void InternalForces(FEGlobalVector& R, const FETimeInfo& tp) = 0;
    
    //! Calculate the body force vector
    virtual void BodyForce(FEGlobalVector& R, const FETimeInfo& tp, FEBodyForce& bf) = 0;
    
    //! calculate the interial forces (for dynamic problems)
    virtual void InertialForces(FEGlobalVector& R, const FETimeInfo& tp) = 0;
    
    // --- S T I F F N E S S   M A T R I X ---
    
    //! Calculate global stiffness matrix (only contribution from internal force derivative)
    //! \todo maybe I should rename this the InternalStiffness matrix?
    virtual void StiffnessMatrix   (FESolver* psolver, const FETimeInfo& tp) = 0;
    
    //! Calculate stiffness contribution of body forces
    virtual void BodyForceStiffness(FESolver* psolver, const FETimeInfo& tp, FEBodyForce& bf) = 0;
    
    //! calculate the mass matrix (for dynamic problems)
    virtual void MassMatrix(FESolver* psolver, const FETimeInfo& tp) = 0;
    
    //! transient analysis
    void SetTransientAnalysis() { m_btrans = true; }
    void SetSteadyStateAnalysis() { m_btrans = false; }
    
public:
    FEModel* GetFEModel() { return m_pfem; }
    
protected:
    FEModel*	m_pfem;
    bool        m_btrans;   // flag for transient (true) or steady-state (false) analysis
};

#endif /* FEFluidFSIDomain_hpp */