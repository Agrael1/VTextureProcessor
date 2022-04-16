#include <Logic/CodeTransformer.h>
#include <utils/utils.h>


struct Context
{
	Context(bool is_simple, std::wstring_view code) : is_simple(is_simple), code(code) {	}
public:
	void write(token i) {
		out += code.substr(prev_off, i.offset - prev_off);
		prev_off = i.offset + i.value.length();
	}
public:
	std::wstring out;
	std::wstring_view code;
	size_t prev_off = 0;
	size_t block = 0;
	token::type prev = token::type::tt_none;

	const bool is_simple;
	bool voidify = false;
};

CodeTransformer::CodeTransformer(std::unordered_set<std::wstring> xtypes, std::unordered_set<std::wstring> xmacros, std::unordered_map<std::wstring, size_t> xfuncs)
	:types(std::move(xtypes)), macros(std::move(xmacros)), funcs(std::move(xfuncs))
{
}

CodeTransformer& CodeTransformer::SetOutputs(std::unordered_set<std::wstring> xouts)
{
	outs = std::move(xouts);
	return *this;
}
CodeTransformer& CodeTransformer::SetInputs(std::unordered_map<std::wstring, input_info> xinputs)
{
	inputs = std::move(xinputs);
	return *this;
}

CodeTransformer& CodeTransformer::SetCBuf(std::unordered_map<std::wstring, std::wstring> xcbuffer)
{
	cbuffer = std::move(xcbuffer);
	return *this;
}

std::wstring CodeTransformer::ReformTexture2D(ver::generator<token>& gen, std::wstring_view node_name, size_t& out)
{
	using enum token::type;
	std::wstring o;
	input_info* ii = nullptr;

	for (auto& i : gen)
	{
		std::wstring c{ i.value };
		switch (i.xtype)
		{
		case close_br:
			if(ii)o += std::format(L").{}", ii->output);
			out = i.offset + 1;
			return o;
		case comma:
			if (ii && !ii->complex)
			{
				out = i.offset + 1;
				return o;
			}
			break;
		case identifier:
			if (auto it = inputs.find(c); it!= inputs.end())
			{
				ii = std::addressof(it->second);
				o += std::format(L"{}_main(", ii->node);
				break;
			}
			o+= i.value;
			break;
		default:
			break;
		}
	}
	return o;
}

std::wstring CodeTransformer::Transform(std::wstring_view node_name, std::wstring_view code)
{
	using enum token::type;
	Context ctx(outs.size() == 1, code);
	std::wstring_view style_name = node_name.substr(0, node_name.find_last_of('_'));
	bool pass_args = false;
	bool is_main = false;


	auto gen = GetToken(code);

	for (auto& i : gen)
	{
		std::wstring c{ i.value };
		switch (i.xtype)
		{
		case open_cbr:
			if (is_main && !ctx.block)
			{
				ctx.write(i);
				ctx.out += std::format(L"{{\n\t{} {};",
					ctx.is_simple ? L"vec4" : style_name,
					ctx.is_simple ? *outs.begin() : L"sv_output");
			}
			ctx.block++; break;
		case close_cbr:
			if (is_main && ctx.block == 1)
			{
				ctx.write(i);
				ctx.out += std::format(L"\treturn {};\n}}",
					ctx.is_simple ? *outs.begin() : L"sv_output");
				is_main = false;
			}
			ctx.block--; break;
		case open_br:
			if (pass_args)
			{
				pass_args = false;
				ctx.prev_off++;
				ctx.out += L"(vec2 sv_texc";
			}
			break;
		case keyword:
			if (c == L"void")
			{
				ctx.write(i);
				ctx.voidify = true;
			}
			break;
		case identifier:
			if (macros.contains(c) || types.contains(c))
			{
				ctx.write(i);
				ctx.out += std::format(L"{}_{}", node_name, i.value);
				break;
			}
			if (!ctx.is_simple && outs.contains(c))
			{
				ctx.write(i);
				ctx.out += std::format(L"sv_output.{}", i.value);
				break;
			}
			if (funcs.contains(c) && ctx.prev != tt_point)
			{
				ctx.write(i);
				pass_args = i.value == L"main" && !ctx.block;
				if (pass_args)
				{
					is_main = pass_args;
					ctx.out += std::format(L"{} {}_main", ctx.is_simple ? L"vec4" : style_name, node_name);
					ctx.voidify = false;
					break;
				}
				if (ctx.voidify) { ctx.out += L"void "; ctx.voidify = false; }
				ctx.out += std::format(L"{}_{}", node_name, i.value);
				break;
			}
			if (cbuffer.contains(c))
			{
				ctx.write(i);
				ctx.out += cbuffer[c];
				break;
			}
			if (i.value == L"texture2D")
			{
				ctx.write(i);
				ctx.out += ReformTexture2D(gen, node_name, ctx.prev_off);
			}
			break;
		default:
			break;
		}
		ctx.prev = i.xtype;
	}
	ctx.out += code.substr(ctx.prev_off);
	ctx.out += L"\n\n//-------------------------------------------------------//\n\n";
	return ctx.out;
}

std::wstring CodeTransformer::EntryPoint(std::wstring_view node_name)
{
	return std::format(L"{}_main", node_name);
}
