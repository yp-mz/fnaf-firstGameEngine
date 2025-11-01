#pragma once

#include <vector>
#include <map>
#include <string>
#include"Anim.h"


class AnimState {//这个类用于管理多个动画类
public:
	Anim* nowAnim;
	AnimState() {
		nowAnim = nullptr;
	}
	void addAnim(std::string name,unsigned size,const char*path);//添加动画并命名
	void setAnim(std::string name);//设置当前动画状态机动画
private:
	std::map<std::string, Anim*> anims;
};