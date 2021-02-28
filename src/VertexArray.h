#pragma once

/* Use vertex buffer's data */
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	/* Identifier, to use when rendering. */
	unsigned int m_RendererID;
public:
	/* Constructor and Destructor */
	VertexArray();
	~VertexArray();

	/* Add data to the buffer */
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	/* Bind and unbind */
	void Bind() const;
	void Unbind() const;
};