#include "OpenVGRender.h"
#include <GL/glew.h>

#define OPENVG_MAX_TRIANGLE 10240

OpenVGRender::OpenVGRender()
{
	auto common = R"(
		#define MAX_STOP_COUNT 16
		struct fill_t
		{
			vec4 Color;
			int Flags;
			int Image;
			int Linear;
			int Radial;
		};
		struct stroke_t
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
		struct primitive_t
		{
			float X, Y;
			uint Fill, Stroke;
		};

		layout(std430, binding=0) buffer FILL_STYLE_BLOCK
		{
			fill_t FillList[];
		};

		layout(std430, binding=1) buffer STROKE_STYLE_BLOCK
		{
			stroke_t StrokeList[];
		};

		layout(std430, binding=2) buffer LINEAR_GRADIENT_BLOCK
		{
			linear_t LinearList[];
		};

		layout(std430, binding=3) buffer RADIAL_GRADIENT_BLOCK
		{
			radial_t RadialList[];
		};

		layout(std430, binding=4) buffer TRANFORM_MATRIX_BLOCK
		{
			mat3 MatrixList[];
		};

		layout (binding = 0) uniform sampler2D TextureList[16];

		uniform vec2 Viewport;
	)";

	auto vsource = VGString(R"(
		#version 450
		layout (location = 0) in vec2 _point;
		layout (location = 1) in ivec2 _style;
		layout (location = 2) in ivec2 _matrix;
		out vec2 uv;
		flat out int fillStyle;
		flat out int strokeStyle;
		)") + common + R"(
		void main()
		{
			fillStyle = _style.x;
			strokeStyle = _style.y;
			mat3 matrix = MatrixList[_matrix.x];
			vec2 point = vec2(matrix * vec3(_point, 1.0));
			uv = vec2(point.x * Viewport.x, 1.0-point.y * Viewport.y);
			gl_Position = vec4(2*point*Viewport - 1, 0.0, 1.0);
		}
	)";

	auto fsource = VGString(R"(
		#version 450
		in vec2 uv;
		flat in int fillStyle;
		flat in int strokeStyle;
		layout (location = 0) out vec4 color;
		)") + common + R"(
		void main()
		{
			if(fillStyle != -1) color = FillList[fillStyle].Color;
			else color = vec4(0, 0, 0, 1);
		}
	)";

	// 检查编译错误 
	auto vshader = glCreateShader(GL_VERTEX_SHADER);
	auto source = vsource.c_str();
	printf("%s\n", source);
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
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(primitive_t) * 3 * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeBuffer = vbo;

	// 4. 设置顶点属性指针 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(primitive_t), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribIPointer(1, 2, GL_INT, sizeof(primitive_t), (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_NativePrimitive = vao;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::fill_t) * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeFillBuffer = vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VGPrimitive::stroke_t) * OPENVG_MAX_TRIANGLE, nullptr, GL_DYNAMIC_DRAW);

	m_NativeStrokeBuffer = vbo;

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
	glDeleteBuffers(1, &m_NativeBuffer); m_NativeBuffer = 0;
	glDeleteBuffers(1, &m_NativeFillBuffer); m_NativeFillBuffer = 0;
	glDeleteBuffers(1, &m_NativeStrokeBuffer); m_NativeStrokeBuffer = 0;
	glDeleteBuffers(1, &m_NativeLinearBuffer); m_NativeLinearBuffer = 0;
	glDeleteBuffers(1, &m_NativeRadialBuffer); m_NativeRadialBuffer = 0;
	glDeleteBuffers(1, &m_NativeMatrixBuffer); m_NativeMatrixBuffer = 0;
	glDeleteVertexArrays(1, &m_NativePrimitive); m_NativePrimitive = 0;
}

void OpenVGRender::render(VGRect client, VGArrayView<const VGPrimitive> data)
{
	m_FillList.clear();
	m_StrokeList.clear();
	m_TextureList.clear();
	m_LinearList.clear();
	m_RadialList.clear();
	m_PrimitiveList.clear();
	m_MatrixList.clear();

	for (size_t i = 0, k = 0; i < data.size(); ++i)
	{
		k += data[i].Primitive.size();
		if (i + 1 == data.size()) m_PrimitiveList.resize(k);
	}

	for (size_t i = 0, p = 0; i < data.size(); ++i)
	{
		auto fillIndex = m_FillList.size();
		auto strokeIndex = m_StrokeList.size();
		auto pointIndex = p;
		auto imageIndex = m_TextureList.size();
		auto linearIndex = m_LinearList.size();
		auto radialIndex = m_RadialList.size();
		auto matrixIndex = m_MatrixList.size();
		auto& points = data[i].Primitive;
		auto& fills = data[i].FillStyle;
		auto& strokes = data[i].StrokeStyle;
		auto& linears = data[i].LinearGradient;
		auto& radials = data[i].RadialGradient;
		auto& matrixs = data[i].MatrixList;

		::memcpy(m_PrimitiveList.data() + pointIndex, points.data(), sizeof(primitive_t) * points.size());
		for (size_t k = 0; k < points.size(); ++k)
		{
			auto& point = m_PrimitiveList[pointIndex + k];
			if (point.Fill != -1) point.Fill += fillIndex;
			if (point.Stroke != -1) point.Stroke += strokeIndex;
			if (point.Matrix != -1) point.Matrix += matrixIndex;
		}
		for (size_t k = 0; k < fills.size(); ++k)
		{
			auto& fill = m_FillList.emplace_back(fills[k]);
			if (fill.Image != -1) fill.Image += imageIndex;
			if (fill.Linear != -1) fill.Linear += linearIndex;
			if (fill.Radial != -1) fill.Radial += radialIndex;
		}
		for (size_t k = 0; k < strokes.size(); ++k)
		{
			auto& stroke = m_StrokeList.emplace_back(strokes[k]);
			if (stroke.Image != -1) stroke.Image += imageIndex;
			if (stroke.Linear != -1) stroke.Linear += linearIndex;
			if (stroke.Radial != -1) stroke.Radial += radialIndex;
		}
		m_LinearList.insert(m_LinearList.end(), linears.begin(), linears.end());
		m_RadialList.insert(m_RadialList.end(), radials.begin(), radials.end());
		m_MatrixList.insert(m_MatrixList.end(), matrixs.begin(), matrixs.end());

		p += points.size();
	}

	// 更新顶点缓冲区
	glBindBuffer(GL_ARRAY_BUFFER, m_NativeBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(primitive_t) * m_PrimitiveList.size(), m_PrimitiveList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_NativeFillBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::fill_t) * m_FillList.size(), m_FillList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_NativeStrokeBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::stroke_t) * m_StrokeList.size(), m_StrokeList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_NativeLinearBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::linear_t) * m_LinearList.size(), m_LinearList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_NativeRadialBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::radial_t) * m_RadialList.size(), m_RadialList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_NativeMatrixBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VGPrimitive::matrix_t) * m_MatrixList.size(), m_MatrixList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 渲染控件视图
	glUseProgram(m_NativeProgram);
	glBindVertexArray(m_NativePrimitive);
	GLint viewportSizeLoc = glGetUniformLocation(m_NativeProgram, "Viewport");
	glUniform2f(viewportSizeLoc, 1.0f / client.W, 1.0f / client.H);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_NativeFillBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_NativeStrokeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_NativeLinearBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_NativeRadialBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_NativeMatrixBuffer);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, m_PrimitiveList.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);
	glUseProgram(0);
}