#pragma once
#include "Singleton.h"
#include "DXMath.h"
#include "CerealHeaders.h"
#include <map>
#include <fstream>
#include <memory>
#include <string>
#include "DEBUG_PRIMITIVE.h"
using namespace Math;
static std::string input{ "" };
static std::string output_file{ "" }, input_file{ "" };
class DIRECTIONAL_LIGHT;
class POINT_LIGHT;
class SPOTLIGHT;




#pragma region LIGHT_DATASET




struct DLIGHT_DATA
{
	Vector4 direction{};
	Vector4 colour{};

};

struct PLIGHT_DATA
{
	Vector4 position{};
	Vector4 colour{};
	float range{};
	Vector3 temp{};
};

struct SLIGHT_DATA
{
	Vector4 position{};
	Vector4 direction{};
	Vector4 colour{};
	float range{};
	float inner{};
	float outer{};
	float temp{};
};


#pragma endregion


class LIGHTING 
{
	std::shared_ptr<DEBUG_PRIMITIVE>d_Primitive;
public:




	enum class L_TYPE
	{
		DIRECTIONAL, POINT, SPOT
	};
private:
	L_TYPE type{L_TYPE::DIRECTIONAL};
	 Vector3 direction{};
	 Vector3 position{};
	 Vector4 colour{};
	 float range{};
	 float inner{};
	 float outer{};
	//std::shared_ptr<LIGHT_DATA_BASE>data;

	friend class LIGHTINGMANAGER;
public:
	LIGHTING() {}
	LIGHTING(L_TYPE t);
	template <typename T>
	void WriteBuffer(T* t) {
		//t = (T*)data.get();
		switch (type)
		{
		case L_TYPE::DIRECTIONAL:
			((DLIGHT_DATA*)t)->direction = { direction.x, direction.y, direction.z, 0.0f };
			((DLIGHT_DATA*)t)->colour = colour;
			break;
		case L_TYPE::POINT:
			((PLIGHT_DATA*)t)->position = { position.x, position.y, position.z, 0 };
			((PLIGHT_DATA*)t)->colour = colour;
			((PLIGHT_DATA*)t)->range = range;
			break;
		case L_TYPE::SPOT:
			((SLIGHT_DATA*)t)->position = { position.x, position.y, position.z, 0 };
			((SLIGHT_DATA*)t)->direction = { direction.x, direction.y, direction.z, 0.0f };
			((SLIGHT_DATA*)t)->colour = colour;
			((SLIGHT_DATA*)t)->range = range;
			((SLIGHT_DATA*)t)->inner = inner;
			((SLIGHT_DATA*)t)->outer= outer;
			break;
		};

	}
	void WriteToData(DIRECTIONAL_LIGHT* d);
	void WriteToData(POINT_LIGHT* p);
	void WriteToData(SPOTLIGHT* s);
	L_TYPE Type();
	Vector3 Direction();
	Vector3 Position();
	Vector4 Colour();
	float Range();
	float Inner();
	float Outer();




	void RenderDebug();

	void SetDirection(Vector3 dir);
	void SetPosition(Vector3 pos);
	void SetColour(Vector4 col);
	void SetRange(float r);
	void SetInnerCorner(float inner);
	void SetOuterCorner(float outer);
	void SetType(L_TYPE t);

	template<class T>
	void serialize(T& t)
	{
		t(type, direction, position, colour, range, inner, outer);
	}
};


class LIGHTINGMANAGER : public Singleton<LIGHTINGMANAGER>
{
	std::map<std::string, std::shared_ptr<LIGHTING>>dataset;
public:
	void Insert(std::string n, std::shared_ptr<LIGHTING>l);
	void Remove(std::string n);
	void Create(std::string n, LIGHTING::L_TYPE t);

	void RenderUI();
	void OutputFile(std::string f);
	void ReadFromFile(std::string f);
	void RenderDebug();

	std::string Name(std::shared_ptr<LIGHTING>d);
	std::shared_ptr<LIGHTING>Retrieve(std::string n);
	std::map<std::string, std::shared_ptr<LIGHTING>>Dataset() { return dataset; }


	template <class T>
	void serialize(T& t)
	{
		t(dataset);
	}
};