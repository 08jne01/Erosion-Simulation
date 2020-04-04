#pragma once
#include <vector>
template<typename T> void offsetVector(std::vector<T>& vector, const T offset)

{
	for (int i = 0; i < vector.size(); i++)

	{
		vector[i] += offset;
	}
}