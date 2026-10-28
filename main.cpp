#include "courcedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    // 先显示考场设置对话框（输入科目名称、选择参考人数）
    CourceDialog w;
    w.show();

    return a.exec();
}
