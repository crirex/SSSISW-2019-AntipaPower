#pragma once

#include "chrono/solver/ChSolverMINRES.h"
#include "chrono/physics/ChSystem.h"
#include "chrono_irrlicht/ChIrrApp.h"

class RenderingSystem
{
public:
	RenderingSystem(std::shared_ptr<chrono::irrlicht::ChIrrApp> application, std::shared_ptr<chrono::ChSystem> system);
	~RenderingSystem() = default;
	
public:
	void Start();

private:
	void InitComponents();

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	std::shared_ptr<chrono::ChSystem> m_system;
};

