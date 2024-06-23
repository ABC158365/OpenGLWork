#pragma once
#include<array>
class Noise
{
};


class PerlinNoise2D
{
public:
	PerlinNoise2D();
	~PerlinNoise2D();

	float BasePerlinNoise2D(float x, float y); //输出数值的范围应该是[-1,1]
	float Octave2D_01(float x, float y, int octaves, float persistence = 0.5);//输出数值的范围限定在[0,1]
private:
	template<typename STLIterator>
	inline void shuffle(STLIterator begin, STLIterator end);
	/*
		生成最大值为max的随机数
	*/
	int random(int max);
	/*
	input: 方向向量(x,y) 哈希值 hash
	根据哈希值可以达到随机梯度
	输出：随机梯度与方向向量的乘积
	*/
	inline float Grad(int hash, float x, float y);
	inline float Grad2(int hash, float x, float y);
	inline float Fade(const float t);
	inline float Lerp(const float a, const float b, const float t);
	inline float RemapClamp_01(float x);
	float Octave2D(float x, float y, int octaves, float persistence = 0.5);
private:
	std::array<int, 256> m_permutation;

};

