// TextureProcessor.cpp: определяет точку входа для приложения.
//

#include <EntryMain.h>
#include <GraphCodex.h>

using namespace std;

int main()
{
	vrg::unique_node x{ "aaaaaa" };

	cout << x->GetName() << endl;
	return 0;
}
