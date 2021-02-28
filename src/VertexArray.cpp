#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

/* Create vertex array */
VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	
}

/* Delete specified vertex array */
VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();  //bind the vertex array
	vb.Bind();	//bind the vertex buffer
	/* Read layout's elements */
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

/* Bind vertex array */
void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

/* Unbind vertex array */
void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}