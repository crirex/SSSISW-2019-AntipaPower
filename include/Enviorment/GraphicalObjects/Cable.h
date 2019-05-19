#pragma once

#include "chrono/fea/ChBuilderBeam.h"
#include "chrono/fea/ChMesh.h"
#include "chrono/fea/ChVisualizationFEAmesh.h"
#include "chrono/fea/ChBuilderBeam.h"
#include "chrono/physics/ChBodyEasy.h"
#include <cmath>

#include "Beam.h"

namespace GraphicalObjects
{
	class Cable :
		public GraphicalObject
	{
	public:
		Cable(Beam & beam);
		~Cable() = default;

	public:
		// Inherited via GraphicalObject
		virtual void Build() override;
		virtual std::shared_ptr<chrono::fea::ChMesh> GetMesh() const override;

	public:
		void SetVisualtizationMesh(const std::shared_ptr<chrono::fea::ChVisualizationFEAmesh>& visualtiozationMesh);
		void ConstructCableFromBeam();

	private:
		void InitializeSectionCable();
		void ConstructBase(std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes);
		void ConstructWeight();
		void ConstructCables(const std::vector<std::shared_ptr<chrono::fea::ChNodeFEAxyz>> nodes);
	private:
		std::shared_ptr<chrono::fea::ChMesh> m_mesh;
		std::shared_ptr<chrono::fea::ChBeamSectionCable> m_sectionCable;
		std::shared_ptr<chrono::ChBody> m_base;

		chrono::fea::ChBuilderBeamANCF m_builder;

		Beam & m_beam;
	};
}