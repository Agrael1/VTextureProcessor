#pragma once
#include <Editor/Lexer.h>
#include <unordered_set>
#include <unordered_map>

struct input_info
{
	std::wstring node;
	std::wstring output;
	bool complex = false;
};

class CodeTransformer
{
public:
	CodeTransformer(std::unordered_set<std::wstring> types,
	std::unordered_set<std::wstring> macros,
	std::unordered_map<std::wstring, size_t> funcs);
public:
	CodeTransformer& SetOutputs(std::unordered_set<std::wstring> outs);
	CodeTransformer& SetInputs(std::unordered_map<std::wstring, input_info> xinputs);
	CodeTransformer& SetCBuf(std::unordered_map<std::wstring, std::wstring> cbuffer);
	std::wstring ReformTexture2D(ver::generator<token>& gen, std::wstring_view node_name, size_t& out);
	std::wstring Transform(std::wstring_view node_name, std::wstring_view code);
	std::wstring EntryPoint(std::wstring_view node_name);
private:
	std::unordered_set<std::wstring> types;
	std::unordered_set<std::wstring> macros;
	std::unordered_set<std::wstring> outs;
	std::unordered_map<std::wstring, std::wstring> cbuffer;
	std::unordered_map<std::wstring, size_t> funcs;
	std::unordered_map<std::wstring, input_info> inputs;
};