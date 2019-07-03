#include "assignment4/Assignment4.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include "common/Utility/Texture/TextureLoader.h"
#include "common/Rendering/Shaders/EpicShader.h"

#include <cmath>

Assignment4::Assignment4(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
}

std::unique_ptr<Application> Assignment4::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment4>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment4::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(60.f, 1280.f / 720.f);
}

glm::vec2 Assignment4::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment4::SetupScene()
{
    SetupExample1();
}

void Assignment4::SetupCamera()
{
    camera->SetPosition(glm::vec3(-35.698f, 40.571f, -2.285f));
    camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -3.3f * PI / 5.f);
    camera->Rotate(glm::vec3(camera->GetRightDirection()), -PI / 8.f);
    camera->Translate(glm::vec3(camera->GetRightDirection()) * 10.f);
    camera->Translate(glm::vec3(camera->GetForwardDirection()) * -10.f);

    PerspectiveCamera* pcamera = static_cast<PerspectiveCamera*>(camera.get());
    pcamera->SetZFar(1000.f);
}

void Assignment4::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    if (state != SDL_KEYDOWN) {
        return;
    }

    switch (key.sym) {
    case SDLK_1:
        if (!repeat && state == SDL_KEYDOWN) {
            SetupExample1();
        }
        break;
    case SDLK_UP:
        camera->Rotate(glm::vec3(camera->GetRightDirection()), 0.1f);
        break;
    case SDLK_DOWN:
        camera->Rotate(glm::vec3(camera->GetRightDirection()), -0.1f);
        break;
    case SDLK_RIGHT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), -0.1f);
        break;
    case SDLK_LEFT:
        camera->Rotate(glm::vec3(SceneObject::GetWorldUp()), 0.1f);
        break;
    case SDLK_w:
        camera->Translate(glm::vec3(camera->GetForwardDirection() * 0.3f));
        break;
    case SDLK_a:
        camera->Translate(glm::vec3(camera->GetRightDirection() * -0.3f));
        break;
    case SDLK_s:
        camera->Translate(glm::vec3(camera->GetForwardDirection() * -0.3f));
        break;
    case SDLK_d:
        camera->Translate(glm::vec3(camera->GetRightDirection() * 0.3f));
        break;
    case SDLK_SPACE:
        camera->Translate(glm::vec3(camera->GetUpDirection() * 0.3f));
        break;
    case SDLK_LCTRL:
    case SDLK_RCTRL:
        camera->Translate(glm::vec3(camera->GetUpDirection() * -0.3f));
        break;
    default:
        Application::HandleInput(key, state, repeat, timestamp, deltaTime);
        break;
    }
}

void Assignment4::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
    std::static_pointer_cast<PerspectiveCamera>(camera)->SetAspectRatio(x / y);
}

void Assignment4::SetupExample1()
{
    scene->ClearScene();
    std::unordered_map<GLenum, std::string> shaderSpec = {
        { GL_VERTEX_SHADER, "hw4/epic.vert" },
        { GL_FRAGMENT_SHADER, "hw4/epic.frag"}
    };
    std::shared_ptr<EpicShader> shader = std::make_shared<EpicShader>(shaderSpec);
    shader->SetMetallic(.2f);
    shader->SetRoughness(.8f);
    shader->SetSpecular(glm::vec4(2.0f));
	
	std::shared_ptr<EpicShader> skullShader = std::make_shared<EpicShader>(shaderSpec);
	shader->SetMetallic(.9f);
	shader->SetRoughness(.2f);
	shader->SetSpecular(glm::vec4(3.f));

    std::shared_ptr<EpicShader> groundShader = std::make_shared<EpicShader>(shaderSpec);

    std::unique_ptr<LightProperties> lightProperties = make_unique<LightProperties>();
    lightProperties->diffuseColor = glm::vec4(3.f, 1.f, 1.f, 1.f);
    lightProperties->specularColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

    pointLight = std::make_shared<Light>(std::move(lightProperties), Light::LightType::POINT);
    pointLight->SetPosition(glm::vec3(100.f, 20.f, 10.f));
    scene->AddLight(pointLight);

    std::unique_ptr<LightProperties> sunProperties = make_unique<LightProperties>();
    sunProperties->diffuseColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    sunProperties->specularColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    sunLight = std::make_shared<Light>(std::move(sunProperties), Light::LightType::DIRECTIONAL);
    scene->AddLight(sunLight);

    std::unique_ptr<LightProperties> hemisphereProperties = make_unique<LightProperties>();
    hemisphereProperties->diffuseColor = glm::vec4(0.89, 0.349f, 0.f, 1.f) * 5.f; // ground color
    hemisphereProperties->specularColor = glm::vec4(0.f, 0.f, 1.f, 1.f) * 5.f; // sky color
    hemisphereLight = std::make_shared<Light>(std::move(hemisphereProperties), Light::LightType::HEMISPHERE);
    scene->AddLight(hemisphereLight);

    std::unique_ptr<SpotlightProperties> spotlightProperties = make_unique<SpotlightProperties>();
    spotlightProperties->diffuseColor = glm::vec4(1.f, 1.f, 0.f, 1.f) * 3.f;
    spotlightProperties->specularColor = glm::vec4(1.f, 1.f, 0.f, 1.f);
    spotlightProperties->innerConeAngleDegrees = 20.f;
    spotlightProperties->outerConeAngleDegrees = 50.f;
    spotLight = std::make_shared<Light>(std::move(spotlightProperties), Light::LightType::SPOT);
    spotLight->SetPosition(glm::vec3(16.f, 80.f, 16.f));
    spotLight->Rotate(glm::vec3(SceneObject::GetWorldRight()), -M_PI / 2.f);
    scene->AddLight(spotLight);

    GenericSetupExample(shader, skullShader, groundShader);

}

void Assignment4::GenericSetupExample(std::shared_ptr<ShaderProgram> shader, std::shared_ptr<ShaderProgram> skullShader, std::shared_ptr<ShaderProgram> groundShader)
{
    std::shared_ptr<RenderingObject> torusTemplate = PrimitiveCreator::CreateTorus(shader, 4.f, 1.5f, 100);

    // Give a R/G/B color to each vertex to visualize the sphere.
    auto totalVertices = torusTemplate->GetTotalVertices();

    std::unique_ptr<RenderingObject::ColorArray> vertexColors = make_unique<RenderingObject::ColorArray>();
    vertexColors->reserve(totalVertices);

    for (decltype(totalVertices) i = 0; i < totalVertices; ++i) {
        vertexColors->emplace_back(1.f, 1.f, 1.0f, 1.f);
    }
    torusTemplate->SetVertexColors(std::move(vertexColors));

    torusDance.clear();
    // 10x10 grid of spheres.
    for (int x = 0; x < 10; ++x)  {
        for (int y = 0; y < 10; ++y) {
            std::shared_ptr<class SceneObject> sceneObject = std::make_shared<SceneObject>(torusTemplate);
            sceneObject->SetPosition(glm::vec3(x * 12.f, 0.f, y * 12.f));
            scene->AddSceneObject(sceneObject);
            torusDance.push_back(sceneObject);
        }
    }

	std::vector<std::shared_ptr<RenderingObject>> skullTemplate = MeshLoader::LoadMesh(skullShader, "skull/12140_Skull_v3_L2.obj");
	skullObject = std::make_shared<SceneObject>(skullTemplate);
	skullObject->SetPosition(glm::vec3(60.f, 0.f, 60.f));
	skullObject->MultScale(2.f);
	skullObject->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -PI / 2.f);
	skullObject->Rotate(glm::vec3(0.0f, 1.f, 0.f), -PI / 2.f);
	scene->AddSceneObject(skullObject);

    std::shared_ptr<RenderingObject> plane = PrimitiveCreator::CreatePlane(groundShader);
    std::shared_ptr<class SceneObject> groundObject = std::make_shared<SceneObject>(plane);
    groundObject->MultScale(150.f);
    groundObject->SetPosition(glm::vec3(-10.f, 0.f, -10.f));
    groundObject->Translate(glm::vec3(SceneObject::GetWorldUp() * -5.f));

    std::unique_ptr<RenderingObject::ColorArray> groundColor = make_unique<RenderingObject::ColorArray>();
    groundColor->reserve(4);

    for (int i = 0; i < 4; ++i) {
        groundColor->emplace_back(0.89, 0.349f, 0.f, 1.f);
    }
    plane->SetVertexColors(std::move(groundColor));

    scene->AddSceneObject(groundObject);
}

void Assignment4::Tick(double deltaTime)
{
    if (sunLight) {
        sunLight->Rotate(glm::vec3(SceneObject::GetWorldRight()), PI * deltaTime / -4.f);
    }

    static float elapsedTime = 0.f;
    elapsedTime += deltaTime;

    for (size_t i = 0; i < torusDance.size(); ++i) {
        glm::vec4 og = torusDance[i]->GetPosition();
		//torusDance[i]->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), std::sin(elapsedTime / PI) * PI);
        torusDance[i]->SetPosition(glm::vec3(og.x, std::abs(std::sin(elapsedTime + (float)i)) * 6.f, og.z));
    }
	glm::vec4 skullPos = skullObject->GetPosition();
	skullObject->SetPosition(glm::vec3(skullPos.x, 6.f + 6.f * std::abs(std::sin(elapsedTime)), skullPos.z));
}
