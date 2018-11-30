#pragma once
#include "FECoreBase.h"
#include "FEMaterialPoint.h"
#include "FEModelParam.h"
#include "DumpStream.h"
#include "FEDomainList.h"
#include "fecore_error.h"

//-----------------------------------------------------------------------------
// forward declaration of some classes
class FEDomain;

//-----------------------------------------------------------------------------
//! Abstract base class for material types

//! From this class all other material classes are derived.

class FECORE_API FEMaterial : public FECoreBase
{
	DECLARE_SUPER_CLASS(FEMATERIAL_ID);

public:
	FEMaterial(FEModel* fem);
	virtual ~FEMaterial();

	//! returns a pointer to a new material point object
	virtual FEMaterialPoint* CreateMaterialPointData() { return 0; };

	//! performs initialization
	bool Init() override;

    //! Update specialized material points at each iteration
    virtual void UpdateSpecializedMaterialPoints(FEMaterialPoint& mp, const FETimeInfo& tp) {}

public:
	// evaluate local coordinate system at material point
	mat3d GetLocalCS(const FEMaterialPoint& mp);

public:
	//! Assign a domain to this material
	void AddDomain(FEDomain* dom);

	//! get the domaint list
	FEDomainList& GetDomainList() { return m_domList; }

private:
	FEParamMat3d	m_Q;			//!< local material coordinate system
	FEDomainList	m_domList;		//!< list of domains that use this material

	DECLARE_FECORE_CLASS();
};
