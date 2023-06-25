#include "EditorView.h"
#include "KR580.h"

#include <string>
#include <sstream>
#include <fstream>


// Debug only
#if 1
static std::string g_DebugProgramSrc = R"(Init:
mvi a, 0x00
mvi b, 0x00
mvi c, 0x01
lxi d, SnakeTable
call ClearDisplay
call PrintCounter

Loop:
	call SnakeDisplay
	mov a, c
	out 0x05
	cpi 0x80
	cz IncrementDisplay
	rlc
	mov c, a
	jmp Loop

; Display content of B register to two last slots of display and increments B register
IncrementDisplay:
	push psw
	mvi a, 0x00
	out 0x05
	mov a, b
	adi 0x01
	call PrintCounter
	mov b, a
	pop psw
	ret

; Returns only lower 4 bits of Accum in Accum
GetLower:
	ani 0x0f
	ret

; Returns only high 4 bits of Accum in Accum
GetHigh:
	ani 0xf0
	rrc
	rrc
	rrc
	rrc
	ret

; Returns digit representation of Accum through table at 0x0900
GetDigitRepresentation:
	push h
	push b

	lxi h, DigitRepresentationTable
	mov b, a
	mov a, l
	add b
	mov l, a
	
	mov a, m

	pop b
	pop h
	ret

; Display content of A register to last two slots of display
PrintCounter:
	push psw	
	push h
	push b

	lxi h, 0x0BFF

	mov b, a
	call GetLower
	call GetDigitRepresentation
	mov m, a

	lxi h, 0x0BFE

	mov a, b
	call GetHigh
	call GetDigitRepresentation
	mov m, a
	
	call DISPLAY
	
	pop b
	pop h
	pop psw
	ret

SnakeDisplay:
	push psw	
	push h

	lxi h, 0x0bfa

	ldax d
	mov m, a
	inx h
	inx d

	ldax d
	mov m, a
	inx h
	inx d

	ldax d
	mov m, a
	inx h
	inx d

	ldax d
	mov m, a
	inx h
	inx d

	lxi h, SnakeTableEnd
	mov a, h
	cmp d
	jnz _SnakeDisplayExit
	mov a, l
	cmp e
	jnz _SnakeDisplayExit
	lxi d, SnakeTable
	
	_SnakeDisplayExit:
	call DISPLAY	
	pop h
	pop psw
	ret

ClearDisplay:
	push psw
	push h
	
	mvi a, 0x00
	
	lxi h, 0x0BFA
	mov m, a

	lxi h, 0x0BFB
	mov m, a

	lxi h, 0x0BFC
	mov m, a

	lxi h, 0x0BFD
	mov m, a

	lxi h, 0x0BFE
	mov m, a

	lxi h, 0x0BFF
	mov m, a

	call DISPLAY
	pop h
	pop psw
	ret

DigitRepresentationTable:
	dw 0x3f
	dw 0x06
	dw 0x5b
	dw 0x4f
	dw 0x66
	dw 0x6d
	dw 0x7d
	dw 0x07
	dw 0x7f
	dw 0x6f
	dw 0x77
	dw 0x7c
	dw 0x39
	dw 0x5e
	dw 0x79
	dw 0x71

SnakeTable:
	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x00
	dw 0x40
	dw 0x40
	dw 0x00

	dw 0x40
	dw 0x40
	dw 0x40
	dw 0x40

	dw 0x70
	dw 0x40
	dw 0x40
	dw 0x46

	dw 0x79
	dw 0x40
	dw 0x40
	dw 0x4f

	dw 0x79
	dw 0x49
	dw 0x49
	dw 0x4f

	dw 0x7f
	dw 0x49
	dw 0x49
	dw 0x7f

	dw 0x7f
	dw 0x79
	dw 0x4f
	dw 0x7f

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f

	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f

	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f

	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x00
	dw 0x00
	dw 0x00
	dw 0x00

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f

	dw 0x7f
	dw 0x7f
	dw 0x7f
	dw 0x7f
SnakeTableEnd:


)";
#else 
static std::string g_DebugProgramSrc = R"(inf:
mvi b, f7
mvi c, 00
mvi d, 00
loop:
mov a, b
out 07
in 06
cpi ff
jz else
xri ff
cpi 04
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
cpi 02 
jnz loop
lxi h, 0bfa
clear:
mvi m, 00
inx h
mov a, h
cpi 0c
jnz clear
lxi h, 0bfa
m1:
mov a, d
cpi 00
jz m2
mvi m, 5c
inx h
dcr d
jmp m1
m2:
call 01c8
jmp inf
)";
#endif

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
	//static const char* fileToEdit = "C:/Users/Kamil/Desktop/source_code.asm";

	ImGui::Begin("Editor", &m_Open, ImGuiWindowFlags_HorizontalScrollbar);

	auto& style = ImGui::GetStyle();
	ImVec2 editor_size = ImGui::GetContentRegionAvail();
	editor_size.y -= ImGui::GetFontSize() + style.CellPadding.y;

	m_Editor.Render("Editor", editor_size);
	ImGui::Text("%d:%-6d", cpos.mLine + 1, cpos.mColumn + 1);
	//ImGui::SameLine();
	//ImGui::Text("%s%s", fileToEdit, m_Editor.CanUndo() ? "*" : " ");
	
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
