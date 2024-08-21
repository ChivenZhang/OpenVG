#include "OpenVGRender.h"
#include <GL/glew.h>

OpenVGRender::OpenVGRender()
{
	auto common = R"(
		#define MAX_STOP_COUNT 16
		struct fill_t
		{
			float Color[4];
			uint Flags;
			uint Image;
			uint Linear;
			uint Radial;
		};
		struct stroke_t
		{
			float Color[4];
			uint Flags;
			uint Image;
			uint Linear;
			uint Radial;
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
			uint Fill = -1, Stroke = -1;
		};

		layout(binding=0) buffer FILL_STYLE_BLOCK
		{
			fill_t FillList[];
		};

		layout(binding=1) buffer STROKE_STYLE_BLOCK
		{
			stroke_t StrokeList[];
		};

		layout(binding=0) sampler2D TextureList[16];
	)";
	auto vsource = R"(
		#version 450 core
		layout (location = 0) in vec2 _point;
		layout (location = 1) in uvec2 _style;
		out vec2 uv;
		flat out uint fillStyle;
		flat out uint strokeStyle;

		void main()
		{
			fillStyle = _style.x;
			strokeStyle = _style.y;
			uv = vec2(_point.x, 1.0-_point.y);
			gl_Position = vec4(2*_point-1, 0.0, 1.0);
		}
	)";

	auto fsource = R"(
		#version 450 core
		in vec2 uv;
		flat in uint fillStyle;
		flat in uint strokeStyle;
		layout (location = 0) out vec4 color;
		layout (binding = 0) uniform sampler2D textureList[16];

		void main()
		{
			color = vec4(1,1,0,1);
		}
	)";

	// 检查编译错误 
	auto vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, &vsource, NULL);
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
	glShaderSource(fshader, 1, &fsource, NULL);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(primitive_t) * 1024, nullptr, GL_DYNAMIC_DRAW);

	m_NativeBuffer = vbo;

	// 4. 设置顶点属性指针 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(primitive_t), (void*)offsetof(primitive_t, X));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, sizeof(primitive_t), (void*)offsetof(primitive_t, Fill));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	m_NativePrimitive = vao;
}

OpenVGRender::~OpenVGRender()
{
	glDeleteBuffers(1, &m_NativeBuffer); m_NativeBuffer = 0;
	glDeleteVertexArrays(1, &m_NativePrimitive); m_NativePrimitive = 0;
	glDeleteProgram(m_NativeProgram); m_NativeProgram = 0;
}

void OpenVGRender::render(VGRect client, VGArrayView<const VGPrimitive> data)
{
	glUseProgram(m_NativeProgram);
	glBindVertexArray(m_NativePrimitive);

	m_PrimitiveList.clear();
	for (size_t i = 0; i < data.size(); ++i)
	{
		m_PrimitiveList.insert(m_PrimitiveList.end(), data[i].Primitive.begin(), data[i].Primitive.end());
	}

	// 更新顶点缓冲区
	glBindBuffer(GL_ARRAY_BUFFER, m_NativeBuffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(primitive_t) * m_PrimitiveList.size(), m_PrimitiveList.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// 渲染控件视图
	glDrawArrays(GL_TRIANGLES, 0, m_PrimitiveList.size());

	glBindVertexArray(0);
	glUseProgram(0);
}

uint32_t OpenVGRender::getTexture() const
{
	return 0;
}

void OpenVGRender::setTexture(uint32_t value)
{
}
