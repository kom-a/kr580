#include "ToolbarView.h"

#include <sstream>

#include "ViewManager.h"
#include "Settings.h"

#include "Icons.h"

ToolbarView::ToolbarView()
	: m_Compiler(),
	m_Disassembler()
{

}

ToolbarView::~ToolbarView()
{

}

void ToolbarView::Render(KR580VM80A* emu)
{
	ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
	ImGuiStyle style = ImGui::GetStyle();
	float height = ImGui::GetFrameHeight() + style.FramePadding.y;

	ViewManager& viewManager = ViewManager::GetInstance();

	if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, windowFlags)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button(ICON_FA_HAMMER " Assemble"))
			{
				EditorView* editor = viewManager.GetEditorView();
				std::string source = editor->GetText();
				const int load_address = Settings::Get().LoadAtProgramCounter ? emu->PC : Settings::Get().LoadAddress;

				m_Compiler.Compile(source, load_address);

				TextEditor::ErrorMarkers compile_errors;
				if (!m_Compiler.errorOccured)
				{
					std::vector<uint8_t>& program = m_Compiler.resultBinary;
					emu->LoadProgram(program, load_address);
				}
				else
				{
					for (auto& error : m_Compiler.compileErrors.messages)
					{
						auto& line = std::get<0>(error);
						auto& message = std::get<1>(error);

						auto& pair = std::make_pair<const int, std::string>(std::move(line), std::move(message));
						compile_errors.insert(pair);
					}
				}

				editor->SetErrors(compile_errors);
			}

			if (ImGui::Button(ICON_FA_CODE " Disassemble"))
			{
				uint8_t* user_memory = &emu->Memory[KR580VM80A::USER_MEMORY_OFFSET];
				std::vector<uint8_t> machine_code = std::vector<uint8_t>(user_memory, user_memory + KR580VM80A::USER_MEMORY_SIZE);
				m_Disassembler.Disassemble(machine_code, KR580VM80A::USER_MEMORY_OFFSET);

				if (!m_Disassembler.errorOccured)
				{
					std::stringstream source;
					for (auto& line : m_Disassembler.mnemonics)
						source << line;
					viewManager.GetEditorView()->SetText(source.str());
				}
				else
				{
					const std::string& error_line = std::get<0>(m_Disassembler.disassembleError.message);
					const std::string& error_message = std::get<1>(m_Disassembler.disassembleError.message);
					std::string message = error_message + "at address \'0x" + error_line + "\'";

					viewManager.GetEditorView()->SetText(message);
				}
			}

			if (ImGui::Button(ICON_FA_STEP_FORWARD " Step"))
			{
				emu->Step();
			}

			if (ImGui::Button(ICON_FA_RUNNING " Run"))
			{
				emu->Running = true;
			}

			if (ImGui::Button( ICON_FA_STOP " Stop"))
			{
				emu->Running = false;
			}

			if (ImGui::Button(ICON_FA_REFRESH " Reset"))
			{
				emu->PC = KR580VM80A::USER_MEMORY_OFFSET;
				emu->SP = KR580VM80A::STACK_OFFSET + KR580VM80A::STACK_SIZE;
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
}
