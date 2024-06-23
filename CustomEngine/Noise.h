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

	float BasePerlinNoise2D(float x, float y); //�����ֵ�ķ�ΧӦ����[-1,1]
	float Octave2D_01(float x, float y, int octaves, float persistence = 0.5);//�����ֵ�ķ�Χ�޶���[0,1]
private:
	template<typename STLIterator>
	inline void shuffle(STLIterator begin, STLIterator end);
	/*
		�������ֵΪmax�������
	*/
	int random(int max);
	/*
	input: ��������(x,y) ��ϣֵ hash
	���ݹ�ϣֵ���Դﵽ����ݶ�
	���������ݶ��뷽�������ĳ˻�
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

