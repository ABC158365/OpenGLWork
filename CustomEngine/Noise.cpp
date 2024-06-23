#include "Noise.h"

#include <random>
#include <numeric>
PerlinNoise2D::PerlinNoise2D()
{
	std::iota(m_permutation.begin(), m_permutation.end(), 0);
	shuffle(m_permutation.begin(), m_permutation.end());
}

PerlinNoise2D::~PerlinNoise2D()
{

}


int PerlinNoise2D::random(int max)
{
	return (std::random_device{}() % (max)+1);
}

/*
洗牌算法
*/
template<typename STLIterator>
inline void PerlinNoise2D::shuffle(STLIterator begin, STLIterator end)
{
	if (begin == end)
	{
		return;
	}
	using difference_type = typename std::iterator_traits<STLIterator>::difference_type;
	for (STLIterator it = begin + 1; it < end; ++it)
	{
		int n = random(static_cast<int>(it - begin));
		std::iter_swap(it, begin + static_cast<difference_type>(n));
	}

}

inline float PerlinNoise2D::Grad(int hash, float x, float y)
{
	float z = 0.34567;
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; // never happens
	}
}

inline float PerlinNoise2D::Grad2(int hash, float x, float y)
{
	const double PI = 3.14159265359;
	const int numPoints = 36;
	hash = hash % numPoints;
	double angle = 2 * PI * hash / numPoints;
	double gradx = cos(angle);
	double grady = sin(angle);

	return gradx * x + grady * y;
}

inline float PerlinNoise2D::Fade(const float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}


inline float PerlinNoise2D::Lerp(const float a, const float b, const float t)
{
	return (a + (b - a) * t);
}
float PerlinNoise2D::BasePerlinNoise2D(float x, float y)
{
	//得到周围四个点
	int _x = std::floor(x);
	int _y = std::floor(y);

	int ix = _x & 255;
	int iy = _y & 255;
	//hash函数得到随机索引值
	int AA = m_permutation[(m_permutation[ix & 255] + iy) & 255];
	int BA = m_permutation[(m_permutation[(ix + 1) & 255] + iy) & 255];
	int AB = m_permutation[(m_permutation[ix & 255] + iy + 1) & 255];
	int BB = m_permutation[(m_permutation[ix + 1 & 255] + iy + 1) & 255];

	//根据索引值 得到方向向量和随机梯度的向量积

	float fx = x - _x;
	float fy = y - _y;

	float g1 = Grad2(AA, fx, fy);
	float g2 = Grad2(BA, fx - 1, fy);
	float g3 = Grad2(AB, fx, fy - 1);
	float g4 = Grad2(BB, fx - 1, fy - 1);

	//插值
	float u = Fade(fx);
	float v = Fade(fy);

	float x1 = Lerp(g1, g2, u);
	float x2 = Lerp(g3, g4, u);

	return Lerp(x1, x2, v);
}


float PerlinNoise2D::Octave2D(float x, float y, int octaves, float persistence)
{
	float result = 0.0;
	float amplitude = 1.0;

	for (int i = 0; i < octaves; i++)
	{
		result += (BasePerlinNoise2D(x, y) * amplitude);
		x *= 2;
		y *= 2;
		amplitude *= persistence;
	}
	return result;
}


inline float PerlinNoise2D::RemapClamp_01(float x)
{
	if (x <= -1.0)
	{
		return 0.0;
	}
	else if (1.0 <= x)
	{
		return 1.0;
	}

	return (x * 0.5 + 0.5);
}


float PerlinNoise2D::Octave2D_01(float x, float y, int octaves, float persistence)
{
	return RemapClamp_01(Octave2D(x, y, octaves));
}
