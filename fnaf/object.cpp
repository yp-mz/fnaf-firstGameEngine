#include <iostream>

#include <string>

#include <glad/glad.h>

#include "AnimState.h"
#include "AnimGroup.h"
#include "Anim.h"
#include "Shader.h"

void AnimState::addAnim(std::string name, unsigned size, const char* path) {
	this->anims[name] = new Anim(size, path);
}
void AnimState::setAnim(std::string name) {
	if (this->anims.count(name) == 1) {
		this->nowAnim = this->anims[name];
		this->nowAnim->frame = 0;
	}
	else
		std::cout << "动画不存在，anim was not found" << std::endl;
}


AnimGroup::AnimGroup(const char* vertex, const char* fragment) {
	this->shader = new Shader(vertex, fragment);
}
void AnimGroup::add(Anim* a) {
	this->order.push_back(0);
	this->anim.push_back(a);
}
void AnimGroup::add(AnimState* c) {
	this->order.push_back(1);
	this->animSta.push_back(c);
}
void AnimGroup::updateCount(char type) {
	if (type > 1)
		std::cout << "该动画类别不存在" << std::endl;
	else {
		if (type == 0) {
			if (this->count[0] < this->anim.size() - 1)
				this->count[0]++;
			else
				this->count[0] = 0;
		}
		else if (type == 1) {
			if (this->count[1] < this->animSta.size() - 1)
				this->count[1]++;
			else
				this->count[1] = 0;
		}
	}
}
void AnimGroup::updateTex() {
	this->shader->use();
	char location = glGetUniformLocation(this->shader->ID, "Tex");
	for (char i = 0; i < this->order.size(); i++) {
		glUniform1i(location + i, i + 1);
	}
}