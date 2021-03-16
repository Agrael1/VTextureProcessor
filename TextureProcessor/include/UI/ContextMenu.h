
class ContextMenu : public QMenu
{
public:
	ContextMenu(QWidget* parent);
private:
	QVBoxLayout layout;
	QLineEdit filter;
	QTreeWidget selection;
};

