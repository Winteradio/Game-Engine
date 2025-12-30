#ifndef __WTR_MATHTYPES_H__
#define __WTR_MATHTYPES_H__

#define _USING_GLM_
#ifdef _USING_GLM_

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#endif

namespace wtr
{
#define _USING_GLM_
#ifdef _USING_GLM_

	using F32QUAT = glm::f32quat;
	using F64QUAT = glm::f64quat;

	using fvec2 = glm::f32vec2;
	using fvec3 = glm::f32vec3;
	using fvec4 = glm::f32vec4;

	using fmat2 = glm::f32mat2;
	using fmat3 = glm::f32mat3;
	using fmat4 = glm::f32mat4;

	using dvec2 = glm::dvec2;
	using dvec3 = glm::dvec3;
	using dvec4 = glm::dvec4;

	using dmat2 = glm::dmat2;
	using dmat3 = glm::dmat3;
	using dmat4 = glm::dmat4;

	using u8vec2 = glm::u8vec2;
	using u8vec3 = glm::u8vec3;
	using u8vec4 = glm::u8vec4;

	using u8mat2 = glm::u8mat2x2;
	using u8mat3 = glm::u8mat3x3;
	using u8mat4 = glm::u8mat4x4;

	using u16vec2 = glm::u16vec2;
	using u16vec3 = glm::u16vec3;
	using u16vec4 = glm::u16vec4;

	using u16mat2 = glm::u16mat2x2;
	using u16mat3 = glm::u16mat3x3;
	using u16mat4 = glm::u16mat4x4;

	using u32vec2 = glm::u32vec2;
	using u32vec3 = glm::u32vec3;
	using u32vec4 = glm::u32vec4;

	using u32mat2 = glm::u32mat2x2;
	using u32mat3 = glm::u32mat3x3;
	using u32mat4 = glm::u32mat4x4;

	using u64mat2 = glm::u64mat2x2;
	using u64mat3 = glm::u64mat3x3;
	using u64mat4 = glm::u64mat4x4;

	using u64vec2 = glm::u64vec2;
	using u64vec3 = glm::u64vec3;
	using u64vec4 = glm::u64vec4;

	using i8vec2 = glm::i8vec2;
	using i8vec3 = glm::i8vec3;
	using i8vec4 = glm::i8vec4;

	using i8mat2 = glm::i8mat2x2;
	using i8mat3 = glm::i8mat3x3;
	using i8mat4 = glm::i8mat4x4;

	using i16vec2 = glm::i16vec2;
	using i16vec3 = glm::i16vec3;
	using i16vec4 = glm::i16vec4;

	using i16mat2 = glm::i16mat2x2;
	using i16mat3 = glm::i16mat3x3;
	using i16mat4 = glm::i16mat4x4;

	using i32vec2 = glm::i32vec2;
	using i32vec3 = glm::i32vec3;
	using i32vec4 = glm::i32vec4;

	using i32mat2 = glm::i32mat2x2;
	using i32mat3 = glm::i32mat3x3;
	using i32mat4 = glm::i32mat4x4;

	using i64vec2 = glm::i64vec2;
	using i64vec3 = glm::i64vec3;
	using i64vec4 = glm::i64vec4;

	using i64mat2 = glm::i64mat2x2;
	using i64mat3 = glm::i64mat3x3;
	using i64mat4 = glm::i64mat4x4;
#endif //
};

#endif // __WTR_MATHTYPES_H__