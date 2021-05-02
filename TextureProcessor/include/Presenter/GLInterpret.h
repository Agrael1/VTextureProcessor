#pragma once
#include <optional>

class Engine
{
    struct Context
    {
        Context()
        {
            QSurfaceFormat surfaceFormat;
            surfaceFormat.setMajorVersion(4);
            surfaceFormat.setMinorVersion(3);

            context.setFormat(surfaceFormat);
            context.create();

            surface.setFormat(surfaceFormat);
            surface.create();
            if (!surface.isValid()) qDebug("Unable to create the Offscreen surface");

            context.makeCurrent(&surface);
            funcs.initializeOpenGLFunctions();
        }
        auto Format()
        {
            QOpenGLFramebufferObjectFormat fboFormat;
            fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            return fboFormat;
        }
        QOffscreenSurface surface;
        QOpenGLContext context;
        QOpenGLPaintDevice device;
        QOpenGLFunctions funcs;
    };
public:
    Engine(QSize size)
        :vs(QOpenGLShader::Vertex), frame(size, con.Format()), vlay(QOpenGLBuffer::VertexBuffer)
    {
        con.funcs.glViewport(0, 0, size.width(), size.height());
        frame.bind();

        vs.compileSourceCode(vertexShaderSource);
        shaders.addShader(&vs);

        vlay.create();
        vlay.setUsagePattern(QOpenGLBuffer::StaticDraw);
        vlay.bind();
        vlay.allocate(vertexPositions, 6 * sizeof(float));
        vbuf.create();
        vbuf.bind();

        vlay.bind();


        shaders.addShaderFromSourceCode(QOpenGLShader::Fragment,
            "#version 330\r\n"
            "in vec3 fragColor;\n"
            "out vec4 color;\n"
            "void main() {\n"
            "    color = vec4(fragColor, 1.0);\n"
            "}\n"
        );
    }
public:
	auto Render(QOpenGLShader& ps)
	{
        shaders.link();
        shaders.bind();

        shaders.enableAttributeArray("position");
        shaders.setAttributeBuffer("position", GL_FLOAT, 0, 2);

        con.funcs.glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
        con.funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        con.funcs.glDrawArrays(GL_TRIANGLES, 0, 3);
        con.funcs.glFinish();

        return frame.toImage();
	}
    void Remove()
    {
        vbuf.destroy();
    }
private:
    Context con;
    QOpenGLFramebufferObject frame;

    QOpenGLBuffer vlay;
    QOpenGLVertexArrayObject vbuf;

    QOpenGLShader vs;
    QOpenGLShaderProgram shaders;

    static constexpr const char* vertexShaderSource =
    "#version 330\r\n"
        "in vec2 position;\n"
        "out vec3 fragColor;\n"
        "void main() {\n"
        "    fragColor = vec3(1.0, 0.0, 0.0);\n"
        "    gl_Position = vec4(position, 0.0, 1.0);\n"
        "}\n";

    //static constexpr const char* vertexShaderSource =
    // R"(#version 330 core
    //    out vec2 texcoords;
    //    void main() {
    //        vec2 vertices[3] = vec2[3](vec2(-1, -1), vec2(3, -1), vec2(-1, 3));
    //        gl_Position = vec4(vertices[gl_VertexID], 0, 1);
    //        texcoords = 0.5 * gl_Position.xy + vec2(0.5);
    //    })";
    static constexpr const float vertexPositions[] = {
        -1.0f, -1.0f,
        -0.0f,  1.0f,
         1.0f, -1.0f,
    };

    static constexpr const float vertexColors[] = {
       1.0f, 0.0f, 0.0f,
       0.0f, 1.0f, 0.0f,
       0.0f, 0.0f, 1.0f
    };
};