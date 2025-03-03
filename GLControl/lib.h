#pragma once

#include <filesystem>
#include <thread>

#include <GLM/glm.hpp>

#include <glm/gtc/quaternion.hpp>

namespace lib
{
	template <typename  T>
	class Point2D
	{
	public:
		T x;
		T y;
	public:
		Point2D() : x(0), y(0) {}

		Point2D(T x_, T y_) : x(x_), y(y_) {}
	};

	template <typename  T>
	class Point3D
	{
	public:
		T x;
		T y;
		T z;
	public:
		Point3D() : x(0), y(0), z(0) {}

		Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}
	};

	using iPoint2D = Point2D<int>;
	using fPoint2D = Point2D<float>;
	using dPoint2D = Point2D<double>;

	using fPoint3D = Point3D<float>;

	using dPoint3D = glm::f32vec3;

	using iPoint3D = glm::i32vec3;


	using Matrix4 = glm::mat4x4;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using Quat = glm::quat;

	static Quat makeQuat(float fAngle_, lib::Vector3 vDirection_)
	{
		float fHafAngle = glm::radians(fAngle_ / 2);
		float fSin = sin(fHafAngle);
		float fCos = cos(fHafAngle);

		return glm::normalize(lib::Quat(fCos, fSin * vDirection_.x, fSin * vDirection_.y, fSin * vDirection_.z));
	}

	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}


	std::vector<std::string> create_file_list(const char* sPath_);


	void unpackColor(lib::iPoint3D vColor256_, lib::fPoint3D& vColorRGB_);
	void unpackColor(unsigned nColor256_, lib::fPoint3D& vColorRGB_);

	//------------------------------------------------------------------------------------------

	template<typename T>
	bool isZero(T value, T epsilon = 0.00001)
	{
		return std::abs(value) < epsilon;
	}

	template<typename T>
	void limit(T& value, T min, T max)
	{
		value = std::min<T>(std::max<T>(value, min), max);
	}

	template<typename T>
	bool between(T value, T min, T max)
	{
		return value <= max && value >= min;
	}

	inline unsigned getCoreCount()
	{
		return std::thread::hardware_concurrency();
	}

}
