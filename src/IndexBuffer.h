#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	/* Constructor and Destructor */
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	/* Bind and Unbind */
	void Bind() const;
	void Unbind() const;
	
	/* Return number of elements in buffer */
	inline unsigned int GetCount() const { return m_Count; }
};