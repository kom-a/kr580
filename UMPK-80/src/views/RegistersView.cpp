#include "RegistersView.h"

RegistersView::RegistersView()
{
	m_State.DataEditingAddr = nullptr;
	m_State.DataEditingTakeFocus = false;

	 m_Style.FormatByte = "%02X";

	 m_Sizes.GlyphWidth = 9 + 1; //  ImGui::CalcTextSize("F").x + 1;
}

RegistersView::~RegistersView()
{

}

void RegistersView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	ImGui::Begin("Registers");

	DrawRegister("A:", emu->A);

	DrawRegister("B:", emu->B);
	ImGui::SameLine();
	DrawRegister("C:", emu->C);

	DrawRegister("D:", emu->D);
	ImGui::SameLine();
	DrawRegister("E:", emu->E);

	DrawRegister("H:", emu->H);
	ImGui::SameLine();
	DrawRegister("L:", emu->L);

	ImGui::End();

	return;
	ImGui::Begin("Registers", &m_Open);

	ImGui::Text("A: %02X", emu->A);
	ImGui::SameLine();
	ImGui::Text("F: %02X", emu->F);

	ImGui::Text("B: %02X", emu->B);
	ImGui::SameLine();
	ImGui::Text("C: %02X", emu->C);

	ImGui::Text("D: %02X", emu->D);
	ImGui::SameLine();
	ImGui::Text("E: %02X", emu->E);

	ImGui::Text("H: %02X", emu->H);
	ImGui::SameLine();
	ImGui::Text("L: %02X", emu->L);

	ImGui::Text("SP: %04X", emu->SP);
	ImGui::Text("PC: %04X", emu->PC);
	
	ImGui::End();
}

void RegistersView::DrawRegister(const char* label, WORD& reg)
{
	ImGui::Text(label);
	ImGui::SameLine();
	if (m_State.DataEditingAddr == &reg)
	{
		bool data_write = false;
		ImGui::PushID(&reg);
		if (m_State.DataEditingTakeFocus)
		{
			ImGui::SetKeyboardFocusHere(0);
			sprintf(m_State.DataInputBuf, m_Style.FormatByte, reg);
		}
		struct UserData
		{
			// FIXME: We should have a way to retrieve the text edit cursor position more easily in the API, this is rather tedious. This is such a ugly mess we may be better off not using InputText() at all here.
			static int Callback(ImGuiInputTextCallbackData* data)
			{
				UserData* user_data = (UserData*)data->UserData;
				if (!data->HasSelection())
					user_data->CursorPos = data->CursorPos;
				if (data->SelectionStart == 0 && data->SelectionEnd == data->BufTextLen)
				{
					// When not editing a byte, always refresh its InputText content pulled from underlying memory data
					// (this is a bit tricky, since InputText technically "owns" the master copy of the buffer we edit it in there)
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, user_data->CurrentBufOverwrite);
					data->SelectionStart = 0;
					data->SelectionEnd = 2;
					data->CursorPos = 0;
				}
				return 0;
			}
			char   CurrentBufOverwrite[3];  // Input
			int    CursorPos;               // Output
		};
		UserData user_data;
		user_data.CursorPos = -1;
		sprintf(user_data.CurrentBufOverwrite, m_Style.FormatByte, reg);
		ImGuiInputTextFlags flags = ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_NoHorizontalScroll | ImGuiInputTextFlags_CallbackAlways;
#if IMGUI_VERSION_NUM >= 18104
		flags |= ImGuiInputTextFlags_AlwaysOverwrite;
#else
		flags |= ImGuiInputTextFlags_AlwaysInsertMode;
#endif

		ImGui::SetNextItemWidth(m_Sizes.GlyphWidth * 2);
		if (ImGui::InputText("##data", m_State.DataInputBuf, IM_ARRAYSIZE(m_State.DataInputBuf), flags, UserData::Callback, &user_data))
			data_write = true;
		else if (!m_State.DataEditingTakeFocus && !ImGui::IsItemActive())
			m_State.DataEditingAddr = nullptr;
		m_State.DataEditingTakeFocus = false;
		if (user_data.CursorPos >= 2)
			data_write = true;
		unsigned int data_input_value = 0;
		if (data_write && sscanf(m_State.DataInputBuf, "%X", &data_input_value) == 1)
		{
			reg = (ImU8)data_input_value;
			m_State.DataEditingAddr = nullptr;
		}
		ImGui::PopID();
	}
	else
	{
		if (reg == 0)
			ImGui::TextDisabled("00");
		else
			ImGui::Text(m_Style.FormatByte, reg);
	}
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		m_State.DataEditingTakeFocus = true;
		m_State.DataEditingAddr = &reg;
	}
}
