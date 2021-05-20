#include <Editor/Editor.h>

Editor::Editor()
    :hl(texter.document())
{
    QFont font;
    font.setFamily("Consolas");
    font.setFixedPitch(true);
    font.setPointSize(10);

    vl.addWidget(&texter);
    setLayout(&vl);
}
