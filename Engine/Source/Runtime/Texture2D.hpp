#pragma once

#include "Texture.hpp"

namespace FishEngine
{
	class Texture2D : public Texture
    {
    public:
        
		InjectClassName(Texture2D);

		Texture2D() = default;

		Texture2D(int width, int height, TextureFormat format, uint8_t* data, int byteCount);

        // The format of the pixel data in the texture (Read Only).
        TextureFormat format() const
        {
            return m_format;
        }
        
        // How many mipmap levels are in this texture (Read Only).
        uint32_t mipmapCount() const
        {
            return m_mipmapCount;
        }
	protected:

		virtual void UploadToGPU() const override;

    private:

		friend class FishEditor::TextureImporter;
        
        // The format of the pixel data in the texture (Read Only).
        TextureFormat m_format;
        
        // How many mipmap levels are in this texture (Read Only).
        uint32_t m_mipmapCount;
        
        static Texture2DPtr m_blackTexture;
        static Texture2DPtr m_whiteTexture;
    };
}