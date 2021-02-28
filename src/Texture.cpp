#include "Texture.h"
#include "stb_image.h"

/* Constructor 
*  Require's image's path, in which part of the buffer identify it, its height and width, and number of bits per pixel.
 */
Texture::Texture(const std::string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	/* Flip Origin vertically 
	 * Corresponder origen de la imagen con el origen del frame
	 */
	stbi_set_flip_vertically_on_load(1);
	/* Load image found at path en un arreglo.
	 * 4 canales a cargar del imagen (RGBa asumido)
	 */
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); //4 channels

	/* Generate texture 
	* 1) Crear identificador
	* 2) Asociarlo con objeto
	* 3) Bind
	 */
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	/* Configure texture's parameters 
	* target = textura en 2D, nombre, parámetro
	 */
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	/* Visualize texture as 2D image 
	* todo lo que opengl encuentra en el buffer, lo puede identificar
	 */
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer ));
	/* Bind con 0 para limpiar memoria */
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	/* If local buffer has elements, free image loaded in stb */
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

/* Destructor */
Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

/* Activate texture */
void Texture::Bind(unsigned int slot /*= 0*/) const
{	
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	/* Bind what's sent to the texture with identifier */
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

/* Unbind */
void Texture::Unbind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
