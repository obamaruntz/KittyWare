#pragma once
#include <sdk/other/vector.h>
#include <cache.h>
#include <d3dx9.h>

float M_PI = 3.14159265358979323846;

struct view_matrix_t
{
	float* operator[ ](int index)
	{
		return matrix[index];
	}
	float matrix[4][4];
};

namespace calc
{
	Vector3 get_forward_vector(const Rotation& rot) {
		float length = std::sqrt(rot.yaw * rot.yaw + rot.pitch * rot.pitch + rot.roll * rot.roll);
		if (length != 0) {
			return { rot.yaw / length, rot.pitch / length, rot.roll / length };
		}
		return { 0, 0, 0 };
	}

	bool is_looking_at_you(const Vector3& pos, const Vector3& other_pos, const Rotation& other_rot) {
		Vector3 direction_to_you = (pos - other_pos).normalize();
		Vector3 other_forward = get_forward_vector(other_rot);

		float dot_product = direction_to_you.dot(other_forward);

		const float threshold = 0.9f;
		return dot_product > threshold;
	}

	constexpr float DEG2RAD(float degrees) {
		return degrees * static_cast<float>(M_PI) / 180.0f;
	}

	Vector3 getDirectionVector(const Rotation& rotation) {
		float yaw = rotation.yaw;
		float pitch = rotation.pitch;
		float roll = rotation.roll; // Roll is usually not used in basic FPS camera systems

		// Calculate the direction vector
		Vector3 direction;
		direction.x = cos(pitch) * cos(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * sin(yaw);

		return direction;
	}

	// Define a line from the player's position in the direction they are looking
	void getLookLine(const Vector3& position, const Rotation& rotation, Vector3& lineEnd, float length = 100.0f) {

		// Calculate the direction vector
		Vector3 direction = get_forward_vector(rotation);

		// Calculate the end point of the line
		lineEnd.x = position.x + direction.x * length;
		lineEnd.y = position.y + direction.y * length;
		lineEnd.z = position.z + direction.z * length;
	}

	float distance(const Vector2& v1, const Vector2& v2) {
		float dx = v2.x - v1.x;
		float dy = v2.y - v1.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	float distance_v3(const Vector3& point1, const Vector3& point2) {
		return sqrt((point1.x - point2.x) * (point1.x - point2.x) +
			(point1.y - point2.y) * (point1.y - point2.y) +
			(point1.z - point2.z) * (point1.z - point2.z));
	}

	static Vector2 world_to_screen(Vector3 pos) {
		D3DXMATRIX viewmatrix = Cache::view_matrix;

		D3DXVECTOR3 world_pos = D3DXVECTOR3(pos.x, pos.y, pos.z);

		D3DXMatrixTranspose(&viewmatrix, &viewmatrix);
		auto vec_x = D3DXVECTOR4(viewmatrix._21, viewmatrix._22, viewmatrix._23, viewmatrix._24),
			vec_y = D3DXVECTOR4(viewmatrix._31, viewmatrix._32, viewmatrix._33, viewmatrix._34),
			vec_z = D3DXVECTOR4(viewmatrix._41, viewmatrix._42, viewmatrix._43, viewmatrix._44);

		auto screen_pos = D3DXVECTOR3((vec_x.x * world_pos.x) + (vec_x.y * world_pos.y) + (vec_x.z * world_pos.z) + vec_x.w,
			(vec_y.x * world_pos.x) + (vec_y.y * world_pos.y) + (vec_y.z * world_pos.z) + vec_y.w,
			(vec_z.x * world_pos.x) + (vec_z.y * world_pos.y) + (vec_z.z * world_pos.z) + vec_z.w);

		if (screen_pos.z <= 0.1f)
			return { -1, -1 };

		screen_pos.z = 1.0f / screen_pos.z;
		screen_pos.x *= screen_pos.z;
		screen_pos.y *= screen_pos.z;

		auto width = GetSystemMetrics(SM_CXSCREEN);
		auto height = GetSystemMetrics(SM_CYSCREEN);
		float x_temp = width / 2;
		float y_temp = height / 2;

		screen_pos.x += x_temp + float(0.5f * screen_pos.x * width + 0.5f);
		screen_pos.y = y_temp - float(0.5f * screen_pos.y * height + 0.5f);
		return { screen_pos.x, screen_pos.y };
	}

	static Vector2 calc_angles(Vector3 from, Vector3 destination)
	{
		float yaw;
		float pitch;

		float deltaX = destination.x - from.x;
		float deltaY = destination.y - from.y;
		yaw = (float)(std::atan2(deltaY, deltaX) * 180 / 3.14159265358979323846);

		float deltaZ = destination.z - from.z;
		double distance = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));
		pitch = (float)(std::atan2(deltaZ, distance) * 180 / 3.14159265358979323846);

		Vector2 finished = { yaw, pitch };

		return finished;
	}
}