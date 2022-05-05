#pragma once
//现在是用的别人的版本 未来可能会自己重新写一个更好的
struct Primitive
{
	Primitive(unsigned int _num) : verticesNum(_num) {};

	void draw(const unsigned int readTex1 = 0, const unsigned int readTex2 = 0, const unsigned int readTex3 = 0);
	virtual void setupMesh()=0;

	unsigned int VAO = 0, VBO = 0;
	const unsigned int verticesNum;
};

struct Cube : public Primitive {
	Cube() : Primitive(36) {};

	virtual void setupMesh() override;
};
