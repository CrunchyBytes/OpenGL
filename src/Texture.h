#pragma once

#include "Renderer.h"

class Texture
{
private:
	/* Identifier */
	unsigned int m_RendererID;
	/* File path */
	std::string m_FilePath;
	/* Buffer's identifier -> GPU */
	unsigned char* m_LocalBuffer;
	/* Dimensions */
	int m_Width, m_Height, m_BPP;
public:
	/* Constructor and Destructor */
	Texture(const std::string& path);
	~Texture();
	
	/* Bind and Unbind*/
	void Bind(unsigned int slot = 0) const;
	void Unbind();

	/* Know array's size */
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};