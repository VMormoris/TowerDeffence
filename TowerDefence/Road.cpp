#include "Road.h"



Road::Road(void): Engine::GameObject(){
	for (int i = 0; i < 29; i++) {
		Engine::Object3D* tile = new Tile();
		tile->setExtra(glm::radians(0.f), glm::vec3(2.f * roadmap[i][0], 0.f, 2.f * roadmap[i][1]), glm::vec3(0.f,1.f,0.f));
		objects.push_back(tile);
	}
	center = glm::vec3(0.f,0.01f, 0.f);
	angle = glm::radians(0.f);
	rotation_vector = glm::vec3(0.f, 1.f, 0.f);
	scale = glm::vec3(2.f, 1.f, 2.f);
}

float Road::roadmap[29][3] = {
	{0.f, 0.f, 0.f},//0
	{ 0.f, 1.f, 0.f},//1
	{ 0.f, 2.f, 0.f },//2
	{ 0.f, 3.f, 0.f },//3
	{ 1.f, 3.f, 1.f },//4
	{ 1.f, 4.f, 2.f },//5
	{ 1.f, 5.f, 0.f },//6
	{ 1.f, 6.f, 0.f},//7
	{ 1.f, 7.f, 0.f},//8
	{ 2.f, 7.f, 1.f },//9
	{ 2.f, 8.f, 2.f },//10
	{ 3.f, 8.f, 1.f },//11
	{ 4.f, 8.f, 0.f },//12
	{ 5.f, 8.f, 0.f },//13
	{ 6.f, 8.f, 0.f },//14
	{ 6.f, 7.f, 1.f },//15
	{ 6.f, 6.f, 0.f },//16
	{ 7.f, 6.f, 2.f },//17
	{ 7.f, 5.f, 1.f },//18
	{ 7.f, 4.f, 0.f },//19
	{ 7.f, 3.f, 0.f },//20
	{ 8.f, 3.f, 2.f },//21
	{ 9.f, 3.f, 0.f },//22
	{ 9.f, 2.f, 1.f },//23
	{ 9.f, 1.f, 0.f},//24
	{ 8.f, 1.f, 1.f },//25
	{ 7.f, 1.f, 0.f },//26
	{ 6.f, 1.f, 0.f },//27
{ 6.f, 0.f, 2.f } };//28