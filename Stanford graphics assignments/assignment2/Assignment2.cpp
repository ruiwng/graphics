#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <fstream>
#include <sstream>

namespace
{
const int SHADER_ERROR_LOG_SIZE = 500;
bool VerifyShaderCompile(GLuint newShaderObject)
{
    GLint compileStatus;
    OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}

bool VerifyProgramLink(GLuint shaderProgram)
{
    GLint linkStatus;
    OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
    if (linkStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera)), vao(0), shaderProgramId(0)
{
    vertexPositions = 
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.f},
            {-1.f, 1.f, 0.f, 1.f},
            {-1.f, 0.f, 0.f, 1.f}
        });
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

std::string Assignment2::ReadFile(const char* fileName) {
	std::ifstream ifs(fileName);
	return std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

void Assignment2::SetupExample1()
{
    // Insert "Load and Compile Shaders" code here.

    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.
    /*const GLuint vertexShaderId = 0;
    const GLuint fragmentShaderId = 0;
    const GLuint shaderProgramId = 0;*/
	GLuint vertexShaderId = OGL_CALL(glCreateShader(GL_VERTEX_SHADER));
	GLuint fragmentShaderId = OGL_CALL(glCreateShader(GL_FRAGMENT_SHADER));
	shaderProgramId = OGL_CALL(glCreateProgram());
	
	const std::string vertexShaderName = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
	std::string vertexShaderSource = ReadFile(vertexShaderName.c_str());
	const char* vertexSource = vertexShaderSource.c_str();
	OGL_CALL(glShaderSource(vertexShaderId, 1, &vertexSource, NULL));
	OGL_CALL(glCompileShader(vertexShaderId));

	const std::string fragmentShaderName = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
	std::string fragmentShaderSource = ReadFile(fragmentShaderName.c_str());
	const char* fragmentSource = fragmentShaderSource.c_str();
	OGL_CALL(glShaderSource(fragmentShaderId, 1, &fragmentSource, NULL));
	OGL_CALL(glCompileShader(fragmentShaderId));

	OGL_CALL(glAttachShader(shaderProgramId, vertexShaderId));
	OGL_CALL(glAttachShader(shaderProgramId, fragmentShaderId));
	OGL_CALL(glLinkProgram(shaderProgramId));

    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }

    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION

    // Insert "Setup Buffers" code here.
	OGL_CALL(glGenVertexArrays(1, &vao));
	OGL_CALL(glBindVertexArray(vao));
	GLuint vbo;
	OGL_CALL(glGenBuffers(1, &vbo));
	OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	OGL_CALL(glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * 4 * sizeof(float), &vertexPositions[0], GL_STATIC_DRAW));
	OGL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	OGL_CALL(glEnableVertexAttribArray(0));
	
	OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	OGL_CALL(glBindVertexArray(0));
	OGL_CALL(glDeleteBuffers(1, &vbo));
}

void Assignment2::Tick(double deltaTime)
{
	static float currentTime = 0.0f;
	OGL_CALL(glBindVertexArray(vao));
	OGL_CALL(glUseProgram(shaderProgramId));
	GLint location = OGL_CALL(glGetUniformLocation(shaderProgramId, "inputTime"));
	currentTime += deltaTime;
	OGL_CALL(glUniform1f(location, currentTime));
	OGL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	OGL_CALL(glBindVertexArray(0));
	OGL_CALL(glUseProgram(0));
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.
}
