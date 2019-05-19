#include "Enviorment/GraphicalObjects/Configurations.h"

std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> GraphicalObjects::Configurations::CreateBeamVisualizationMeshConfig(std::shared_ptr<chrono::ChPhysicsItem> mesh)
{
	auto visualizationMesh = std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(std::dynamic_pointer_cast<chrono::fea::ChMesh>(mesh).get()));
	visualizationMesh->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_ELEM_STRAIN_VONMISES);
	visualizationMesh->SetColorscaleMinMax(-0.5, 0.5);
	visualizationMesh->SetSmoothFaces(true);
	visualizationMesh->SetWireframe(false);
	return visualizationMesh;
}

std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> GraphicalObjects::Configurations::CreateCableVisualizationMeshConfigFirst(std::shared_ptr<chrono::ChPhysicsItem> mesh)
{
	auto visualizationMesh = std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(std::dynamic_pointer_cast<chrono::fea::ChMesh>(mesh).get()));
	visualizationMesh->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_ELEM_BEAM_MZ);
	visualizationMesh->SetColorscaleMinMax(-0.4, 0.4);
	visualizationMesh->SetSmoothFaces(true);
	visualizationMesh->SetWireframe(false);
	return visualizationMesh;
}

std::shared_ptr<chrono::fea::ChVisualizationFEAmesh> GraphicalObjects::Configurations::CreateCableVisualizationMeshConfigSecond(std::shared_ptr<chrono::ChPhysicsItem> mesh)
{
	auto visualizationMesh = std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(std::dynamic_pointer_cast<chrono::fea::ChMesh>(mesh).get()));
	visualizationMesh->SetFEMglyphType(chrono::fea::ChVisualizationFEAmesh::E_GLYPH_NODE_DOT_POS);
	visualizationMesh->SetFEMdataType(chrono::fea::ChVisualizationFEAmesh::E_PLOT_NONE);
	visualizationMesh->SetSymbolsThickness(0.006);
	visualizationMesh->SetSymbolsScale(0.01);
	visualizationMesh->SetZbufferHide(false);
	return visualizationMesh;
}

std::shared_ptr<chrono::fea::ChContinuumPlasticVonMises> GraphicalObjects::Configurations::CreateBeamMaterialConfig()
{
	auto material = std::make_shared<chrono::fea::ChContinuumPlasticVonMises>();
	material->Set_E(0.005e9);
	material->Set_v(0.3);
	return material;
}

std::shared_ptr<chrono::irrlicht::ChIrrApp> GraphicalObjects::Configurations::CreateAppConfig(std::shared_ptr<chrono::ChSystem> system)
{
	auto application = std::make_shared<chrono::irrlicht::ChIrrApp>(system.get(), L"Environment", irr::core::dimension2d<irr::u32>(1200, 720), false);
	application->AddTypicalLogo();
	application->AddTypicalSky();
	application->AddTypicalLights();
	application->AddTypicalCamera(irr::core::vector3df(3, 3, 3));
	return application;
}
