#include "Model.h"

Model::Model(const std::string& file_path)
{
	if (!m_loader.LoadFile(file_path))
	{
		throw std::logic_error("Fail to load the model from the obj file");
	}
	for (auto& i : m_loader.LoadedVertices)
	{
		i.Position = NormalizeVec(i.Position);
	}
}
const std::vector<D3D11_INPUT_ELEMENT_DESC>& Model::GetVertexLayout()
{
	return m_VertexLayout;
}
const std::vector<objl::Vertex>& Model::GetVertices() const
{
	return m_loader.LoadedVertices;
}
const std::vector<unsigned int>& Model::GetIndices() const
{
	return m_loader.LoadedIndices;
}

const objl::Vector3& Model::NormalizeVec(const objl::Vector3& vec3)
{
	{
		float length = (float)sqrt(vec3.X * vec3.X + vec3.Y * vec3.Y + vec3.Z * vec3.Z);
		if (length == 0.0f)
		{
			length = 1.0f;
		}
		return vec3 / length;
	}
}

const std::vector<D3D11_INPUT_ELEMENT_DESC> Model::m_VertexLayout = {
	{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "FaceNorm", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12u, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24u, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};
