#include "MeshRenderer.hpp"

#include <cassert>

#include "GameObject.hpp"
#include "Debug.hpp"
#include "Scene.hpp"
#include "MeshFilter.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Animator.hpp"
#include "Pipeline.hpp"
#include "Camera.hpp"
#include "Gizmos.hpp"
//#include "Shader.hpp"
#include "Graphics.hpp"


namespace FishEngine
{
    MeshRenderer::MeshRenderer(MaterialPtr material) : Renderer(material)
    {
        
    }
    
    Bounds MeshRenderer::localBounds() const
    {
        auto mf = gameObject()->GetComponent<MeshFilter>();
        if (mf == nullptr || mf->mesh() == nullptr)
            return Bounds();
        return mf->mesh()->bounds();
    }
    
    void MeshRenderer::Render() const
    {
        auto meshFilter = gameObject()->GetComponent<MeshFilter>();
        if (meshFilter == nullptr) {
            Debug::LogWarning("This GameObject has no MeshFilter");
            return;
        }
        
        auto model = transform()->localToWorldMatrix();
        Pipeline::UpdatePerDrawUniforms(model);
        
        //std::map<std::string, TexturePtr> textures;
        //auto& lights = Light::lights();
        //if (lights.size() > 0) {
        //    auto& l = lights.front();
        //    if (l->transform() != nullptr) {
        //        textures["ShadowMap"] = l->m_shadowMap;
        //    }
        //}
        
        const auto& mesh = meshFilter->mesh();
        
        for (auto& m : m_materials)
        {
#if 0
            auto shader = m->shader();
            assert(shader != nullptr);
            shader->Use();
            shader->PreRender();
            m->BindTextures(textures);
            m->BindProperties();
            shader->CheckStatus();
            mesh->Render();
            shader->PostRender();
#else
            Graphics::DrawMesh(mesh, m);
#endif
        }
    }
    
    void MeshRenderer::OnDrawGizmosSelected()
    {
        //Gizmos::setColor(Color::blue);
        //auto b = bounds();
        //Gizmos::DrawWireCube(b.center(), b.size());
    }
}
