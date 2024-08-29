#include "OpenVGRender.h"
#include <GL/glew.h>

#define OPENVG_MAX_TRIANGLE 1024000

OpenVGRender::OpenVGRender()
{
	auto common = R"(
		#define MAX_STOP_COUNT 16
		#define VG_FLAGS_FILL_STROKE 0x0001
		#define VG_FLAGS_COLOR_IMAGE 0x0002
		#define VG_FLAGS_STYLE_LINEAR 0x0004
		#define VG_FLAGS_STYLE_RADIAL 0x0008
		struct vertex_t
		{
			float X, Y, U, V;
			int Style, Matrix;
		};
		struct style_t
		{
			vec4 Color;
			int Flags;
			int Image;
			int Linear;
			int Radial;
		};
		struct linear_t
		{
			vec4  NumStops;
			vec2  GradStartPos;
			vec2  GradEndPos;
			vec4  StopPoints[MAX_STOP_COUNT / 4];
			vec4  StopColors[MAX_STOP_COUNT];
		};
		struct radial_t
		{
			vec4  NumStops;
			vec2  CenterPos;
			vec2  Radius;
			vec4  StopPoints[MAX_STOP_COUNT / 4];
			vec4  StopColors[MAX_STOP_COUNT];
		};

		layout(std430, binding=0) buffer STYLE_BLOCK
		{
			style_t StyleList[];
		};

		layout(std430, binding=1) buffer LINEAR_BLOCK
		{
			linear_t LinearList[];
		};

		layout(std430, binding=2) buffer RADIAL_BLOCK
		{
			radial_t RadialList[];
		};

		layout(std430, binding=3) buffer MATRIX_BLOCK
		{
			mat3 MatrixList[];
		};

		layout (binding = 0) uniform sampler2D TextureList[16];

		uniform vec2 Viewport;
	)";

	auto vsource = VGString(R"(
		#version 450
		layout (location = 0) in vec2 _point;
		layout (location = 1) in vec2 _uv;
		layout (location = 2) in int _style;
		layout (location = 3) in int _matrix;
		out vec2 uv;
		flat out int style;
		)") + common + R"(
		void main()
		{
			style = _style;
			mat3 matrix = MatrixList[_matrix];
			vec2 vertex = vec2(matrix * vec3(_point, 1.0)) * Viewport;
			uv = vec2(vertex.x, 1.0 - vertex.y);
			gl_Position = vec4(2 * vertex - 1, 0.0, 1.0);
		}
	)";

	auto fsource = VGString(R"(
		#version 450
		in vec2 uv;
		flat in int style;
		layout (location = 0) out vec4 color;
		)") + common + R"(
		void main()
		{
			color = StyleList[style].Color;
			int flags = StyleList[style].Flags;
			if((flags & VG_FLAGS_FILL_STROKE) == 0)
			{
				if((flags & VG_FLAGS_COLOR_IMAGE) == 1)
					color *= texture(TextureList[StyleList[style].Image], uv);
			}
			else
			{
				if((flags & VG_FLAGS_COLOR_IMAGE) == 1)
					color *= texture(TextureList[StyleList[style].Image], uv);
			}
		}
	)";

	// 检查编译错误 
	auto vshader = glCreateShader(GL_VERTEX_SHADER);
	auto source = vsource.c_str();
	glShaderSource(vshader, 1, &source, NULL);
	glCompileShader(vshader);
	GLint success;
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(vshader, 512, NULL, infoLog);
		std::cerr << "Shader compilation failed: " << infoLog << std::endl;
		glDeleteShader(vshader); // 删除着色器，防止内存泄漏  
		::exit(-1);
	}

	// 检查编译错误 
	auto fshader = glCreateShader(GL_FRAGMENT_SHADER);
	source = fsource.c_str();
	glShaderSource(fshader, 1, &source, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(fshader, 512, NULL, infoLog);
		std::cerr << "Shader compilation failed: " << infoLog << std::endl;
		glDeleteShader(fshader); // 删除着色器，防止内存泄漏  
		::exit(-1);
	}

	// 检查链接错误  
	auto shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vshader);
	glAttachShader(shaderProgram, fshader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "Shader program linking failed: " << infoLog << std::endl;
		glDeleteProgram(shaderProgram); // 删除程序，防止内存泄漏  
		::exit(-1);
	}
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	m_NativeProgram = shaderProgram;

	// 3. 配置顶点缓冲区
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	m_NativePrimitive = vao;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::point_t) * 3 * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeVertexBuffer = vbo;

	// 4. 设置顶点属性指针 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VGPrimitive::point_t), (void*)offsetof(VGPrimitive::point_t, X));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VGPrimitive::point_t), (void*)offsetof(VGPrimitive::point_t, U));
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(2, 1, GL_INT, sizeof(VGPrimitive::point_t), (void*)offsetof(VGPrimitive::point_t, Style));
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(3, 1, GL_INT, sizeof(VGPrimitive::point_t), (void*)offsetof(VGPrimitive::point_t, Matrix));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::style_t) * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeStyleBuffer = vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::linear_t) * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeLinearBuffer = vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::radial_t) * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeRadialBuffer = vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::matrix_t) * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeMatrixBuffer = vbo;
}

OpenVGRender::~OpenVGRender()
{
	glDeleteProgram(m_NativeProgram); m_NativeProgram = 0;
	glDeleteBuffers(1, &m_NativeVertexBuffer); m_NativeVertexBuffer = 0;
	glDeleteBuffers(1, &m_NativeStyleBuffer); m_NativeStyleBuffer = 0;
	glDeleteBuffers(1, &m_NativeLinearBuffer); m_NativeLinearBuffer = 0;
	glDeleteBuffers(1, &m_NativeRadialBuffer); m_NativeRadialBuffer = 0;
	glDeleteBuffers(1, &m_NativeMatrixBuffer); m_NativeMatrixBuffer = 0;
	glDeleteVertexArrays(1, &m_NativePrimitive); m_NativePrimitive = 0;
}

void OpenVGRender::render(VGRect client, VGArrayView<const VGPrimitive> data)
{
	m_PointList.clear();
	m_StyleList.clear();
	m_LinearList.clear();
	m_RadialList.clear();
	m_MatrixList.clear();
	m_TextureList.clear();

	for (size_t i = 0, k = 0; i < data.size(); ++i)
	{
		k += data[i].PointList.size();
		if (i + 1 == data.size()) m_PointList.resize(std::max<size_t>(k, m_PointList.size()));
	}

	m_PrimitiveIndex = 0;
	for (size_t i = 0; i < data.size(); ++i)
	{
		auto pointIndex = m_PrimitiveIndex;
		auto styleIndex = m_StyleList.size();
		auto linearIndex = m_LinearList.size();
		auto radialIndex = m_RadialList.size();
		auto matrixIndex = m_MatrixList.size();
		auto imageIndex = m_TextureList.size();
		auto& points = data[i].PointList;
		auto& styles = data[i].StyleList;
		auto& linears = data[i].LinearList;
		auto& radials = data[i].RadialList;
		auto& matrixs = data[i].MatrixList;

		::memcpy(m_PointList.data() + pointIndex, points.data(), sizeof(VGPrimitive::point_t) * points.size());
		for (size_t k = 0; k < points.size(); ++k)
		{
			auto& point = m_PointList[pointIndex + k];
			if (point.Style != -1) point.Style += styleIndex;
			if (point.Matrix != -1) point.Matrix += matrixIndex;
		}
		for (size_t k = 0; k < styles.size(); ++k)
		{
			auto& style = m_StyleList.emplace_back(styles[k]);
			if (style.Image != -1) style.Image += imageIndex;
			if (style.Linear != -1) style.Linear += linearIndex;
			if (style.Radial != -1) style.Radial += radialIndex;
		}
		m_LinearList.insert(m_LinearList.end(), linears.begin(), linears.end());
		m_RadialList.insert(m_RadialList.end(), radials.begin(), radials.end());
		m_MatrixList.insert(m_MatrixList.end(), matrixs.begin(), matrixs.end());

		m_PrimitiveIndex += points.size();
	}
	if (m_PrimitiveIndex == 0) return;

	// 更新顶点缓冲区
	if (m_PointList.size())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_NativeVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::point_t) * m_PointList.size(), m_PointList.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_StyleList.size())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_NativeStyleBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::style_t) * m_StyleList.size(), m_StyleList.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_LinearList.size())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_NativeLinearBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::linear_t) * m_LinearList.size(), m_LinearList.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_RadialList.size())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_NativeRadialBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::radial_t) * m_RadialList.size(), m_RadialList.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (m_MatrixList.size())
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_NativeMatrixBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::matrix_t) * m_MatrixList.size(), m_MatrixList.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// 渲染控件视图
	glUseProgram(m_NativeProgram);
	glBindVertexArray(m_NativePrimitive);
	glUniform2f(glGetUniformLocation(m_NativeProgram, "Viewport"), 1.0f / client.W, 1.0f / client.H);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_NativeStyleBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_NativeLinearBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_NativeRadialBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_NativeMatrixBuffer);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, std::min<size_t>(m_PrimitiveIndex, 3 * OPENVG_MAX_TRIANGLE));
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
	glUseProgram(0);
}