#include "ControlView.h"

#include <sstream>

#include "../ViewsController.h"

ControlView::ControlView()
	: m_Compiler(),
	m_Disassembler()
{

}

ControlView::~ControlView()
{

}

void ControlView::Render(KR580VM80A* emu)
{
	ViewsController& views_controller = ViewsController::GetInstance();

	ImGui::Begin("Contorl");

	if (ImGui::Button("Assemble"))
	{
		EditorView* editor = views_controller.GetEditorView();
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
			views_controller.GetEditorView()->SetText(source.str());
		}
		else
		{
			const std::string& error_line = std::get<0>(m_Disassembler.disassembleError.message);
			const std::string& error_message = std::get<1>(m_Disassembler.disassembleError.message);
			std::string message = error_message + "at address \'0x" + error_line + "\'";

			views_controller.GetEditorView()->SetText(message);
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
}
