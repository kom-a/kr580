#include "Texture2D.h"

#include <iostream>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const std::string& filename)
{
	glGenTextures(1, &m_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_set_flip_vertically_on_load(false);
	uint8_t* data = stbi_load(filename.c_str(), &m_Width, &m_Height, &m_Channels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// TODO: Use logger here
		std::cout << "Texture loaded: " << filename << std::endl;
	}
	else
	{
		// TODO: Use logger here
		std::cout << "failed to load texture: " << filename << std::endl;
	}

	stbi_image_free(data);
}

Texture2D::Texture2D(int width, int height, int channels, uint8_t* data)
	: m_Width(width), m_Height(height), m_Channels(channels)
{
	glGenTextures(1, &m_TextureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// TODO: Use logger here
	std::cout << "Texture created: " << width << ", " << height << ", " << channels << std::endl;
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_TextureID);
}

void Texture2D::Bind(int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}