#pragma once
#include "glcommon.h"
#include <vector>

class xtVertexBufferObject
{
public:
	xtVertexBufferObject(void);
	~xtVertexBufferObject(void);

	void CreateVBO(int a_iSize = 0);
	void ReleaseVBO();

	void *MapBufferToMemory(int iUsageHint );
	void *MapSubBufferToMemory(int iUsageHint, unsigned int uiOffset, unsigned int uiLength);
	void UnMapBuffer();

	void BindVBO(int a_iBufferType=GL_ARRAY_BUFFER);
	void UpLoadDataToGPU(int iUsageHint );

	void AddData(void *ptrData, unsigned int uiDataSize);

	void *GetDataPointer();
	unsigned int GetBuffer();

private:
	unsigned int uiBuffer;
	int iSize;
	int iBufferType;
	std::vector<unsigned char> data;

	bool bDataUploaded;
};

