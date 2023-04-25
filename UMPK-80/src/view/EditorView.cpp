#include "EditorView.h"
#include "KR580.h"

#include <string>
#include <sstream>
#include <fstream>


static std::string g_DebugProgramSrc = R"(inf:
	mvi b, 0xf7
	mvi c, 0x00
	mvi d, 0x00
loop:
	mov a, b
	out 0x07
	in 0x06
	cpi 0xff
	jz else
	xri 0xff
	cpi 0x04
	jnz notfour
	dcr  a
notfour:
	mov d, a
	mov a, c
	rlc 
	add c
	add d
	mov d, a
else:
	inr c
	mov a, b
	rlc 
	mov b, a 
	mov a, c
	cpi 0x02 
	jnz loop
	lxi h, 0x0bfa
clear:
	mvi m, 0x00
	inx h
	mov a, h
	cpi 0x0c
	jnz clear
	lxi h, 0x0bfa
	m1:
	mov a, d
	cpi 0x00
	jz m2
	mvi m, 0x5c
	inx h
	dcr d
	jmp m1
	m2:
	call 0x01c8
	jmp inf
)";

EditorView::EditorView()
	: m_Editor(),
	m_Compiler(),
	m_Disassembler()
{
	m_Open = true;

	m_Editor.SetShowWhitespaces(false);

	m_Editor.SetLanguageDefinition(GetLanguageDefinition());
	m_Editor.SetPalette(GetDefaultPalette());	

	m_Editor.SetText(g_DebugProgramSrc);
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
	static const char* fileToEdit = "C:/Users/Kamil/Desktop/source_code.asm";

	ImGui::Begin("Editor", &m_Open, ImGuiWindowFlags_HorizontalScrollbar);

	auto& style = ImGui::GetStyle();
	ImVec2 editor_size = ImGui::GetContentRegionAvail();
	editor_size.y -= ImGui::GetFontSize() + style.CellPadding.y;

	m_Editor.Render("Editor", editor_size);
	ImGui::Text("%d:%-6d", cpos.mLine + 1, cpos.mColumn + 1);
	ImGui::SameLine();
	ImGui::Text("%s%s", fileToEdit, m_Editor.CanUndo() ? "*" : " ");

	m_Editor.MoveHome();
	
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

TextEditor::LanguageDefinition EditorView::GetLanguageDefinition()
{
	TextEditor::LanguageDefinition langDef;

	static const char* const keywords[] = {
			"A", "B", "C", "D", "E", "F", "H", "L", "M", "PSW", "BC", "DE", "HL"
	};

	for (auto& k : keywords)
		langDef.mKeywords.insert(k);

	static const char* const identifiers[] = {
		"ADD", "ADI", "ADC", "ACI", "ANA", "ANI", "CALL", "CZ", "CNZ", 
		"CP", "CM", "CC", "CNC", "CPE", "CPO", "CMA", "CMC", "CMP", 
		"CPI", "DAA", "DAD", "DCR", "DCX", "DI", "EI", "HLT", "IN", 
		"INR", "INX", "JMP", "JZ", "JNZ", "JP", "JM", "JC", "JNC", 
		"JPE", "JPO", "LDA", "LDAX", "LHLD", "LXI", "MOV", "MVI", 
		"NOP", "ORA", "ORI", "OUT", "PCHL", "POP", "PUSH", "RAL", 
		"RAR", "RLC", "RRC", "RIM", "RET", "RZ", "RNZ", "RP", "RM", 
		"RC", "RNC", "RPE", "RPO", "RST0", "RST1", "RST2", "RST3", 
		"RST4", "RST5", "RST6", "RST7", "SIM", "SPHL", "SHLD", "STA", 
		"STAX", "STC", "SUB", "SUI", "SBB", "SBI", "XCHG", "XTHL", "XRA", "XRI", };

	for (auto& k : identifiers)
	{
		TextEditor::Identifier id;
		id.mDeclaration = "Built-in function";
		langDef.mIdentifiers.insert(std::make_pair(std::string(k), id));
	}

	langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("0[xX][0-9a-fA-F]*", TextEditor::PaletteIndex::Number));
	langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*[:]$", TextEditor::PaletteIndex::Breakpoint));
	langDef.mTokenRegexStrings.push_back(std::make_pair<std::string, TextEditor::PaletteIndex>("[a-zA-Z][a-zA-Z0-9]*", TextEditor::PaletteIndex::Identifier));

	langDef.mCommentStart = "\n";
	langDef.mCommentEnd = "\n";
	langDef.mSingleLineComment = ";";

	langDef.mCaseSensitive = false;
	langDef.mAutoIndentation = false;

	langDef.mName = "KR580";

	return langDef;
}

TextEditor::Palette EditorView::GetDefaultPalette()
{
	const TextEditor::Palette palette = { {
			0xff7f7f7f,	// Default
			0xffd69c56,	// Keyword	
			0xff4d9bc6,	// Number
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
			0xff9b4dc6, // Breakpoint
			0xff707000, // Line number
			0x40000000, // Current line fill
			0x40808080, // Current line fill (inactive)
			0x40a0a0a0, // Current line edge
		} };

	return palette;
}
