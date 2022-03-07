#include "EditorView.h"

static const char* testProgram = "LXI H, 0800\n"
"MVI B, 55\n"
"LOOP:\n"
"MOV A, B\n"
"MOV M, A\n"
"HELLO\n"
"RLC\n"
"INX H\n"
"MOV B, A\n"
"MVI A, 0A\n"
"CMP H\n"
"JNZ LOOP\n"
"MVI A, 70\n"
"CMP L\n"
"JNZ LOOP\n"
"RST1\n"
"\n"
"NOP\n"
"NOP\n"
"NOP\n"
"NOP\n"
"NOP\n"
"NOP\n"
"NOP\n"
"NOP\n"
"NOP\n"
"\n"
"LXI H, 0800\n"
"MVI B, 55\n"
"LOOP2:\n"
"MOV A, B\n"
"CMP M\n"
"JNZ FAIL\n"
"INX H\n"
"RLC\n"
"MOV B, A\n"
"MVI A, 0A\n"
"CMP H\n"
"JNZ LOOP2\n"
"MVI A, 70\n"
"CMP L\n"
"JNZ LOOP2\n"
"JMP SUCCESS\n"
"\n"
"SUCCESS:\n"
"CALL 05BA\n"
"JMP EXIT\n"
"\n"
"\n"
"FAIL:\n"
"PUSH H\n"
"CALL 01E9\n"
"CALL 01C8\n"
"CALL PRINT\n"
"POP H\n"
"JMP FAIL\n"
"\n"
"PRINT:\n"
"XCHG                   ; (DE) <--> (HL)\n"
"LXI H, 0BF0       ; (HL) <-- 0BF0\n"
"LDAX D               ; (A) <-- ((DE))\n"
"CALL SPLIT\n"
"MOV A, E\n"
"CALL SPLIT\n"
"MOV A, D\n"
"CALL SPLIT\n"
"JMP LOOP\n"
"\n"
"SPLIT:\n"
"MOV B, A\n"
"ANI 0F                 ; (A)    <-- (A) & 0x0F\n"
"MOV M, A            ; ((HL)) <-- (A)\n"
"INX H                   ; (HL)++\n"
"MOV A, B\n"
"RRC                      ; (A) <-- (A) >> 1\n"
"RRC\n"
"RRC\n"
"RRC\n"
"ANI 0F                 ; (A)    <-- (A) & 0x0F\n"
"MOV M, A            ; ((HL)) <-- (A)\n"
"INX H\n"
"RET\n"
"\n"
"EXIT:\n"
"RST1\n";

EditorView::EditorView()
	: m_Editor()
{
	m_Open = true;

	m_Editor.SetShowWhitespaces(false);
	TextEditor::ErrorMarkers markers;
	markers.insert(std::make_pair<int, std::string>(6, "Unknown operation"));
	m_Editor.SetErrorMarkers(markers);

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
		id.mDeclaration = "Built-in";
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
			0xff101010, // Background
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

	auto cpos = m_Editor.GetCursorPosition();
	ImGui::Begin("Editor", &m_Open);

	static const char* filename = "Hello world.cpp";

	m_Editor.Render("Editor");

	ImGui::End();
}
