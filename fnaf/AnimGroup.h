#pragma once
#include <vector>


#include "Anim.h"
#include "AnimState.h"
#include "Shader.h"

class AnimGroup {//一系列动画的组合产生的动画，可以实现透明度混合
public:
	std::vector<char> order;
	std::vector<Anim*> anim;
	std::vector<AnimState*> animSta;
	int count[2] = { 0 };
	//此处设计和渲染器相同
	Shader* shader;
	AnimGroup(const char* vertex, const char* fragment);
	void add(Anim*);
	void add(AnimState*);
	void updateCount(char type);
	void updateTex();
};