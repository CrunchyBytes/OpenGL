#pragma once

class VertexBuffer
{
private:
	/* Identifier */
	unsigned int m_RendererID; 
public:
	/* Constructor */
	VertexBuffer(const void* data, unsigned int size);
	/* Destructor */
	~VertexBuffer();

	/* Bind and Unbind */
	void Bind() const;
	void Unbind() const;
};