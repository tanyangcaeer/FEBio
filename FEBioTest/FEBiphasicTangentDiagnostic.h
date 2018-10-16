#include "FEDiagnostic.h"

//-----------------------------------------------------------------------------
class FEBiphasicScenario : public FEDiagnosticScenario
{
public:
	FEBiphasicScenario(FEDiagnostic* pdia) : FEDiagnosticScenario(pdia) { m_dt = 1.0; }

public:
	double	m_dt;
};

//-----------------------------------------------------------------------------
class FEBiphasicTangentUniaxial : public FEBiphasicScenario
{
public:
	FEBiphasicTangentUniaxial(FEDiagnostic* pdia);

	bool Init() override;

private:
    double		m_strain;
    double      m_pressure;

	DECLARE_FECORE_CLASS();
};

//-----------------------------------------------------------------------------
//! The FEBiphasicTangentDiagnostic class tests the stiffness matrix implementation
//! by comparing it to a numerical approximating of the derivative of the
//! residual.

class FEBiphasicTangentDiagnostic : public FEDiagnostic
{
public:
    FEBiphasicTangentDiagnostic(FEModel& fem);
    virtual ~FEBiphasicTangentDiagnostic(){}
    
    bool Init();
    
    bool Run();

	FEDiagnosticScenario* CreateScenario(const std::string& sname);
    
protected:
    void deriv_residual(matrix& ke);
    
    void print_matrix(matrix& m);
    
public:
    FEBiphasicScenario*	m_pscn;
};
