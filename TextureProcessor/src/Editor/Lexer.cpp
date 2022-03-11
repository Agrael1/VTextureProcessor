#include <Editor/Lexer.h>
#include <ranges>
#include <optional>
#include <format>

class LexContext
{
	static constexpr auto sr = statements_range();
	static constexpr auto kr = kwords_range();

public:
	LexContext(std::wstring_view code)
		:code(code) {}
public:
	wchar_t fetch_one()noexcept
	{
		wchar_t x = prefetch_one();
		if (!x)return 0;
		advance_one();
		if (x == '\n') { line++; column = 0; }
		return x;
	}
	std::optional<token> try_get_statement(wchar_t c)
	{
		if (std::ranges::find(sr, c) == sr.end())return{};
		switch (c)
		{
		case 'b':
			return stmt_from_subset(s_break);
		case 'c':
			c = prefetch_one();
			if (c == 'a')
				return stmt_from_subset(s_case, 2);
			if (c == 'o')
				return stmt_from_subset(s_continue, 2);
			break;
		case 'd':
			c = prefetch_one();
			if (c == 'o')
				return create_advance(statements[s_do], token::type::statement);
			if (c == 'e')
				return stmt_from_subset(s_default, 2);
			if (c == 'i')
				return stmt_from_subset(s_discard, 2);
			break;
		case 'e':
			return stmt_from_subset(s_else);
		case 'f':
			return stmt_from_subset(s_for);
		case 'i':
			c = prefetch_one();
			if (c == 'f')
				return create_advance(statements[s_if], token::type::statement);
			if (c == 'n')
			{
				if (auto st = create_advance(statements[s_in], token::type::statement))
					return st;
				return stmt_from_subset(s_inout, 2);
			}
			break;
		case 'l':
			return stmt_from_subset(s_layout);
		case 'o':
			return stmt_from_subset(s_out);
		case 'r':
			return stmt_from_subset(s_return);
		case 's':
			return stmt_from_subset(s_switch);
		case 'w':
			return stmt_from_subset(s_while);
		default:
			break;
		}
		return{};
	}
	std::optional<token> try_get_kw(wchar_t c)
	{
		if (std::ranges::find(kr, c) == kr.end())return{};
		switch (c)
		{
		case L'a':
			return kw_from_subset(k_attribute);
		case L'b':
			switch (prefetch_one())
			{
			case 'o':return kw_from_subset(k_bool, 2);
			case 'v':return try_create_vec(k_bvec2);
			default:return{};
			}
		case L'c':
			return kw_from_subset(k_const);
		case L'd':
			switch (prefetch_one())
			{
			case 'o':return kw_from_subset(k_double, 2);
			case 'm':return try_create_mat(k_dmat2);
			case 'v':return try_create_vec(k_dvec2);
			default:return{};
			}
		case 'f':
			c = prefetch_one();
			if (c == L'a')
				return kw_from_subset(k_false, 2);
			if (c == L'l')
				return kw_from_subset(k_float, 2);
			break;
		case 'i':
			switch (prefetch_one())
			{
			case 'n':return kw_from_subset(k_int, 2);
			case 's':return try_create_isampler(k_isampler1D);
			case 'v':return try_create_vec(k_ivec2);
			default:return{};
			}
		case 'm':
			return try_create_mat(k_mat2, 1);
		case 'u':
			switch (prefetch_one())
			{
			case 'i':return kw_from_subset(k_uint, 2);
			case 's':return try_create_isampler(k_usampler1D);
			case 'n':return kw_from_subset(k_uniform, 2);
			case 'v':return try_create_vec(k_uvec2);
			default:return{};
			}
		case 's':
			switch (prefetch_one())
			{
			case 'a':return{};//sampler
			case 't':return kw_from_subset(k_struct, 2);
			default:return{};
			}
		case 't':
			return kw_from_subset(k_true);
		case 'v':
			switch (prefetch_one())
			{
			case 'a':return kw_from_subset(k_varying, 2);
			case 'e':return try_create_vec(k_vec2, 1);
			case 'o':return kw_from_subset(k_void, 2);
			default:return{};
			}
		default:
			break;
		}

		return {};
	}
	std::optional<token> try_get_ident(wchar_t c)
	{
		size_t roffset = offset - 1;
		const wchar_t* av = code.data() - 1;
		while (!not_lit(prefetch_one()))fetch_one();
		return token{ token::type::identifier, roffset, {av, offset - roffset} };
	}
	token make_comment()
	{
		size_t roffset = offset - 2;
		const wchar_t* av = code.data() - 2;
		wchar_t c = fetch_one();

		while (c != '\n' && c != '\0')
			c = fetch_one();

		return token{ token::type::comment, roffset, {av, offset - roffset} };
	}
	bool empty()const noexcept {
		return code.empty() || !code[0];
	}
	static bool not_lit(wchar_t c) {
		return !iswalnum(c) && c != L'_';
	}
	auto Offset()const noexcept
	{
		return offset;
	}
private:
	wchar_t prefetch_one()const noexcept
	{
		if (empty())return 0;
		return code[0];
	}
	wchar_t prefetch_n(size_t n)const noexcept
	{
		if (empty() || code.length() <= n)return 0;
		return code[n];
	}
	bool term(size_t length)const noexcept {
		return code.length() <= (length) || not_lit(code[length]);
	}

	std::optional<token> stmt_from_subset(size_t stateN, size_t xoffset = 1) {
		return from_subset(xoffset, statements[stateN], token::type::statement);
	}
	std::optional<token> kw_from_subset(size_t stateN, size_t xoffset = 1) {
		return from_subset(xoffset, kwords[stateN], token::type::keyword);
	}
	std::optional<token> from_subset(size_t xoffset, std::wstring_view xst, token::type tt)
	{
		auto cmpw = xst.substr(xoffset);
		if (cmpw == code.substr(xoffset - 1, cmpw.length()))
			return create_advance(xst, tt);
		return{};
	}
	std::optional<token> create_advance(std::wstring_view sw, token::type tt)
	{
		if (term(sw.length() - 1))
		{
			auto r = offset - 1;
			advance(sw);
			return token{ tt, r, sw };
		}
		return {};
	}
	std::optional<token> try_create_vec(e_kwd vec, size_t offset = 0)
	{
		if (code.substr(0, 3 - offset) == L"vec" + offset)
		{
			wchar_t c = prefetch_n(3 - offset);//2 3 4
			if (!(c > L'1' && c < L'5'))return{};
			return create_advance(kwords[c - L'2' + vec], token::type::keyword);
		}
		return {};
	}
	std::optional<token> try_create_mat(e_kwd mat, size_t offset = 0)
	{
		auto roffset = 3 - offset;
		if (code.substr(0, roffset) == L"mat" + offset)
		{
			wchar_t c = prefetch_n(roffset);
			if (!(c > L'1' && c < L'5'))return{};
			auto it = std::span<const std::wstring_view>(kwords.begin() + (c - L'2') * 4 + mat, 4);
			c = prefetch_n(roffset + 1);
			if (not_lit(c))return create_advance(it[0], token::type::keyword);
			if (c != L'x')return{};
			c = prefetch_n(roffset + 2);
			if (!(c > L'1' && c < L'5'))return{};
			return create_advance(it[c - L'1'], token::type::keyword);
		}
		return {};
	}
	std::optional<token> try_create_isampler(e_kwd smpl)
	{
		if (code.substr(0, 7) == L"sampler") //sampler
		{
			wchar_t c = prefetch_n(7);
			switch (c)
			{
			case 'B':
				return kw_from_subset(smpl + 8, 7);
			case 'C':
				if (code.substr(7, 4) != kwords[k_isamplerCube].substr(8, 4))
					return{}; // Cube
				if (prefetch_n(11) == 'A')
					return kw_from_subset(smpl + 10, 11);
				return create_advance(kwords[smpl + 9], token::type::keyword);
			case '1':
				if (prefetch_n(8) != 'D')return{};
				if (prefetch_n(9) == 'A')
					return kw_from_subset(smpl + 1, 9);
				return create_advance(kwords[smpl], token::type::keyword);
			case '2':
				if (prefetch_n(8) != 'D')return{};
				if (prefetch_n(9) == 'A')
					return kw_from_subset(smpl + 3, 9);
				if (code.substr(9, 2) == L"MS")
				{
					if (prefetch_n(11) == 'A')
						return kw_from_subset(smpl + 5, 12);
					return create_advance(kwords[smpl + 4], token::type::keyword);
				}
				if (prefetch_n(9) == 'R')
					return kw_from_subset(smpl + 6, 9);
				return create_advance(kwords[smpl + 2], token::type::keyword);
			case '3':
				return kw_from_subset(smpl + 7, 8);
			default:
				return{};
			}
		}
		return {};
	}
	void advance(std::wstring_view sw)
	{
		auto l = sw.length() - 1; //without 1 byte
		offset += l;
		line += l;
		code = code.substr(l);
	}
	void advance_one()
	{
		offset++;
		line++;
		code = code.substr(1);
	}

private:
	size_t line = 0;
	size_t column = 0;
	size_t offset = 0;

	std::wstring_view code;
};


static std::optional<token> GetTokenLoop(LexContext& lex)
{
	while (!lex.empty())
	{
		wchar_t c = lex.fetch_one();
		if (iswdigit(c))
			continue;

		switch (c)
		{
		case '/':
			switch (lex.fetch_one())
			{
			case '/': return lex.make_comment();
			default: break;
			}
			continue;
		case '[':
			return token{token::type::open_sq, lex.Offset() - 1};
		case ']':
			return token{token::type::close_sq, lex.Offset() - 1};
		case '(':
			return token{ token::type::open_br, lex.Offset() - 1 };
		case ')':
			return token{ token::type::close_br, lex.Offset() - 1 };
		case '{':
			return token{ token::type::open_cbr, lex.Offset() - 1 };
		case '}':
			return token{ token::type::close_cbr, lex.Offset() - 1 };
		default:
			break;
		}

		if (LexContext::not_lit(c))
			continue;
		if (auto st = lex.try_get_statement(c); st)
			return *st;
		if (auto st = lex.try_get_kw(c); st)
			return *st;
		if (auto st = lex.try_get_ident(c); st)
			return *st;
	}
	return{};
}
ver::generator<token> GetToken(std::wstring_view code)
{
	LexContext lex{ code }; std::optional<token> st;
	while (st = GetTokenLoop(lex))
		co_yield *st;
}

