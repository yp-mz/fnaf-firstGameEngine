#pragma once
#include "Shader.h"
#include <glm/glm.hpp>

class Anim {//这是一个用于控制动画帧的类
public:
	unsigned int size, frame;
	int width, height;
	unsigned int vao, vbo, tex;
	const char* path;
	bool isLoop,isVisible,isSetV,isFlip,isPlay;
	float trans[4];
	glm::mat4 t;

	Anim(unsigned int s, const char* p);//输入动画的大小（帧数）和相对路径，生成一个动画
	void setPos(float x, float y);
	void setSize(float x,float y);
	void changeF();//改变动画的当前帧，通常每帧改变一次，从当前帧转为下一帧
	void useVertex();//设置放置动画的顶点的大小，应与动画帧的长宽比对应
};