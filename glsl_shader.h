#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>

class GLSLShader
{
  public:
    unsigned int ID{};
    void LoadShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        std::ifstream gShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // if geometry shader path is present, also load a geometry shader
            if (geometryPath != nullptr)
            {
                gShaderFile.open(geometryPath);
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        LoadShaderFromString(vertexCode, fragmentCode, geometryCode);
    }
    void LoadShaderFromString(std::string vertexCode, std::string fragmentCode, std::string geometryCode)
    {
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // if geometry shader is given, compile geometry shader
        unsigned int geometry;
        if (!geometryCode.empty())
        {
            const char *gShaderCode = geometryCode.c_str();
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            checkCompileErrors(geometry, "GEOMETRY");
        }
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        if (!geometryCode.empty())
            glAttachShader(ID, geometry);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (!geometryCode.empty())
            glDeleteShader(geometry);
    }

    GLSLShader() = default;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    GLSLShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr)
    {
        LoadShader(vertexPath, fragmentPath, geometryPath);
    }

    // activate the shader
    // ------------------------------------------------------------------------
    void activate() const
    {
        glUseProgram(ID);
    }
    static void deactivate()
    {
        glUseProgram(0);
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string &name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void BuildInShader()
    {
        std::string vs = R"(
		#version 410 core
		layout(location = 0) in vec3 _position;
		layout(location = 1) in vec3 _normal;
		layout(location = 2) in vec3 _color;
		uniform int triangle_num_per_circle;
		uniform float circle_radius;

        out COLOR_OUT
        {
		    vec3 vertColor;
        }color_out;
		void main()
		{
			gl_Position = vec4(_position, 1.0);
            color_out.vertColor = _color;
		}
	    )";


		std::string gs = R"(
		#version 410 core
        layout(points) in ;
        layout(triangle_strip, max_vertices = 146) out; // 注意输出类型
        uniform int triangle_num_per_circle;
		uniform float circle_radius;

        in COLOR_OUT
        {
            vec3 vertColor;
        }color_in[];

        out vec3 vert_color;
        void Draw_TopCircle_Plane()
        {
            int fragments = triangle_num_per_circle;
            float Radius = circle_radius;
			vec4 centerP = gl_in[0].gl_Position;
		    int i;
			float Pi=3.14;
			gl_Position = centerP;
		    EmitVertex();
			float du = 2.0f * Pi / fragments;
		    for (i = 0; i < fragments; i++)
		    {
				vec4 vertex_1 = vec4(centerP.x+Radius*cos(du*i),centerP.y+Radius*sin(du*i),0.0,1.0);
				gl_Position = vertex_1;
				EmitVertex();
				gl_Position = centerP;
				EmitVertex();
		    }
			vec4 vertex_2 = vec4(centerP.x+Radius*cos(du*(fragments)),centerP.y+sin(du*(fragments)),0.0,1.0);
			gl_Position = vertex_2;
			EmitVertex();
			vec4 vertex_3 = vec4(centerP.x+Radius,centerP.y,0.0,1.0);
			gl_Position = vertex_3;
			EmitVertex();
			EndPrimitive();
        }

		void main()
		{

            vert_color = color_in[0].vertColor;
            Draw_TopCircle_Plane();
//            int tri_num = triangle_num_per_circle;
//            float radius = circle_radius;
//            float per_degree = 2*3.1415926/tri_num;
//            vec4 last_point = gl_in[0].gl_Position + vec4(radius, 0.0, 0.0,1.0);
//            float cur_degree;
//            vec4 cur_point;
//            int i = 1;
//            for(;i<tri_num;i++)
//            {
//                cur_degree = i*per_degree;
//                cur_point = gl_in[0].gl_Position + vec4(radius*cos(cur_degree),radius*sin(cur_degree),0,1);
//
//                gl_Position = last_point;
//                EmitVertex();
//                gl_Position = gl_in[0].gl_Position;
//                EmitVertex();
//                gl_Position = cur_point;
//                EmitVertex();
//                last_point = cur_point;
//		    }
//            gl_Position = cur_point;
//		    EmitVertex();
//            gl_Position = gl_in[0].gl_Position;
//            EmitVertex();
//            gl_Position = gl_in[0].gl_Position + vec4(radius, 0.0, 0.0,1.0);
//                EmitVertex();
//
//            EndPrimitive();
//
//            gl_PointSize = gl_in[0].gl_PointSize;


		}
	    )";

        std::string fs = R"(
		#version 410 core
        uniform int triangle_num_per_circle;
		uniform float circle_radius;
		in vec3 vert_color;

		out vec4 color;



		void main()
		{
            color = vec4(vert_color,1);
            //color = vec4(1,0,0,1);
		}
        )";

        LoadShaderFromString(vs, fs, gs);
    }

  private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

#endif
