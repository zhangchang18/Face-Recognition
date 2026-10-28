#include "courcedialog.h"
#include "ui_courcedialog.h"
#include "mainwindow.h"

CourceDialog::CourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CourceDialog)
{
    ui->setupUi(this);
    for(int i=20;i<=50;i++){
        ui->comboBox->addItem(QString::number(i));
    }
}

CourceDialog::~CourceDialog()
{
    delete ui;
}

void CourceDialog::on_pushButton_clicked()
{
    // 1. 获取输入的科目名称
    QString object = ui->lineEdit->text().trimmed();

    // 2. 获取选择的参考人数
    int num = ui->comboBox->currentText().toInt();

    // 3. 跳转页面：创建新窗口对象 → 调用带参构造函数 → 显示新窗口
    MainWindow *win = new MainWindow(object, num);
    win->setAttribute(Qt::WA_DeleteOnClose);   // 关闭新窗口时自动释放内存
    connect(this,&CourceDialog::sendInfo,win,&MainWindow::recvInfo);
    win->show();
    emit sendInfo(object,num);
    // 4. 关闭当前对话框
    this->close();
}
