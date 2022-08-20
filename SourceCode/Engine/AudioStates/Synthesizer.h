#pragma once
#include <limits.h>
#include <Library/Math.h>


template <class T>
size_t BitSizeOf()
{
	return sizeof(T) * 8 * 16 * 16 * 16 / 2;
}



namespace Synthesizer
{

	template <class T>
	T DryWet(T _dry, T _wet, float dw_factor)
	{
		return (_dry * (1 - dw_factor)) + _wet * (dw_factor);
	}


	template <class T>
	inline void Distort(T* data, float factor = 30.0f)
	{
		for (int ind = 0; ind < 44100; ++ind)
		{
			// Normalize
			double output{ (double)data[ind] };
			output /= (double)BitSizeOf<T>();

			// Distort
			output *= factor;
			output = Clamp<double>(-1.0, 1.0, output);
			output /= factor;

			// Revert
			output *= (double)BitSizeOf<T>();
			data[ind] = (T)output;

		}
	}

	template <class T>
	inline void Tremolo(T* data, float factor = 2.0f)
	{
		for (int ind = 0; ind < 44100; ++ind)
		{
			double output = (double)data[ind] * sinf(factor * (2 * DirectX::XM_PI) * ind / 44100);
			data[ind] = (T)output;
		}
	}

	template <class T>
	inline void Echo(T* data, float factor = 0.5f, int echo_count = 5, int data_size = 44100)
	{
		static const int prev{20000};			
		int count = 0;

		for (int ind = 0; ind < data_size; ++ind)
		{


			int cur_echo{1}, previous_reference{};
			double cur_factor{ factor };
			double output = (double)data[ind];
			while (cur_echo <= echo_count)
			{
				// Check previous reference to apply 
				previous_reference = ind - prev * cur_echo;

				// Factor is weakened after each echo
				cur_factor /= cur_echo;
				if (previous_reference < 0)
					break;
				output += (double)data[previous_reference] * cur_factor;
				
				if ((int)output > 255)
					++count;
				++cur_echo;
			}
			data[ind] = (T)output;
		}
	}
	

}