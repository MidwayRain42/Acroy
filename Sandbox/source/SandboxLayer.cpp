#include "SandboxLayer.hpp"
#include <Core/Log.hpp>
#include <Renderer/Renderer.hpp>
#include <Events/ApplicationEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Renderer/Texture.hpp>
#include <Scene/Entity.hpp>
#include <Scene/Components.hpp>
#include <Renderer/PerspectiveCamera.hpp>
#include <Scene/ScriptableEntity.hpp>
#include <imgui.h>

#include "CameraController.hpp"


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
    // cubeMat.albedoTex = cubeTexture;
    cubeMat.scale = 2.0f;


    // ----------------------------------------
    // -------------- Setup Scene -------------
    // ----------------------------------------

    auto camera = Acroy::CreateRef<Acroy::PerspectiveCamera>(90.f, 16.0f/9.0f, 0.1f, 100.0f);

    m_scene = Acroy::CreateRef<Acroy::Scene>(camera);

    Acroy::Entity cameraEntity = m_scene->Create("Camera");
    cameraEntity.AddComponent<Acroy::CameraComponent>(camera);
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

    cameraEntity.AddComponent<Acroy::NativeScriptComponent>().Bind<CameraController>();

    // Store named entity handles for the inspector panel
    m_entities.emplace_back("Camera", cameraEntity);
    m_entities.emplace_back("Ground", groundEntity);
    m_entities.emplace_back("Cube",   cubeEntity);
}

void SandboxLayer::OnUpdate(Acroy::Timestep timestep)
{
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
    if (ImGui::DragFloat3("##v", &degrees.x))
        radiansValue = glm::radians(degrees);

    ImGui::SameLine();
    if (ImGui::SmallButton("R"))
        radiansValue = glm::vec3(0.0f);

    ImGui::PopID();
}

// Draw a styled component section header (colored bar + label).
static bool DrawComponentHeader(const char* label, ImVec4 color)
{
    ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(color.x, color.y, color.z, 0.35f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(color.x, color.y, color.z, 0.50f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive,  ImVec4(color.x, color.y, color.z, 0.70f));
    bool open = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen);
    ImGui::PopStyleColor(3);
    return open;
}

void SandboxLayer::DrawComponentsPanel()
{
    ImGui::Begin("Components");

    if (m_selectedEntity < 0 || m_selectedEntity >= (int)m_entities.size())
    {
        ImGui::TextDisabled("Select an entity in the Scene Inspector.");
        ImGui::End();
        return;
    }

    auto& [name, entity] = m_entities[m_selectedEntity];

    // Entity title
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.85f, 0.4f, 1.0f));
    ImGui::Text("%s", name.c_str());
    ImGui::PopStyleColor();
    ImGui::Separator();
    ImGui::Spacing();

    // ---- TagComponent ----
    if (entity.HasComponent<Acroy::TagComponent>())
    {
        if (DrawComponentHeader("Tag", {0.5f, 0.8f, 1.0f, 1.0f}))
        {
            auto& tc = entity.GetComponent<Acroy::TagComponent>();
            char buf[256];
            strncpy(buf, tc.tag.c_str(), sizeof(buf));
            buf[sizeof(buf) - 1] = '\0';
            ImGui::SetNextItemWidth(-1);
            if (ImGui::InputText("##tag", buf, sizeof(buf)))
                tc.tag = buf;
            ImGui::Spacing();
        }
    }

    // ---- TransformComponent ----
    if (entity.HasComponent<Acroy::TransformComponent>())
    {
        if (DrawComponentHeader("Transform", {0.4f, 1.0f, 0.6f, 1.0f}))
        {
            auto& tc = entity.GetComponent<Acroy::TransformComponent>();
            if (ImGui::BeginTable("##tc", 2, ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerV))
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
    }

    // ---- MeshComponent ----
    if (entity.HasComponent<Acroy::MeshComponent>())
    {
        if (DrawComponentHeader("Mesh", {1.0f, 0.6f, 0.3f, 1.0f}))
        {
            auto& mc = entity.GetComponent<Acroy::MeshComponent>();
            ImGui::TextDisabled("Mesh ptr: %p", (void*)mc.mesh.get());
            ImGui::Spacing();
        }
    }

    // ---- MaterialComponent ----
    if (entity.HasComponent<Acroy::MaterialComponent>())
    {
        if (DrawComponentHeader("Material", {0.9f, 0.4f, 0.9f, 1.0f}))
        {
            auto& mat = entity.GetComponent<Acroy::MaterialComponent>();

            ImGui::TextDisabled("Shader: %p", (void*)mat.shader.get());

            ImGui::SetNextItemWidth(120.0f);
            ImGui::DragFloat("UV Scale", &mat.scale, 0.05f, 0.01f, 100.0f, "%.2f");

            ImGui::ColorEdit4("Albedo Color", &mat.albedo.x);

            if (mat.albedoTex)
                ImGui::TextDisabled("Albedo Tex: %p", (void*)mat.albedoTex.get());
            else
                ImGui::TextDisabled("Albedo Tex: (none)");

            ImGui::Spacing();
        }
    }

// ---- CameraComponent ----
if (entity.HasComponent<Acroy::CameraComponent>())
{
    if (DrawComponentHeader("Camera", {0.4f, 0.7f, 1.0f, 1.0f}))
    {
        auto& cc = entity.GetComponent<Acroy::CameraComponent>();
        ImGui::Checkbox("Primary", &cc.primary);

        // Cast to PerspectiveCamera to expose FOV / near / far controls
        auto* persp = dynamic_cast<Acroy::PerspectiveCamera*>(cc.camera.get());
        if (persp)
        {
            float fov = persp->GetFOV();
            if (ImGui::DragFloat("FOV", &fov, 0.5f, 10.0f, 170.0f, "%.1f deg"))
                persp->SetFOV(fov);

            // Aspect is read-only (driven by window resize)
            float aspect = persp->GetAspect();
            ImGui::BeginDisabled();
            ImGui::DragFloat("Aspect", &aspect, 0.0f, 0.0f, 0.0f, "%.3f");
            ImGui::EndDisabled();
        }
        else
        {
            ImGui::TextDisabled("(orthographic / unknown camera type)");
        }

        ImGui::Spacing();
    }
}

    // ---- NativeScriptComponent ----
    if (entity.HasComponent<Acroy::NativeScriptComponent>())
    {
        if (DrawComponentHeader("Native Script", {1.0f, 0.75f, 0.3f, 1.0f}))
        {
            auto& nsc = entity.GetComponent<Acroy::NativeScriptComponent>();
            ImGui::TextDisabled("Instance: %p", (void*)nsc.instance);
            ImGui::TextDisabled("Instantiate fn: %p", (void*)nsc.InstaniateScript);
            ImGui::Spacing();
        }
    }

    ImGui::End();
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

    for (int i = 0; i < (int)m_entities.size(); ++i)
    {
        auto& [name, entity] = m_entities[i];

        // Highlight selected entity
        bool selected = (m_selectedEntity == i);
        if (selected)
        {
            ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(0.26f, 0.59f, 0.98f, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.55f));
        }

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (selected) flags |= ImGuiTreeNodeFlags_Selected;

        bool open = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, "%s", name.c_str());

        if (selected) ImGui::PopStyleColor(2);

        // Clicking the node selects the entity
        if (ImGui::IsItemClicked())
            m_selectedEntity = i;

        if (open)
        {
            // Show a compact tag list of attached components
            if (entity.HasComponent<Acroy::TransformComponent>())  ImGui::BulletText("Transform");
            if (entity.HasComponent<Acroy::MeshComponent>())       ImGui::BulletText("Mesh");
            if (entity.HasComponent<Acroy::MaterialComponent>())   ImGui::BulletText("Material");
            if (entity.HasComponent<Acroy::CameraComponent>())     ImGui::BulletText("Camera");
            if (entity.HasComponent<Acroy::NativeScriptComponent>()) ImGui::BulletText("NativeScript");

            ImGui::TreePop();
        }

        ImGui::Spacing();
    }

    ImGui::End();

    // ----------------------------------------
    // --------- Components Panel -------------
    // ----------------------------------------

    DrawComponentsPanel();
}

void SandboxLayer::OnEvent(Acroy::Event& event)
{
    if (event.GetEventType() == Acroy::EventType::WindowResize)
    {
        auto e = dynamic_cast<Acroy::WindowResizeEvent*>(&event);
        m_scene->OnWindowResize(e->GetWidth(), e->GetHeight());
    }
}