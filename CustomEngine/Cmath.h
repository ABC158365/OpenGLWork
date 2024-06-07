#pragma once
namespace Cmath {

	static float radians(float degrees)
	{
		return degrees * static_cast<float>(0.01745329251994329576923690768489);
	}

	template<typename genType>
	static genType pi()
	{
		return static_cast<genType>(3.14159265358979323846264338327950288);
	}
}



