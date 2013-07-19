#pragma once
typedef void* xtLPVOID;

class xtCreateScene
{
public:
	xtCreateScene(void);
	~xtCreateScene(void);
};

void initScene(xtLPVOID);
void renderScene(xtLPVOID);
void releaseScene(xtLPVOID);

