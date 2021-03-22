
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
private:
	QVBoxLayout layout;
	QLineEdit filter;
	QTreeWidget selection;
	QTreeWidgetItem* current_group;
};