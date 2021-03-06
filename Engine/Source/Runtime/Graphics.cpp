#include "Graphics.hpp"
#include "Pipeline.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "RenderSettings.hpp"
#include "RenderSystem.hpp"

namespace FishEngine
{
    void Graphics::DrawMesh(const MeshPtr& mesh, const Matrix4x4& matrix, const MaterialPtr& material)
    {
        Pipeline::UpdatePerDrawUniforms(matrix);
        DrawMesh(mesh, material);
    }

    void Graphics::DrawMesh(const MeshPtr& mesh, const MaterialPtr& material)
    {
        //if (material->IsKeywordEnabled(ShaderKeyword::AmbientIBL))
        //{
        //    material->SetTexture("AmbientCubemap", RenderSettings::ambientCubemap());
        //    material->SetTexture("PreIntegratedGF", RenderSettings::preintegratedGF());
        //}

        auto shader = material->shader();
        shader->Use();
        if (shader->HasUniform("AmbientCubemap"))
        {
            shader->BindTexture("AmbientCubemap", RenderSettings::ambientCubemap());
        }
        if (shader->HasUniform("PreIntegratedGF"))
        {
            shader->BindTexture("PreIntegratedGF", RenderSettings::preintegratedGF());
        }
        shader->PreRender();
        material->BindProperties();
        shader->CheckStatus();
        mesh->Render();
        shader->PostRender();
    }
}

