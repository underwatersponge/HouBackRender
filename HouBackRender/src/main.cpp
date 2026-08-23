#include "GenBatch.h"
#include "Utility.cpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"

#include <iostream>
static void GlfwErrorCallback(int error, const char* description)
{
	std::cout << "GLFW ERROR:" << error << description<< "\n";
}

int main()
{
	// init need class
	GenBatch genBatch = GenBatch();

	glfwSetErrorCallback(GlfwErrorCallback);
	
	if (!glfwInit())
		return 1;
	const char* glsl_version = nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow((int)(800*main_scale), (int)(450*main_scale), "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "can not load opengl function " << std::endl;
		return -1;
	}
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// io.ConfigViewportsNoAutoMerge = true;
	// io.ConfigViewportsNoTaskBarIcon = true;
	
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);
	style.FontScaleDpi = main_scale;
	
#if GLFW_VERSION_MAJOR >=3 && GLFW_VERSION_MINOR >= 3
	io.ConfigDpiScaleFonts = true;
	io.ConfigDpiScaleViewports = true;
#endif
	
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();
		
#ifdef _DEBUG
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
#endif
		{
			// ============================ //
			ImGui::Begin("Setting");
			ImGui::Text("hello this is a test");
			if(ImGui::Button("Open"))
			{
				genBatch.SetHouBinPathFromDir(window);
			}
			ImGui::SameLine();
			if (ImGui::Button("OpenDir"))
			{
				genBatch.AddHipPathFromFile(window);
			}
			static char renNodeBuf[100] = "";
			int bufSize = 100;
			ImGui::InputText("##RenNode", renNodeBuf, 100);
			if (ImGui::Button("AddToRender"))
			{
				genBatch.AddHouRenNodePath(std::string(renNodeBuf));
			}

			// status view
			Utility::ImGuiTextWithScale("Selected HouBinPath:", 1.5f);
			ImGui::Text(genBatch.GetHouBinPath().c_str());
			
			Utility::ImGuiTextWithScale("Choosed Render Hips:", 1.5f);
			const std::vector<std::string> hipFiles = genBatch.GetHipPaths();
			for (auto ite = hipFiles.begin(); ite != hipFiles.end(); ++ite)
			{
				ImGui::Text(ite->c_str());
			}

			Utility::ImGuiTextWithScale("Choosed Render Nodes:", 1.5f);
			const std::vector<std::string> renNodes = genBatch.GetNodesPaths();
			for (auto ite = renNodes.begin(); ite != renNodes.end(); ++ite)
			{
				ImGui::Text(ite->c_str());
			}

			if(ImGui::Button("GenBatch"))
				genBatch.WriteBatchToFile();
			ImGui::End();
		}
		
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
		
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
		glfwSwapBuffers(window);
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	
	glfwDestroyWindow(window);
	glfwTerminate();
	
	// ============== //
	//GenBatch().WriteToFile();
	return 0;
}