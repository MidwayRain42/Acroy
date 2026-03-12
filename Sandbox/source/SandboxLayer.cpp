#include "SandboxLayer.hpp"
#include <Core/Log.hpp>
#include <Renderer/Renderer.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Renderer/Texture.hpp>
#include <Scene/Entity.hpp>
#include <Scene/Components.hpp>
#include <Renderer/PerspectiveCamera.hpp>
#include <imgui.h>

void SandboxLayer::OnAttach()
{
    ACROY_INFO("SandboxLayer attached");

    // ----------------------------------------
    // -------------- Ground ------------------
    // ----------------------------------------

    float planeVertices[] = {
        // positions          // normals         // texcoords
        -15.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
         15.0f, 0.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         15.0f, 0.0f,  15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -15.0f, 0.0f,  15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    uint32_t planeIndices[] = { 0, 1, 2, 2, 3, 0 };
    
    Acroy::BufferLayout layout = {
        { "a_position", Acroy::ShaderDataType::Float3, false },
        { "a_normal", Acroy::ShaderDataType::Float3, false },
        { "a_texCoord", Acroy::ShaderDataType::Float2, false }
    };
    Acroy::Ref<Acroy::Mesh> groundMesh = Acroy::CreateRef<Acroy::Mesh>(planeVertices, sizeof(planeVertices), planeIndices, sizeof(planeIndices) / sizeof(uint32_t), layout);
    Acroy::Ref<Acroy::Texture2D> groundTexture = Acroy::CreateRef<Acroy::Texture2D>("/home/sam/Downloads/Grass008_2K-JPG/Grass008_2K-JPG_Color.jpg");

    // ----------------------------------------
    // -------------- Cube --------------------
    // ----------------------------------------

    float cubeVertices[] = {
    //  Positions           Normals       TextureCoords
        // Front
       -0.5, -0.5,  0.5,    0,  0,  1,    0, 0,
        0.5, -0.5,  0.5,    0,  0,  1,    1, 0,
        0.5,  0.5,  0.5,    0,  0,  1,    1, 1,
       -0.5,  0.5,  0.5,    0,  0,  1,    0, 1,
        // Back
       -0.5, -0.5, -0.5,    0,  0, -1,    1, 0,
        0.5, -0.5, -0.5,    0,  0, -1,    0, 0,
        0.5,  0.5, -0.5,    0,  0, -1,    0, 1,
       -0.5,  0.5, -0.5,    0,  0, -1,    1, 1,
        // Left
       -0.5, -0.5, -0.5,   -1,  0,  0,    0, 0,
       -0.5, -0.5,  0.5,   -1,  0,  0,    1, 0,
       -0.5,  0.5,  0.5,   -1,  0,  0,    1, 1,
       -0.5,  0.5, -0.5,   -1,  0,  0,    0, 1,
        // Right
        0.5, -0.5, -0.5,    1,  0,  0,    1, 0,
        0.5, -0.5,  0.5,    1,  0,  0,    0, 0,
        0.5,  0.5,  0.5,    1,  0,  0,    0, 1,
        0.5,  0.5, -0.5,    1,  0,  0,    1, 1,
        // Top
       -0.5,  0.5, -0.5,    0,  1,  0,    0, 1,
       -0.5,  0.5,  0.5,    0,  1,  0,    0, 0,
        0.5,  0.5,  0.5,    0,  1,  0,    1, 0,
        0.5,  0.5, -0.5,    0,  1,  0,    1, 1,
        // Bottom
       -0.5, -0.5, -0.5,    0, -1,  0,    1, 1,
       -0.5, -0.5,  0.5,    0, -1,  0,    1, 0,
        0.5, -0.5,  0.5,    0, -1,  0,    0, 0,
        0.5, -0.5, -0.5,    0, -1,  0,    0, 1
    };

    uint32_t cubeIndices[] = {
        0,1,2,2,3,0,    4,5,6,6,7,4,
        8,9,10,10,11,8, 12,13,14,14,15,12,
        16,17,18,18,19,16, 20,21,22,22,23,20
    };

    Acroy::Ref<Acroy::Mesh> cubeMesh = Acroy::CreateRef<Acroy::Mesh>(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices) / sizeof(uint32_t), layout);
 
    Acroy::Ref<Acroy::Texture2D> cubeTexture = Acroy::CreateRef<Acroy::Texture2D>("/home/sam/Documents/dev/OpenGL-Sandbox-main/resources/textures/Planks/planks.png");

    // ----------------------------------------
    // -------------- Shader ------------------
    // ----------------------------------------

    Acroy::Ref<Acroy::Shader> shader = Acroy::CreateRef<Acroy::Shader>("/home/sam/Documents/dev/Acroy/Sandbox/assets/shaders/SimpleShader.glsl");
    
    Acroy::MaterialComponent groundMat(shader);
    groundMat.albedoTex = groundTexture;
    groundMat.scale = 15.0f;

    Acroy::MaterialComponent cubeMat(shader);
    cubeMat.albedoTex = cubeTexture;
    cubeMat.scale = 2.0f;


    // ----------------------------------------
    // -------------- Setup Scene -------------
    // ----------------------------------------

    auto cam = Acroy::CreateRef<Acroy::PerspectiveCamera>(90.f, 16.0f/9.0f, 0.1f, 100.0f);

    m_scene = Acroy::CreateRef<Acroy::Scene>(cam);

    Acroy::Entity cameraEntity = m_scene->Create("Camera");
    cameraEntity.AddComponent<Acroy::CameraComponent>(cam);
    Acroy::TransformComponent& camTransform = cameraEntity.GetComponent<Acroy::TransformComponent>();
    camTransform.position = glm::vec3(0.0, 1.0, 0.0);

    Acroy::Entity groundEntity = m_scene->Create("Ground");
    groundEntity.AddComponent<Acroy::MeshComponent>(groundMesh);
    groundEntity.AddComponent<Acroy::MaterialComponent>(groundMat);

    Acroy::Entity cubeEntity = m_scene->Create("Cube");
    cubeEntity.AddComponent<Acroy::MeshComponent>(cubeMesh);
    cubeEntity.AddComponent<Acroy::MaterialComponent>(cubeMat);

    Acroy::TransformComponent& cubeTransform = cubeEntity.GetComponent<Acroy::TransformComponent>();
    cubeTransform.position = glm::vec3(0.0, 1.0, 0.0);
    cubeTransform.scale = glm::vec3(2.0f);

    // Store named entity handles for the inspector panel
    m_entities.emplace_back("Camera", cameraEntity);
    m_entities.emplace_back("Ground", groundEntity);
    m_entities.emplace_back("Cube",   cubeEntity);
}

void SandboxLayer::OnUpdate(Acroy::Timestep timestep)
{
    // Acroy::Renderer::SetClearColor({0.73f,0.81f,0.92f,1.0f});
    Acroy::Renderer::SetClearColor({72.f/255.f, 72.f/255.f, 72.f/255.f, 1.f});
    Acroy::Renderer::Clear();

    m_scene->OnUpdate(timestep);
}

// Helper to draw a labeled DragFloat3 row inside the transform panel.
static void DrawVec3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float speed = 0.1f)
{
    ImGui::PushID(label);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-1);
    ImGui::DragFloat3("##v", &values.x, speed);

    ImGui::SameLine();
    if (ImGui::SmallButton("R"))
        values = glm::vec3(resetValue);

    ImGui::PopID();
}

// Rotation variant: displays and edits in degrees, but tc.rotation is stored
// in radians (as glm::rotate expects). Converts on the way in and out.
static void DrawRotationControl(const char* label, glm::vec3& radiansValue)
{
    ImGui::PushID(label);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::TextUnformatted(label);

    ImGui::TableSetColumnIndex(1);
    ImGui::SetNextItemWidth(-1);

    glm::vec3 degrees = glm::degrees(radiansValue);
    // if (ImGui::DragFloat3("##v", &degrees.x, 1.0f, "%.1f"))
    if (ImGui::DragFloat3("##v", &degrees.x))
        radiansValue = glm::radians(degrees);

    ImGui::SameLine();
    if (ImGui::SmallButton("R"))
        radiansValue = glm::vec3(0.0f);

    ImGui::PopID();
}

void SandboxLayer::OnImGuiRender()
{
    ImGuiIO& io = ImGui::GetIO();

    // ----------------------------------------
    // ----------- Render Stats ---------------
    // ----------------------------------------

    ImGui::Begin("Render Stats");

    float fps = io.Framerate;
    float frametime = 1000.0f / (fps > 0.0f ? fps : 1.0f);

    ImGui::Text("Framerate: ");
    ImGui::SameLine();
    ImGui::Text("%.1f", fps);

    ImGui::Text("Frametime: ");
    ImGui::SameLine();
    ImGui::Text("%.2f ms", frametime);

    ImGui::End();

    // ----------------------------------------
    // --------- Scene Inspector --------------
    // ----------------------------------------

    ImGui::Begin("Scene Inspector");

    for (auto& [name, entity] : m_entities)  // pair<string, Entity>
    {
        // Collapsible header per entity
        bool open = ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        if (!open)
            continue;

        if (!entity.HasComponent<Acroy::TransformComponent>())
        {
            ImGui::TextDisabled("  (no TransformComponent)");
            continue;
        }

        Acroy::TransformComponent& tc = entity.GetComponent<Acroy::TransformComponent>();

        // Two-column table: label | control
        if (ImGui::BeginTable("##transform", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 70.0f);
            ImGui::TableSetupColumn("Value",    ImGuiTableColumnFlags_WidthStretch);

            DrawVec3Control("Position", tc.position, 0.0f, 0.05f);
            DrawRotationControl("Rotation", tc.rotation);
            DrawVec3Control("Scale",    tc.scale,    1.0f, 0.05f);

            ImGui::EndTable();
        }

        ImGui::Spacing();
    }

    ImGui::End();
}

void SandboxLayer::OnEvent(Acroy::Event& event)
{
    if (event.GetEventType() == Acroy::EventType::WindowResize)
    {
        auto e = dynamic_cast<Acroy::WindowResizeEvent*>(&event);
        m_scene->OnWindowResize(e->GetWidth(), e->GetHeight());
    }
}