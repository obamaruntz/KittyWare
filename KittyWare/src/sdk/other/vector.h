#pragma once
#include <numbers>
#include <cmath>

struct Rotation {
	float yaw, pitch, roll;
};

struct iVector2
{
	int x = {}, y = {};

	constexpr bool is_zero() const noexcept {
		return x == 0 && y == 0;
	}

	constexpr const iVector2& operator-(const iVector2& other) const noexcept
	{
		return iVector2{ x - other.x, y - other.y };
	}

	void print() const noexcept;
};

struct Vector2
{
	float x = {}, y = {};

	constexpr bool is_zero() const noexcept {
		return x == 0.f && y == 0.f;
	}
	constexpr const Vector2& operator-(const Vector2& other) const noexcept
	{
		return Vector2{ x - other.x, y - other.y };
	}


	void print() const noexcept;
};

struct Vector4
{
	float x = {}, y = {}; float z = {}, w = {};
};


struct Vector3
{
	constexpr Vector3(
		const float x = 0.f,
		const float y = 0.f,
		const float z = 0.f) noexcept :
		x(x), y(y), z(z) { }

	// operator overloads
	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y, z - other.z };
	}
	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}
	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const Vector3& other) const {
		return x != other.x || y != other.y || z != other.z;
	}

	Vector3& operator+=(const Vector3& other) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3 normalize() const {
		float length = std::sqrt(x * x + y * y + z * z);
		return { x / length, y / length, z / length };
	}

	float dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	float x, y, z;

	constexpr bool is_zero() const noexcept {
		return x == 0.f && y == 0.f && z == 0.f;
	}

	void print() const noexcept;

};
