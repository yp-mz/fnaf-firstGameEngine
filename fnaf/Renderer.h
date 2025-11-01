#pragma once
#include <vector>
#include "Anim.h"
#include "Shader.h"
#include "AnimState.h"
#include "AnimGroup.h"
#include "Camera.h"

#define ANIM 0
#define ANIM_STATE 1
#define ANIM_GROUP 2

//#define NORMAL 0
//#define GROUP 1

class Renderer {//渲染流程：加入anim或animSta对象到渲染队列，渲染期间按照顺序绑定类中储存的vao tex依次渲染，如果有透明度选项则需要开启Group模式，纹理会在shader中进行混合运算
public:
	std::vector<char> order;//动画和动画状态机是不同的类，但是渲染的时候需要按照添加顺序渲染，因此可以通过添加它们的类型然后再在各自对应的类型列表中定位
	std::vector<Anim*> anim;//动画类渲染队列
	std::vector<AnimState*> animSta;//动画机类渲染队列
	std::vector<AnimGroup*> animGro;//动画机类渲染队列
	char count[3] = { 0 };//用count记录动画状态机列表和动画列表此时的索引，用于定位渲染列表的对象
	std::vector<unsigned int> shaders;
	unsigned int sid;//感觉其实应该写成sidNow，但是太多了就不想一个一个改

	int width, height;
	glm::mat4 otto;

	Camera *camera;
	//std::vector<int> mode;//用于储存当前的渲染模式
	//unsigned int modeNow;
	Renderer(Camera* c) {
		camera = c;
		sid = -1;
		width = 1280;
		height = 720;
		otto = glm::ortho(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, -0.1f, 0.1f);
		//modeNow = NORMAL;
	}
	void add(Anim* a);//add的动画类重载
	void add(AnimState* c);//add的动画状态机类重载
	void add(AnimGroup* g);//add的动画组类重载
	//add用于将对象添加到渲染队列
	void bind(Shader* a);//绑定着色器后，接下来添加的渲染对象渲染时会采用当前绑定的着色器
	//void setMode(unsigned int mode);
	void update();//更新帧
	void render();//渲染
private:
	void updateOtto();
	void updateCount(char type);//更新列表索引
};