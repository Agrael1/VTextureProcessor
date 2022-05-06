#pragma once

namespace ver::detail
{
	enum Format
	{
		statements,
		kwords,
		comment,
		user_type,
		variable,
		function,
		str_lit,
		macro_lit,
		num_lit,
		define,
		cbuf,
		builtin,
		size
	};
}