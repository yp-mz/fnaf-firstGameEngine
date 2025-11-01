#include <fstream>
#include <sstream>
#include<iostream>
#include<string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include"Anim.h"
#include"AnimState.h"
//#include"AnimGroup.h"
#include"Renderer.h"
#include"Shader.h"

//Shader
Shader::Shader(const char* vertex, const char* fragment) {
	// 1. 从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertex);
		fShaderFile.open(fragment);
		std::stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();



	unsigned int vid, fid;
	vid = glCreateShader(GL_VERTEX_SHADER);
	fid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vid, 1, &vShaderCode, NULL);
	glShaderSource(fid, 1, &fShaderCode, NULL);
	glCompileShader(vid);
	glCompileShader(fid);
	int success;
	char infoLog[512];
	glGetShaderiv(vid, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vid, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	this->ID = glCreateProgram();
	glAttachShader(this->ID,vid);
	glAttachShader(this->ID,fid);
	glLinkProgram(this->ID);

	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vid);
	glDeleteShader(fid);
}
void Shader::use() {
	glUseProgram(this->ID);
}
void Shader::setInt(const char* name, int value) {
	glUseProgram(this->ID);
	glUniform1i(glGetUniformLocation(this->ID, name), value);
	glUseProgram(0);
}
void Shader::setFloat(const char* name, float value) {
	glUseProgram(this->ID);
	glUniform1f(glGetUniformLocation(this->ID, name), value);
	glUseProgram(0);
}
void Shader::setMatrix(const char* name, glm::mat4* value) {
	glUseProgram(this->ID);
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(*value));
	glUseProgram(0);
}







//Anim
Anim::Anim(unsigned int s, const char* p) {
	this->frame = 0;
	this->size = s;
	this->path = p;
	this->trans[2] = 1;
	this->trans[3] = 1;
	this->isFlip = false;
	this->isLoop = true;
	this->isPlay = true;
	this->isVisible = true;
	this->isSetV = true;
		
	glGenBuffers(1, &this->vbo);
	glGenVertexArrays(1, &this->vao);
		
	glGenTextures(1, &this->tex);
	glBindTexture(GL_TEXTURE_2D, this->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 0,0,0,0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//opengl顶点初始化

	int chennal;
	std::string a = std::string(this->path) + '0' + ".png";
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(a.c_str(), &width, &height, &chennal, 0);
	//this->setSize(float(width), float(hight));
	glBindTexture(GL_TEXTURE_2D, this->tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	//加载初始化纹理（第一帧）

}
void Anim::changeF(){
	if (size > 1 && isPlay) {//若只有一帧则视为图像
		int w,h,chennal;
		stbi_set_flip_vertically_on_load(true);
		std::string a = std::string(this->path) + std::to_string(frame) + ".png";
		unsigned char* data = stbi_load(a.c_str(), &w, &h, &chennal, 0);
		if (std::abs(h - height) > height / 5 || std::abs(w - width) > width / 5)
			this->isSetV = true;
		width = w;
		height = h;
		glBindTexture(GL_TEXTURE_2D, this->tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		//加载纹理（每一帧）
		if (!isFlip) {
			if (this->frame < this->size - 1)
				this->frame++;
			else {
				if (isLoop)
					this->frame = 0;
				else
					isPlay = false;
			}
		}
		else {
			if (this->frame > 0)
				this->frame--;
			else {
				if (isLoop)
					this->frame = this->size - 1;
				else
					isPlay = false;
			}
		}
	}
}
void Anim::useVertex(){
	if (!this->isSetV)
		glBindVertexArray(this->vao);
	else {
		float vertices[] = {
					-1.0f, -1.0f, 0.0f,  0.0f,0.0f,
					1.0f, -1.0f, 0.0f,  1.0f,0.0f,
					-1.0f,  1.0f, 0.0f,  0.0f,1.0f,
					1.0f,  1.0f, 0.0f,  1.0f,1.0f
		};
		float a = float(this->height) / float(this->width);
		float b = 1 / a;
		if (this->height > this->width) {
			vertices[1] = -a;
			vertices[6] = -a;
			vertices[11] = a;
			vertices[16] = a;
			//std::cout << -w / h / 2;
		}
		else {
			vertices[0] = -b;
			vertices[5] = b;
			vertices[10] = -b;
			vertices[15] = b;
		}
		//按图片长宽比设置图片顶点坐标

		/*for(int i = 0;i<20;i++)
			std::cout << vertices[i] << "  ";
		std::cout << std::endl;*/
		glBindVertexArray(this->vao);
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		//glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
void Anim::setPos(float x, float y) {
	this->trans[0] = x;
	this->trans[1] = y;
}
void Anim::setSize(float x, float y) {
	this->trans[2] = x;
	this->trans[3] = y;
}









//Renderer
void Renderer::add(Anim* a) {
	if (this->sid == -1)
		std::cout << "未绑定着色器，bind shader first" << std::endl;
	else {
		this->order.push_back(ANIM);
		this->anim.push_back(a);
		this->shaders.push_back(this->sid);
	}
}
void Renderer::add(AnimState* c) {
	if (this->sid == -1)
		std::cout << "未绑定着色器，bind shader first" << std::endl;
	else {
		this->order.push_back(ANIM_STATE);
		this->animSta.push_back(c);
		this->shaders.push_back(this->sid);
	}
}
void Renderer::add(AnimGroup* g) {//动画组不需要绑定shader，因为它的shader有极强的专属性
	this->order.push_back(ANIM_GROUP);
	this->animGro.push_back(g);
	this->shaders.push_back(g->shader->ID);
	//g->updateTex();
}
void Renderer::bind(Shader* a) {
	this->sid = a->ID;
}
//void Renderer::setMode(unsigned int m) {
//	this->modeNow = m;
//}
void Renderer::update() {
	for (char i = 0; i < this->anim.size(); i++) {
		this->anim[i]->changeF();
		//std::cout << this->anim[i]->frame << std::endl;
	}
	for (char i = 0; i < this->animSta.size(); i++) {
		this->animSta[i]->nowAnim->changeF();
	}
	for (char i = 0; i < this->animGro.size(); i++) {
		AnimGroup* group = this->animGro[i];
		for (char j = 0; j < group->anim.size(); j++) {
			group->anim[j]->changeF();
		}
		for (char j = 0; j < group->animSta.size(); j++) {
			group->animSta[j]->nowAnim->changeF();
		}
	}
	//updateOtto();
}
void Renderer::render() {
	if (this->order.size() != 0) {
		glm::mat4 trans;
		glm::mat4 view = glm::mat4(1);
		view = glm::translate(view, glm::vec3(-this->camera->x, -this->camera->y, 0));
		for (char i = 0; i < this->order.size(); i++) {
			trans = glm::mat4(1);

			glUseProgram(this->shaders[i]);
			glActiveTexture(GL_TEXTURE0);
			if (this->order[i] == ANIM) {
				Anim* a = this->anim[this->count[0]];
				this->updateCount(ANIM);
				if (!a->isVisible)
					continue;

				this->anim[this->count[0]]->useVertex();
				//glBindVertexArray(this->anim[this->count[0]]->vao);
				glBindTexture(GL_TEXTURE_2D, this->anim[this->count[0]]->tex);

				trans = glm::translate(trans, glm::vec3(a->trans[0], a->trans[1], 0));
				trans = glm::scale(trans, glm::vec3(a->trans[2], a->trans[3], 0));
				glUniformMatrix4fv(glGetUniformLocation(this->shaders[i], "trans"), 1, GL_FALSE, glm::value_ptr(trans));
			}
			else if (this->order[i] == ANIM_STATE) {
				Anim* a = this->animSta[this->count[1]]->nowAnim;
				this->updateCount(ANIM_STATE);
				if (!a->isVisible)
					continue;
				
				this->animSta[this->count[1]]->nowAnim->useVertex();
				//glBindVertexArray(this->animSta[this->count[1]]->nowAnim->vao);
				glBindTexture(GL_TEXTURE_2D, this->animSta[this->count[1]]->nowAnim->tex);

				trans = glm::translate(trans, glm::vec3(a->trans[0], a->trans[1], 0));
				trans = glm::scale(trans, glm::vec3(a->trans[2], a->trans[3], 0));
				glUniformMatrix4fv(glGetUniformLocation(this->shaders[i], "trans"), 1, GL_FALSE, glm::value_ptr(trans));
			}
			else if (this->order[i] == ANIM_GROUP) {
				AnimGroup* group = this->animGro[this->count[2]];
				unsigned int width, height ,vao;
				if (group->order[0] == ANIM) {
					group->anim[0]->useVertex();
					width = group->anim[0]->width;
					height = group->anim[0]->height;
					//vao = group->anim[0]->vao;
				}
				else if (group->order[0] == ANIM_STATE) {
					group->animSta[0]->nowAnim->useVertex();
					width = group->animSta[0]->nowAnim->width;
					height = group->animSta[0]->nowAnim->height;
					//glUniformMatrix4fv(glGetUniformLocation(this->shaders[i], "trans"), 1, GL_FALSE, glm::value_ptr(trans));
					//vao = group->animSta[0]->nowAnim->vao;
				}
				else {
					std::cout << "空动画组或无效动画组，empty group/invalid group" << std::endl;
					break;
				}
				GLenum err;
				while ((err = glGetError()) != GL_NO_ERROR) {
					std::cout << "OpenGL error: " << err << std::endl;
				}

				unsigned int location = glGetUniformLocation(group->shader->ID, "trans");
				unsigned int bascLoc = glGetUniformLocation(group->shader->ID, "Tex");

				for (char j = 0; j < group->order.size(); j++) {
					trans = glm::mat4(1);
					glActiveTexture(GL_TEXTURE1 + j);
					if (group->order[j] == ANIM) {
						Anim* a = group->anim[group->count[0]];
						glBindTexture(GL_TEXTURE_2D, a->tex);

						trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
						trans = glm::scale(trans, glm::vec3(1 / a->trans[2], 1 / a->trans[3], 0));
						trans = glm::scale(trans, glm::vec3(float(width) / float(a->width), float(height) / float(a->height), 0));
						trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
						trans = glm::translate(trans, glm::vec3(-a->trans[0], -a->trans[1], 0));
						//std::cout << float(width) << "  " << float(a->width) << "  " << j << std::endl;
						//std::cout << float(height) << "  " << float(a->height) << "  " << j << std::endl;
						////注：顺序不能变，矩阵相乘没有交换律
						//std::cout << a->trans[0] << "  " << a->trans[1] << "  " << j << std::endl;
						//std::cout << a->trans[2] << "  " << a->trans[3] << "  " << j<< std::endl;
						group->updateCount(0);
					}
					else if (group->order[j] == ANIM_STATE) {
						Anim* a = group->animSta[group->count[1]]->nowAnim;
						glBindTexture(GL_TEXTURE_2D, a->tex);

						trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
						trans = glm::scale(trans, glm::vec3(1 / a->trans[2], 1 / a->trans[3], 0));
						trans = glm::scale(trans, glm::vec3(float(width) / float(a->width), float(height) / float(a->height), 0));
						trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
						trans = glm::translate(trans, glm::vec3(-a->trans[0], -a->trans[1], 0));

						trans = glm::scale(trans, glm::vec3(1 / a->trans[2], 1 / a->trans[3], 0));
						trans = glm::translate(trans, glm::vec3(-a->trans[0], -a->trans[1], 0));
						group->updateCount(1);
					}
					//glBindVertexArray(vao);
					glUniform1i(bascLoc + j, j+1);
					glUniformMatrix4fv(location + j, 1, GL_FALSE, glm::value_ptr(trans));
					GLenum err;
					while ((err = glGetError()) != GL_NO_ERROR) {
						std::cout << "OpenGL error: " << err << std::endl;
					}
					
				}
				this->updateCount(ANIM_GROUP);
			}
			glUniformMatrix4fv(glGetUniformLocation(this->shaders[i], "otto"), 1, GL_FALSE, glm::value_ptr(this->otto));
			glUniformMatrix4fv(glGetUniformLocation(this->shaders[i], "view"), 1, GL_FALSE, glm::value_ptr(view));
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cout << "OpenGL error: " << err << std::endl;
			}
		}
	}
	else
		std::cout << "渲染队列空，renderinglist has no object" << std::endl;
}
void Renderer::updateCount(char type) {
	if (type > 2)
		std::cout << "该渲染类别不存在" << std::endl;
	else {
		if (type == ANIM) {
			if (this->count[0] < this->anim.size() - 1)
				this->count[0]++;
			else
				this->count[0] = 0;
		}
		else if (type == ANIM_STATE) {
			if (this->count[1] < this->animSta.size() - 1)
				this->count[1]++;
			else
				this->count[1] = 0;
		}
		else if (type == ANIM_GROUP) {
			if (this->count[2] < this->animGro.size() - 1)
				this->count[2]++;
			else
				this->count[2] = 0;
		}
	}
}
void Renderer::updateOtto() {
	long long view[4];
	glGetInteger64v(GL_VIEWPORT, view);
	if (view[2] != this->width || view[3] != this->height) {
		float aspect_ratio = (float)view[2] / (float)view[3];
		// 假设原始希望显示的区域高度为2个单位
		float view_height = 2.0f;
		float view_width = view_height * aspect_ratio; // 关键：按比例调整宽度

		this->otto = glm::ortho(
		-view_width / 2,   // left
		view_width / 2,   // right
		-view_height / 2,  // bottom
		view_height / 2,  // top
		-1.0f, 1.0f      // near/far
	);
	this->width = view[2];
	this->height = view[3];
	std::cout << view[2] << "  " << view[3] << std::endl;
	}
}
//std::string n_To_cha(int num) {
//	if (num < 10) {
//		return {char('0' + num)};
//	}
//	else
//		return { char('0' + int(num / 10)),char('0' + int(num % 10)) };
//		
//}