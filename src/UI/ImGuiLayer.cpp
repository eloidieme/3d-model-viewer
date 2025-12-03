#include "UI/ImGuiLayer.hpp"
#include "Core/Log.hpp"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

ImGuiLayer::ImGuiLayer(GLFWwindow *windowHandle)
    : m_windowHandle(windowHandle) {}

void ImGuiLayer::onAttach() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  float xscale, yscale;
  glfwGetWindowContentScale(m_windowHandle, &xscale, &yscale);

  int winWidth, winHeight;
  int fbWidth, fbHeight;
  glfwGetWindowSize(m_windowHandle, &winWidth, &winHeight);
  glfwGetFramebufferSize(m_windowHandle, &fbWidth, &fbHeight);

  float fbScaleX = (winWidth > 0) ? (float)fbWidth / winWidth : 1.0f;

  float uiScale = xscale / fbScaleX;

  io.Fonts->AddFontDefault();
  io.FontGlobalScale = uiScale;

  if (uiScale > 1.0f) {
    ImGui::GetStyle().ScaleAllSizes(uiScale);
  }

  ImGui::StyleColorsDark();

  ImGuiStyle &style = ImGui::GetStyle();
  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    style.WindowRounding = 0.0f;
    style.Colors[ImGuiCol_WindowBg].w = 1.0f;
  }

  ImGui_ImplGlfw_InitForOpenGL(m_windowHandle, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  LOG_CORE_INFO("ImGui Layer Initialized");
}

void ImGuiLayer::onDetach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::begin() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::end() {
  ImGuiIO &io = ImGui::GetIO();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
  }
}

void ImGuiLayer::onEvent(Event &e) {
  ImGuiIO &io = ImGui::GetIO();

  if (e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse) {
    e.m_handled = true;
    return;
  }

  if (e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard) {
    e.m_handled = true;
    return;
  }

  if (e.getType() == EventType::KeyTyped) {
    KeyTypedEvent &event = static_cast<KeyTypedEvent &>(e);
    io.AddInputCharacter((unsigned int)event.getKeyCode());
  }
}