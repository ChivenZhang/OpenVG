#pragma once

#if defined( _MSVC_LANG )
#	define OPENVG_CPLUSPLUS _MSVC_LANG
#else
#	define OPENVG_CPLUSPLUS __cplusplus
#endif
#if 201703L < OPENVG_CPLUSPLUS
#	define OPENVG_CPP_VERSION 20
#elif 201402L < OPENVG_CPLUSPLUS
#	define OPENVG_CPP_VERSION 17
#elif 201103L < OPENVG_CPLUSPLUS
#	define OPENVG_CPP_VERSION 14
#elif 199711L < OPENVG_CPLUSPLUS
#	define OPENVG_CPP_VERSION 11
#else
#	error "At least c++ standard version 11"
#endif

// ============================================

#ifdef OPENVG_SHARED_LIBRARY
#	if defined(_WIN32)
#		define OPENVG_API __declspec(dllexport)
#		define OPENVG_C_API extern "C" __declspec(dllexport)
#	else
#		define OPENVG_API __attribute__((visibility("default")))
#		define OPENVG_C_API extern "C" __attribute__((visibility("default")))
#	endif
#else
#	if defined(_WIN32)
#		define OPENVG_API 
#		define OPENVG_C_API extern "C" 
#	else
#		define OPENVG_API 
#		define OPENVG_C_API 
#	endif
#endif

// ============================================

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#pragma warning(error:4150)
#pragma warning(disable:4250)
#pragma warning(disable:4200)
#pragma warning(disable:26812)
#pragma warning(disable:26815)
#pragma warning(disable:26816)

#define _USE_MATH_DEFINES
#include <math.h>
#include <assert.h>
#include <iostream>
#include <utility>
#include <algorithm>
#include <sstream>
#include <memory>
#include <string>
#include <array>
#if 20 <= OPENVG_CPP_VERSION
#include <span>
#else
#include "span.h"
#endif
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>
#include <bitset>
#include <exception>
#include <functional>

// ============================================

template<class T>
using VGRaw = T*;
template<class T>
using VGRef = std::shared_ptr<T>;
using VGString = std::string;
using VGCString = const char*;
using VGWString = std::wstring;
#if 20 <= OPENVG_CPP_VERSION
using VGString8 = std::u8string;
#else
using VGString8 = std::string;
#endif
using VGString16 = std::u16string;
using VGString32 = std::u32string;
using VGStringView = std::string_view;
using VGWStringView = std::wstring_view;
#if 20 <= OPENVG_CPP_VERSION
using VGString8View = std::u8string_view;
#else
using VGString8View = std::string_view;
#endif
using VGString16View = std::u16string_view;
using VGString32View = std::u32string_view;
template <class T, size_t N>
using VGArray = std::array<T, N>;
template <class T, size_t N = std::dynamic_extent>
using VGArrayView = std::span<T, N>;
template <class T>
using VGVector = std::vector<T>;
template <class T>
using VGDeque = std::deque<T>;
template <class T>
using VGList = std::list<T>;
template <class T, class L = std::less<T>>
using VGSet = std::set<T, L>;
template <class K, class T, class L = std::less<K>>
using VGMultiSet = std::multiset<K, T, L>;
template <class K, class H = std::hash<K>, class E = std::equal_to<K>>
using VGHashSet = std::unordered_set<K, H, E>;
template <class K, class T, class L = std::less<K>>
using VGMap = std::map<K, T, L>;
template <class K, class T, class L = std::less<K>>
using VGMultiMap = std::multimap<K, T, L>;
template <class K, class T, class H = std::hash<K>, class E = std::equal_to<K>>
using VGHashMap = std::unordered_map<K, T, H, E>;
template <class T>
using VGQueue = std::queue<T>;
template <class T, class C = VGVector<T>, class L = std::less<typename C::value_type>>
using VGPriorityQueue = std::priority_queue<T, C, L>;
template <class T>
using VGStack = std::stack<T>;
template <size_t N>
using VGBitset = std::bitset<N>;
template <class T, class U>
using VGBinary = std::pair<T, U>;
template <class ...TS>
using VGTuple = std::tuple<TS...>;
using VGException = std::exception;
template <class T>
using VGLambda = std::function<T>;
using VGStringList = VGVector<VGString>;
using VGWStringList = VGVector<VGWString>;
using VGString8List = VGVector<VGString8>;
using VGString16List = VGVector<VGString16>;
using VGString32List = VGVector<VGString32>;
template<class T>
using VGStringMap = VGMap<VGString, T>;
template<class T>
using VGWStringMap = VGMap<VGWString, T>;
template<class T>
using VGString8Map = VGMap<VGString8, T>;
template<class T>
using VGString16Map = VGMap<VGString16, T>;
template<class T>
using VGString32Map = VGMap<VGString32, T>;
template<class T>
using VGStringHashMap = VGHashMap<VGString, T>;
template<class T>
using VGWStringHashMap = VGHashMap<VGWString, T>;
template<class T>
using VGString8HashMap = VGHashMap<VGString8, T>;
template<class T>
using VGString16HashMap = VGHashMap<VGString16, T>;
template<class T>
using VGString32HashMap = VGHashMap<VGString32, T>;

// ============================================

template<typename T, typename ... Args>
inline VGRef<T> VGNew(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}
template<typename U, typename T>
inline VGRef<U> VGCast(VGRef<T>&& target)
{
	if (target == nullptr) return nullptr;
	return std::dynamic_pointer_cast<U>(target);
}
template<typename U, typename T>
inline VGRef<U> VGCast(VGRef<T> const& target)
{
	if (target == nullptr) return nullptr;
	return std::dynamic_pointer_cast<U>(target);
}
template<typename U, typename T>
inline VGRaw<U> VGCast(VGRaw<T>&& target)
{
	if (target == nullptr) return nullptr;
	return dynamic_cast<U*>((T*)target);
}
template<typename U, typename T>
inline VGRaw<U> VGCast(VGRaw<T> const& target)
{
	if (target == nullptr) return nullptr;
	return const_cast<U*>(dynamic_cast<const U*>((const T*)target));
}

inline constexpr uint32_t VGHash(VGCString value)
{
	uint32_t hash = 0; // From JDK 8
	if (value == nullptr) return hash;
	while (*value) hash = hash * 31 + (*value++);
	return hash;
}

// ============================================