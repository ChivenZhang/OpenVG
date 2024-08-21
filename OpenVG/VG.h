﻿#pragma once

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

struct VGFloat2
{
	float X = 0, Y = 0;
};

inline bool operator ==(VGFloat2 const& a, VGFloat2 const& b)
{
	return a.X == b.X && a.Y == b.Y;
}

struct VGFloat3
{
	float X = 0, Y = 0, Z = 0;
};

inline bool operator ==(VGFloat3 const& a, VGFloat3 const& b)
{
	return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

struct VGFloat4
{
	float X = 0, Y = 0, Z = 0, W = 0;
};

inline bool operator ==(VGFloat4 const& a, VGFloat4 const& b)
{
	return a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;
}

struct VGFloat2x2
{
	VGFloat2 X, Y;
};

inline bool operator ==(VGFloat2x2 const& a, VGFloat2x2 const& b)
{
	return a.X == b.X && a.Y == b.Y;
}

struct VGFloat3x3
{
	VGFloat3 X, Y, Z;
};

inline bool operator ==(VGFloat3x3 const& a, VGFloat3x3 const& b)
{
	return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
}

struct VGFloat4x4
{
	VGFloat4 X, Y, Z, W;
};

inline bool operator ==(VGFloat4x4 const& a, VGFloat4x4 const& b)
{
	return a.X == b.X && a.Y == b.Y && a.Z == b.Z && a.W == b.W;
}
struct VGRect
{
	float X = 0, Y = 0, W = 0, H = 0;
};

struct VGPoint
{
	float X = 0, Y = 0;
};

enum class VGPointType : uint8_t
{
	Close = 0, ///< Ends the current sub-path and connects it with its initial point. This command doesn't expect any points.
	MoveTo,    ///< Sets a new initial point of the sub-path and a new current point. This command expects 1 point: the starting position.
	LineTo,    ///< Draws a line from the current point to the given point and sets a new value of the current point. This command expects 1 point: the end-position of the line.
	CubicTo,   ///< Draws a cubic Bezier curve from the current point to the given point using two given control points and sets a new value of the current point. This command expects 3 points: the 1st control-point, the 2nd control-point, the end-point of the curve.
};

struct VGColor
{
	float R = 0, B = 0, G = 0, A = 0;
};

struct VGColorStop
{
	float Offset = 0;
	float R = 0, B = 0, G = 0, A = 0;
};

struct VGImage
{
	uint32_t Width = 0, Height = 0, Stride = 0;
	VGArrayView<const uint8_t> Data;
};
using VGImageRaw = VGRaw<VGImage>;

enum class VGStrokeCap : uint8_t
{
	Square = 0, ///< The stroke is extended in both end-points of a sub-path by a rectangle, with the width equal to the stroke width and the length equal to the half of the stroke width. For zero length sub-paths the square is rendered with the size of the stroke width.
	Round,      ///< The stroke is extended in both end-points of a sub-path by a half circle, with a radius equal to the half of a stroke width. For zero length sub-paths a full circle is rendered.
	Butt        ///< The stroke ends exactly at each of the two end-points of a sub-path. For zero length sub-paths no stroke is rendered.
};

enum class VGStrokeJoin : uint8_t
{
	Bevel = 0, ///< The outer corner of the joined path segments is bevelled at the join point. The triangular region of the corner is enclosed by a straight line between the outer corners of each stroke.
	Round,     ///< The outer corner of the joined path segments is rounded. The circular region is centered at the join point.
	Miter      ///< The outer corner of the joined path segments is spiked. The spike is created by extension beyond the join point of the outer edges of the stroke until they intersect. In case the extension goes beyond the limit, the join style is converted to the Bevel style.
};

struct VGPrimitive
{
	static constexpr int MAX_STOP_COUNT = 16;
	struct fill_t
	{
		float Color[4];
		uint32_t Flags;
		uint32_t Image;
		uint32_t Linear;
		uint32_t Radial;
	};
	struct stroke_t
	{
		float Color[4];
		uint32_t Flags;
		uint32_t Image;
		uint32_t Linear;
		uint32_t Radial;
	};
	struct linear_t
	{
		VGFloat4  NumStops;
		VGFloat2  GradStartPos;
		VGFloat2  GradEndPos;
		VGFloat4  StopPoints[MAX_STOP_COUNT / 4];
		VGFloat4  StopColors[MAX_STOP_COUNT];
	};
	struct radial_t
	{
		VGFloat4  NumStops;
		VGFloat2  CenterPos;
		VGFloat2  Radius;
		VGFloat4  StopPoints[MAX_STOP_COUNT / 4];
		VGFloat4  StopColors[MAX_STOP_COUNT];
	};
	struct primitive_t
	{
		float X = 0, Y = 0;
		uint32_t Fill = -1, Stroke = -1;
	};

	VGVector<fill_t> FillStyle;
	VGVector<stroke_t> StrokeStyle;
	VGVector<primitive_t> Primitive;
	VGVector<linear_t> LinearGradient;
	VGVector<radial_t> RadialGradient;
	VGVector<void*> TextureList;
};
using VGPrimitiveRef = VGRef<VGPrimitive>;
using VGPrimitiveRaw = VGRaw<VGPrimitive>;