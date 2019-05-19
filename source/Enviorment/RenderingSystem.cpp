#include "Enviorment/RenderingSystem.h"

RenderingSystem::RenderingSystem(std::shared_ptr<chrono::irrlicht::ChIrrApp> application, std::shared_ptr<chrono::ChSystem> system) :
	m_application(application), m_system(system)
{
}

void RenderingSystem::Start()
{
	InitComponents();

	while (this->m_application->GetDevice()->run())
	{
		this->m_application->BeginScene();
		this->m_application->DrawAll();
		this->m_application->DoStep();
		this->m_application->EndScene();
	};
}

void RenderingSystem::InitComponents()
{
	this->m_application->AssetBindAll();
	this->m_application->AssetUpdateAll();
	this->m_system->SetupInitial();

	this->m_system->SetSolverType(chrono::ChSolver::Type::MINRES);
	this->m_system->SetSolverWarmStarting(true);
	this->m_system->SetMaxItersSolverSpeed(460);
	this->m_system->SetMaxItersSolverStab(460);
	this->m_system->SetTolForce(0.1e-13);

	auto msolver = std::static_pointer_cast<chrono::ChSolverMINRES>(this->m_system->GetSolver());
	msolver->SetVerbose(false);
	msolver->SetDiagonalPreconditioning(true);
}
