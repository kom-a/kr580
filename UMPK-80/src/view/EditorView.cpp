#include "EditorView.h"

#include <sstream>
#include <fstream>

EditorView::EditorView()
	: m_Editor(),
	m_Compiler(),
	m_Disassembler()
{
	m_Open = true;

	m_Editor.SetShowWhitespaces(false);

	auto lang = TextEditor::LanguageDefinition::C();
	lang.mIdentifiers.clear();
	lang.mKeywords.clear();

	lang.mCaseSensitive = false;
	
	lang.mKeywords.insert("MOV");
	lang.mKeywords.insert("MVI");
	lang.mKeywords.insert("NOP");
	lang.mKeywords.insert("LXI");
	lang.mKeywords.insert("RLC");
	lang.mKeywords.insert("CMP");
	lang.mKeywords.insert("JNZ");
	lang.mKeywords.insert("RST1");
	lang.mKeywords.insert("INX");
	lang.mKeywords.insert("JMP");
	lang.mKeywords.insert("PUSH");
	lang.mKeywords.insert("XCHG");
	lang.mKeywords.insert("LDAX");
	lang.mKeywords.insert("CALL");
	lang.mKeywords.insert("ANI");
	lang.mKeywords.insert("RRC");
	lang.mKeywords.insert("RET");
	lang.mKeywords.insert("POP");
	lang.mKeywords.insert("STA");
	lang.mKeywords.insert("ADD");

	const char* const identifiers[] = {
			"A", "B", "C", "D", "E", "F", "H", "L", "M",
			"PSW", "BC", "DE", "HL"
	};

	for (auto& k : identifiers)
	{
		TextEditor::Identifier id;
		id.mDeclaration = "Register";
		lang.mIdentifiers.insert(std::make_pair(std::string(k), id));
	}

	m_Editor.SetLanguageDefinition(lang);

	const TextEditor::Palette palette = { {
			0xff7f7f7f,	// Default
			0xffd69c56,	// Keyword	
			0xff9bc64d,	// Number
			0xff7070e0,	// String
			0xff70a0e0, // Char literal
			0xffffffff, // Punctuation
			0xff408080,	// Preprocessor
			0xffaaaaaa, // Identifier
			0xff9bc64d, // Known identifier
			0xffc040a0, // Preproc identifier
			0xff206020, // Comment (single line)
			0xff406020, // Comment (multi line)
			0xff252525, // Background
			0xffe0e0e0, // Cursor
			0x80a06020, // Selection
			0x800020ff, // ErrorMarker
			0x40f08000, // Breakpoint
			0xff707000, // Line number
			0x40000000, // Current line fill
			0x40808080, // Current line fill (inactive)
			0x40a0a0a0, // Current line edge
		} };

	m_Editor.SetPalette(palette);
}

EditorView::~EditorView()
{

}

void EditorView::Render(KR580VM80A* emu)
{
	if (!m_Open)
		return;

	const uint32_t offset = 0x0800;	// Hardcode this for now

	auto cpos = m_Editor.GetCursorPosition();
	static const char* fileToEdit = "C:/Users/Камиль/Desktop/source_code.asm";

	ImGui::Begin("Editor", &m_Open, ImGuiWindowFlags_HorizontalScrollbar);

	auto& style = ImGui::GetStyle();
	ImVec2 editor_size = ImGui::GetContentRegionAvail();
	editor_size.y -= ImGui::GetFontSize() + style.CellPadding.y;

	m_Editor.Render("Editor", editor_size);
	ImGui::Text("%d:%-6d", cpos.mLine + 1, cpos.mColumn + 1);
	ImGui::SameLine();
	ImGui::Text("%s%s", fileToEdit, m_Editor.CanUndo() ? "*" : " ");
	
	ImGui::End();
}

bool EditorView::LoadFromFile(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
		return false;

	std::stringstream source;
	std::string line;
	while (std::getline(file, line)) {
		source << line << '\n';
	}

	m_Editor.SetText(source.str());
	file.close();
	return true;
}

bool EditorView::SaveToFile(const std::string& filename)
{
	std::ofstream file(filename);
	if (!file.is_open())
		return false;

	file << m_Editor.GetText();
	file.close();
	return true;
}
