#pragma once

#include <vector>
#include <set>
#include <map>
#include <string>
#include <memory> //unique_ptr, ...

#include "common/qt.h"

#ifndef HAS_MAKE_UNIQUE

namespace  std {
template <typename T>
unique_ptr<T> make_unique()
{
	return unique_ptr<T>(new T());
}

template <typename T, typename U>
unique_ptr<T> make_unique(U&& arg)
{
	return unique_ptr<T>(new T(arg));
}

template <typename T, typename U1, typename U2>
unique_ptr<T> make_unique(U1&& arg1, U2&& arg2)
{
	return unique_ptr<T>(new T(arg1, arg2));
}

template <typename T, typename U1, typename U2, typename U3>
unique_ptr<T> make_unique(U1&& arg1, U2&& arg2, U3&& arg3)
{
	return unique_ptr<T>(new T(arg1, arg2, arg3));
}

template <typename T, typename U1, typename U2, typename U3, typename U4>
unique_ptr<T> make_unique(U1&& arg1, U2&& arg2, U3&& arg3, U4&& arg4)
{
	return unique_ptr<T>(new T(arg1, arg2, arg3, arg4));
}

}

#endif

namespace boost
{
#ifdef BOOST_NO_EXCEPTIONS
	inline void throw_exception(std::exception const & e) {
		throw 11; // or whatever
	};
#endif
}// 

