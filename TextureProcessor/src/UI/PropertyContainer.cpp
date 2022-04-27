#include <UI/PropertyContainer.h>
#include <QCheckBox>
#include <QGroupBox>
#include <ranges>

template<ver::dc::Type ty>
class Editable :public UI::IEditorOption
{
	using map_t = ver::dc::Map<ty>;
	using internal_t = map_t::SysType;
	using option_t = map_t::param;
	static_assert(map_t::valid, "Invalid type supplied");
public:
	struct Def :public QGroupBox
	{
		using validator = std::conditional_t<ver::dc::integral_core<ty>, QIntValidator, QDoubleValidator>;
		using underlying_t = std::conditional_t<ver::dc::integral_core<ty>, int32_t, float>;
	public:
		Def(ver::dc::Options& opt)
		{
			opt.enable_def = true;
			setTitle("Default:");
			mat.setContentsMargins(0, 0, 0, 0);
			FillLayout(opt);
			setLayout(&mat);
		}
		void FillLayout(ver::dc::Options& opt)
		{
			for (size_t i = 0; i < map_t::floats / ver::dc::GetStride<ty>(); i++)
				for (size_t j = 0; j < ver::dc::GetStride<ty>(); j++)
				{
					auto le = new QLineEdit;
					le->setValidator(&dv);
					auto& x = reinterpret_cast<underlying_t*>(std::addressof(opt.param.get<ty>().def))[i * ver::dc::GetStride<ty>() + j];

					le->setText(QString::number(x));
					connect(le, &QLineEdit::textChanged, [&x](const QString& a) {
						if constexpr (ver::dc::integral_core<ty>)
						{
							x = a.toInt();
							return;
						}
						x = static_cast<float>(a.toDouble());
						});
					mat.addWidget(le, i, j);
				}
		}
	private:
		QGridLayout mat;
		validator dv;
	};
	struct MinMax : public QGroupBox
	{
		using validator = std::conditional_t<ver::dc::integral_core<ty>, QIntValidator, QDoubleValidator>;
		using underlying_t = std::conditional_t<ver::dc::integral_core<ty>, int32_t, float>;
	public:
		MinMax(ver::dc::Options& opt, bool min = true)
		{
			vl.setContentsMargins(0, 0, 0, 0);
			setTitle(min ? "Min:" : "Max:");
			setCheckable(true);

			setChecked(min ? opt.enable_min : opt.enable_max);

			connect(this, &QGroupBox::toggled, [min, &opt](int state) {(min ? opt.enable_min : opt.enable_max) = state; });

			for (size_t j = 0; j < map_t::floats; j++)
			{
				auto le = new QLineEdit;
				le->setValidator(&dv);
				auto& r = opt.param.get<ty>();
				auto& x = reinterpret_cast<underlying_t*>(std::addressof(min ? r.min : r.max))[j];

				le->setText(QString::number(x));
				connect(le, &QLineEdit::textChanged, [&x](const QString& a) {
					if constexpr (ver::dc::integral_core<ty>)
					{
						x = a.toInt();
						return;
					}
					x = static_cast<float>(a.toDouble());
					});
				vl.addWidget(le);
			}
			setLayout(&vl);
		}
	private:
		QVBoxLayout vl;
		validator dv;
	};
public:
	Editable(ver::dc::Options* xopt)
	{
		gl.setContentsMargins(0, 0, 0, 0);
		if (xopt)opt = *xopt;
		QWidget* def = new Def(opt);
		gl.addWidget(def, 0, 0, 1, -1);

		if constexpr (ver::dc::has_min<ty>)
			gl.addWidget(new MinMax(opt), 1, 0);
		if constexpr (ver::dc::has_max<ty>)
			gl.addWidget(new MinMax(opt, false), 1, 1);
		setLayout(&gl);
	}
public:
	ver::dc::Options& GetOption() noexcept override
	{
		return opt;
	}
private:
	QGridLayout gl;
private:
	ver::dc::Options opt{ ty };
};

std::unique_ptr<UI::IEditorOption> make_widget(ver::dc::Type ty, ver::dc::Options* opt)
{
	switch (ty)
	{
#define X(a) case ver::dc::Type::a: return std::make_unique<Editable<ver::dc::Type::a>>(opt);
		LEAF_ELEMENT_TYPES
#undef X
	}
	return nullptr;
}


UI::PropertyContainer::PropertyContainer(ver::dc::Layout buffer, std::span<ver::dc::Options> params)
{
	add.setIconSize({ 24,24 });
	add.setIcon(QIcon{ ":/icons8-add-property.png" });
	hl.setAlignment(Qt::AlignRight);
	vl.setContentsMargins(0, 0, 0, 0);
	hl.addWidget(&add);
	vl.addLayout(&hl);

	add.connect(&add, &QToolButton::pressed, [this]() {
		auto& x = editors.emplace_back(std::make_unique<Editor>(editors.size()));
		x->SetDesctructCallback([this](size_t x) {RemoveElement(x); });
		vl.addWidget(x.get());
		}
	);

	auto xlineA = new QFrame;
	xlineA->setFrameShape(QFrame::HLine);
	xlineA->setFrameShadow(QFrame::Sunken);
	vl.addWidget(xlineA);


	editors.reserve(buffer.count());
	auto it = params.begin();
	ver::dc::Options* opt = nullptr;
	for (size_t r = 0; auto & i : buffer.Get())
	{
		opt = std::addressof(*it);

		auto& x = editors.emplace_back(std::make_unique<Editor>(i, opt, r));
		x->SetDesctructCallback([this](size_t x) {RemoveElement(x); });
		vl.addWidget(x.get());
		if (opt) { it++; }
		r++;
	}

	auto lineA = new QFrame;
	lineA->setFrameShape(QFrame::HLine);
	lineA->setFrameShadow(QFrame::Sunken);

	vl2.setContentsMargins(0, 0, 0, 0);
	vl2.addLayout(&vl);
	vl2.addWidget(&saver);
	vl2.addWidget(lineA);
	setLayout(&vl2);
}
UI::PropertyContainer::~PropertyContainer()
{

}

std::vector<ver::dc::Options> UI::PropertyContainer::GatherOptions()
{
	std::vector<ver::dc::Options> out;
	out.reserve(editors.size());
	for (auto & i : editors)
	{
		auto* x = i->GetOption();
		if (!x)continue;
		out.emplace_back(*x);
	}
	return out;
}

ver::dc::Layout UI::PropertyContainer::GatherLayout()
{
	ver::dc::Layout out;
	for (auto& i : editors)
		out.Add(i->GetEntry());
	return out;
}

bool UI::PropertyContainer::Accept()
{
	bool x = true;
	for (auto& i : editors)
		x &= i->Accept();
	return x;
}

void UI::PropertyContainer::RemoveElement(size_t xposition)
{
	editors.erase(editors.begin() + xposition);
	for (auto& i : std::views::drop(editors, xposition))
		i->UpdatePosition();
}

UI::Editor::Editor(size_t position)
	:opt(nullptr), position(position)
{
	type.addItem("Empty");
	std::ranges::for_each(ver::dc::type_strings, [&](auto x) {type.addItem(x); });
	Init();
}
UI::Editor::Editor(const ver::dc::Layout::Entry& entry, ver::dc::Options* xopt, size_t position)
	:opt(xopt), position(position)
{
	type.addItem(QStringLiteral("Empty"));
	std::ranges::for_each(ver::dc::type_strings, [&](auto x) {type.addItem(x); });
	type.setCurrentIndex(int(entry.second.Get()));
	code_name.setText(entry.first.c_str());
	name.setText(opt && opt->enable_alias ? opt->alias.c_str() : entry.first.c_str());
	Init();
}

ver::dc::Options* UI::Editor::GetOption() const noexcept
{
	auto n = name.text();
	auto cn = code_name.text();
	ver::dc::Options* o = !editor?opt:&editor->GetOption();
	if (!o)return nullptr;

	o->enable_alias = n != cn && !n.isEmpty();
	if (o->enable_alias)
		o->alias = n.toStdString();

	if (!o->flags)return nullptr;
	return o;
}
ver::dc::Layout::Entry UI::Editor::GetEntry() const noexcept
{
	return ver::dc::Layout::Entry{ code_name.text().toStdString(), ver::dc::Type(type.currentIndex()) };
}
bool UI::Editor::Accept()
{
	bool b = code_name.text().isEmpty();
	if (b) code_name.setStyleSheet("border: 1px solid red");
	bool c = type.currentIndex();
	//no empty entry
	return !b && c;
}
void UI::Editor::Init()
{
	hl.addWidget(&type);
	remove.setIcon(QIcon{ ":/icon_window_close.png" });
	hl.addWidget(&remove);
	vl.addLayout(&hl);

	vl.addWidget(&lName);
	vl.addWidget(&name);
	vl.addWidget(&lcname);
	code_name.setValidator(&rev);
	vl.addWidget(&code_name);
	edit.setFixedSize({ 24,24 });
	edit.setIconSize({ 24,24 });
	edit.setIcon(QIcon{ ":/icons8-edit-prop.png" });

	vl.addWidget(&edit);

	edit.connect(&edit, &QToolButton::pressed, [this]() {
		edit.hide();
		editor = make_widget(ver::dc::Type(type.currentIndex()), opt);
		if (editor)vl.addWidget(editor.get());
		});
	type.connect(&type, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index)
		{
			edit.show();
			editor = make_widget(ver::dc::Type(type.currentIndex()), nullptr);
			opt = nullptr;
		});
	auto xlineA = new QFrame;
	xlineA->setFrameShape(QFrame::HLine);
	xlineA->setFrameShadow(QFrame::Sunken);
	vl.addWidget(xlineA);
	setLayout(&vl);
}

UI::PropertyContainer::Saver::Saver()
{
	save.setFixedSize({ 32, 32 });
	save.setIconSize({ 32, 32 });
	discard.setFixedSize({ 32, 32 });
	discard.setIconSize({ 32, 32 });
	save.setIcon(QIcon{ ":/icons8-save.png" });
	discard.setIcon(QIcon{ ":/icons8-discard.png" });
	hl.addWidget(&save);
	hl.addWidget(&discard);
	hl.setContentsMargins(0, 0, 11, 0);
	hl.setAlignment(Qt::AlignRight);

	connect(&discard, &QToolButton::pressed, [this]() {if (c_discard)c_discard(); });
	connect(&save, &QToolButton::pressed, [this]() {if (c_save)c_save(); });
	setLayout(&hl);
}
