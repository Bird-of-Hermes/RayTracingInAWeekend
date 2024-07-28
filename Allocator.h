#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <utility>
#include <stdexcept>
#include <cstring>
#include <vector>
#include <unordered_map>
#include "ErrorEnum.h"

namespace RT
{
	class Allocator
	{
		friend class string;
		friend class wstring;

	public:

		~Allocator()
		{
			Release();
		}

		RESULT_VALUE Allocate(size_t toAllocate)
		{
			if (_data == nullptr)
			{
				_size = toAllocate;
				_data = new char[toAllocate] {0};
				return RESULT_VALUE::OK;
			}
			return RESULT_VALUE::GENERIC_ERROR;
		}

		RESULT_VALUE GiveAddress(size_t bytesRequested, void** ptr)
		{
			if (_data == nullptr)
			{
				return RESULT_VALUE::ALLOCATOR_NOT_INITIALIZED;
			}
			if (bytesRequested + _consumed > _size)
			{
				memset(_data, 0, _size); // zero's everything and starts over.
				_consumed = 0;
			}

			*ptr = _data + _consumed;
			_consumed += bytesRequested;

			return RESULT_VALUE::OK;
		}

		void Release()
		{
			if (_data == nullptr)
			{
				delete[] _data;
				_data = nullptr;
				_size = 0;
				_consumed = 0;
			}
		}

	private:

		size_t _size = 0;
		size_t _consumed = 0;
		char* _data = nullptr;
	};

	class string
	{
	public:
		string(const char* str = "")
		{
			if (strAllocator._data == nullptr)
			{
				strAllocator.Allocate(1024ull * 1024); // 1mb
			}
			_size = strnlen(str, 1024);

			if (strAllocator.GiveAddress(_size + 1, reinterpret_cast<void**>(&_data)) != RESULT_VALUE::OK)
			{
				throw std::exception();
			}
			
			memcpy(_data, str, _size);

			_data[_size] = '\0';
		}

		~string()
		{
			_data = nullptr;
		}

		size_t size() const noexcept
		{
			return _size;
		}

		char* c_str() noexcept
		{
			return &_data[0];
		}

		char* operator[](size_t index) const noexcept
		{
			return &_data[index];
		}

		string& operator+=(const string& other)
		{
			size_t newSize = _size + other._size;
			char* newData = nullptr;

			if (strAllocator.GiveAddress(newSize + 1, reinterpret_cast<void**>(&newData)) != RESULT_VALUE::OK)
			{
				throw std::exception();
			}

			memcpy(newData, _data, _size);
			memcpy(newData + _size, other._data, other._size);

			_data = newData;
			_size = newSize;
			_data[newSize] = '\0';

			return *this;
		}

		friend string operator+(const string& lhs, int num)
		{
			char numStr[12];
			int len = snprintf(numStr, sizeof(numStr), "%d", num);

			if (len < 0 || len >= sizeof(numStr)) {
				throw std::exception();
			}

			size_t newSize = lhs._size + len;
			string result;

			if (strAllocator.GiveAddress(newSize + 1, reinterpret_cast<void**>(&result._data)) != RESULT_VALUE::OK)
			{
				throw std::exception();
			}

			memcpy(result._data, lhs._data, lhs._size);
			memcpy(result._data + lhs._size, numStr, len);
			result._size = newSize;
			result._data[newSize] = '\0';

			return result;
		}

		string& operator+=(int num)
		{
			char numStr[12];
			int len = snprintf(numStr, sizeof(numStr), "%d", num);

			if (len < 0 || len >= sizeof(numStr)) {
				throw std::exception();
			}

			size_t oldSize = _size;
			size_t newSize = _size + len;
			char* newData = nullptr;

			if (strAllocator.GiveAddress(newSize + 1, reinterpret_cast<void**>(&newData)) != RESULT_VALUE::OK)
			{
				throw std::exception();
			}

			memcpy(newData, _data, oldSize);
			memcpy(newData + oldSize, numStr, len);

			_data = newData;
			_size = newSize;
			_data[newSize] = '\0';

			return *this;
		}

		char* at(size_t index) const
		{
			if (index > 0 && index < _size)
			{
				return &_data[index];
			}
			else
			{
				// out of bounds
				throw std::out_of_range("Indexed out of range");
			}
		}

	private:
		char* _data = nullptr;
		size_t _size = 0;
		size_t _capacity = 0;
		inline static Allocator strAllocator = {};
	};

	class wstring
	{
	public:
		wstring(const wchar_t* str = L"")
		{
			if (strAllocator._data == nullptr)
			{
				strAllocator.Allocate(1024ull * 1024); // 1mb
			}
			_size = wcsnlen(str, 1024);

			if (strAllocator.GiveAddress(_size + 2, reinterpret_cast<void**>(&_data)) != RESULT_VALUE::OK)
			{
				// insuficient memory
				throw std::exception();
			}

			memcpy(_data, str, _size);

			_data[_size] = L'\0';
		}
		~wstring()
		{
			_data = nullptr;
		}

		size_t size() const noexcept
		{
			return _size;
		}

		wchar_t* c_str() noexcept
		{
			return &_data[0];
		}

		wchar_t* operator[](size_t index) const noexcept
		{
			return &_data[index];
		}

		wchar_t* at(size_t index) const
		{
			if (index > 0 && index < _size)
			{
				return &_data[index];
			}
			else
			{
				// out of bounds
				throw std::out_of_range("Indexed out of range");
			}
		}

	private:
		wchar_t* _data = nullptr;
		size_t _size = 0;
		inline static Allocator strAllocator = {};
	};
};

#endif