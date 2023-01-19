#include "ToolbarView.h"

#include <sstream>

#include "ViewManager.h"
#include "resource/texture/TextureManager.h"

#include "IconsFontAwesome4.h"
#include "IconsFontAwesome5.h"
#include "IconsFontAwesome6.h"

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
#if 1
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
				const int offset = 0x0800; // NOTE: hardcode this only for now

				m_Compiler.Compile(source, offset);

				TextEditor::ErrorMarkers compile_errors;
				if (!m_Compiler.errorOccured)
				{
					std::vector<uint8_t>& program = m_Compiler.resultBinary;
					emu->LoadProgram(program, offset);
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
				uint8_t* user_memory = &emu->Memory[USER_MEMORY_OFFSET];
				std::vector<uint8_t> machine_code = std::vector<uint8_t>(user_memory, user_memory + USER_MEMORY_SIZE);
				m_Disassembler.Disassemble(machine_code, USER_MEMORY_OFFSET);

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
				emu->PC = USER_MEMORY_OFFSET;
				emu->SP = STACK_OFFSET + STACK_SIZE;
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
#else

	ViewsController& viewManager = ViewsController::GetInstance();

	ImGui::Begin("Contorl");

	if (ImGui::Button("Assemble"))
	{
		EditorView* editor = viewManager.GetEditorView();
		std::string source = editor->GetText();
		const int offset = 0x0800; // NOTE: hardcode this only for now

		m_Compiler.Compile(source, offset);

		TextEditor::ErrorMarkers compile_errors;
		if (!m_Compiler.errorOccured)
		{
			std::vector<uint8_t>& program = m_Compiler.resultBinary;
			emu->LoadProgram(program, offset);
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
	
	if (ImGui::Button("Disassemble"))
	{
		uint8_t* user_memory = &emu->Memory[USER_MEMORY_OFFSET];
		std::vector<uint8_t> machine_code = std::vector<uint8_t>(user_memory, user_memory + USER_MEMORY_SIZE);
		m_Disassembler.Disassemble(machine_code, USER_MEMORY_OFFSET);

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
	
	if (ImGui::Button("Step"))
	{
		emu->Step();
	}
	
	if (ImGui::Button("Run"))
	{
		emu->Running = true;
	}
	
	if (ImGui::Button("Stop"))
	{
		emu->Running = false;
	}
	
	if (ImGui::Button("Reset"))
	{
		emu->PC = USER_MEMORY_OFFSET;
		emu->SP = STACK_OFFSET + STACK_SIZE;
	}

	ImGui::End();
#endif
}
