
class ContextMenu : public QMenu
{
	friend class MenuBuilder;
public:
	ContextMenu(QWidget* parent);
public:
	void AppendGroup(std::wstring_view groupName);
	void AppendItem(std::wstring_view item);
	void Finish();
	void Execute(const QPoint& where);

	template<typename Functor>
	void SetItemClickCallback(Functor&& fn)
	{
		connect(&selection, &QTreeWidget::itemClicked, std::forward<Functor>(fn) );
	}
private:
	QVBoxLayout layout;
	QLineEdit filter;
	QTreeWidget selection;
	QTreeWidgetItem* current_group;
};

constexpr std::wstring_view skipper = L"XXXX";