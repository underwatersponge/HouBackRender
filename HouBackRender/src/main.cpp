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

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main()
{
	// init need class
	GenBatch genBatch = GenBatch("config.json", "RenList.json");

	glfwSetErrorCallback(GlfwErrorCallback);
	
	if (!glfwInit())
		return 1;
	const char* glsl_version = nullptr;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	GLFWwindow* window = glfwCreateWindow((int)(500*main_scale), (int)(600*main_scale), "HouBackRender", NULL, NULL);
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
			static bool b_open = true;
			// ============================ 
			ImGuiWindowFlags windowFlags = 0;
			windowFlags |= ImGuiWindowFlags_MenuBar;

			// TODO: a beautifully ui maybe use a tree-like?
			ImGui::Begin("Setting", nullptr, windowFlags);
			
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("SaveSetting"))
				{
					if (ImGui::MenuItem("SaveBinDir"))
					{
						//genBatch.WriteCurSettingToFile("test.json", GenBatch::SaveSettingType::OnlyHouBinPath);
					}
					if (ImGui::MenuItem("SaveBinAHip"))
					{
						//genBatch.WriteCurSettingToFile("test.json", GenBatch::SaveSettingType::BinDirAHipPath);
					}
					if (ImGui::MenuItem("SaveAll"))
					{
						//genBatch.WriteCurSettingToFile("test.json", GenBatch::SaveSettingType::All);
					}
					ImGui::EndMenu();
				}
					ImGui::EndMenuBar();
			}
			
			ImGui::Text("welcome have fun!"); ImGui::SameLine();
			Utility::ImGuiHelpMarker("note:be carefully use the savesetting which save the all and SaveBinAHip(maybe) now!\nit do not have mush test(even no)");
			Utility::ImGuiTextWithScale("Helpers->:", 1.3f); ImGui::SameLine();
			Utility::ImGuiHelpMarker("Click button #SelHouBin# to choose your houdini bin directory! Click button #SelHipFile# to choose your hipfile! \n"
				"Click button #CleanFiles# to clean the current selected hip file! Click button #ClearAllRenNode# to clean current seted ren hou node path!\n"
				" Type your hou node path in below inputbox the click button #AddToRender# to add it to render\n "
				"Note:now only support type one by one!\n "
				"Click button #GenBatch# to generate batch file,then double click gen batch file to render!");
			Utility::ImGuiTextWithScale("Tips->", 1.3f); ImGui::SameLine();
			Utility::ImGuiHelpMarker("For now do not support multi files!but you can render multfiles with generate a batch file then clean the hipfile and rendnodepath and create new of they to generate a new batch file!");
			ImGui::Spacing();

			if(ImGui::Button("SelHouBin"))
			{
				genBatch.SetHouBinPathFromDir(window);
			}
			ImGui::SameLine();
			if (ImGui::Button("SelHipFile"))
			{
				genBatch.AddHipPathFromFile(window);
			}
			ImGui::SameLine();
			if (ImGui::Button("Clean"))
			{
				genBatch.Clean(0);
			}
			ImGui::SameLine();
			if (ImGui::Button("ClearAll"))
			{
				genBatch.CleanAll();
			}
			ImGui::SameLine();
			if (ImGui::Button("LoadRenContainer"))
			{
				genBatch.ReadRenContainerFromFile("RenList.json");
			}
			static char renNodeBuf[100] = "";
			ImGui::InputText("##RenNode", renNodeBuf, 100);
			if (ImGui::Button("AddToRender"))
			{
				//genBatch.AddHouRenNodePath(std::string(renNodeBuf));
				genBatch.AddHouRenNodePath(0, renNodeBuf);
			}

			// status view
			ImGui::SeparatorText("Current Status!");
			Utility::ImGuiTextWithScale("Current HouBinDir:", 1.5f);
			ImGui::Text(genBatch.GetHouBinDir().c_str());
			
			Utility::ImGuiTextWithScale("Current HipFile and RenNodes:", 1.5f);
			if (0){
			for (size_t i=0; i<genBatch.GetSize(); ++i)
			{
				const std::string& hipFiles = genBatch.GetHipPath(i);
				if (ImGui::TreeNode(hipFiles.c_str()))
				{
					static int itemSelIndex = 0;
					// static bool itemHighLighted = false;
					int itemHighLightedIndex = -1;
					// ImGui::Checkbox("Highlight hovered item", &itemHighLighted);
					if (ImGui::BeginListBox("nodes"))
					{
						const std::vector<std::string>& renNodes = genBatch.GetNodesPaths(i);
						for (int n=0; n<renNodes.size(); ++n)
						{
							const bool bSelected = (itemSelIndex == n);
							if (ImGui::Selectable(renNodes[n].c_str(), bSelected))
							{
								itemHighLightedIndex = n;
							}
							if (bSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndListBox();
					}
					ImGui::TreePop();
				}
			}
			}
			
			
			ImGui::Text("////////////////Testing///////////");
			if (ImGui::Button("GenRunPyBatch"))
				genBatch.GenRunPyBatch();
			ImGui::SameLine();
			if (ImGui::Button("GenConfigJson"))
				genBatch.GenConfigJson();

			if (ImGui::Button("GenRenListJson"))
				genBatch.GenRenListJson();
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
	
	return 0;
}