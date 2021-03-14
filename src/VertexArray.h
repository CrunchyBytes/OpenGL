#pragma once

/* Use vertex buffer's data */
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	/* Identifier */
	unsigned int m_RendererID;
public:
	/* Constructor */
	VertexArray();
	/* Destructor */
	~VertexArray();

	/* Add data to the buffer */
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	/* Bind and Unbind */
	void Bind() const;
	void Unbind() const;
};