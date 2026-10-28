#ifndef COURCEDIALOG_H
#define COURCEDIALOG_H

#include <QDialog>

namespace Ui {
class CourceDialog;
}

class CourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CourceDialog(QWidget *parent = nullptr);
    ~CourceDialog();

signals:
    void sendInfo(QString name,int num);
private slots:
    void on_pushButton_clicked();

private:
    Ui::CourceDialog *ui;
};

#endif // COURCEDIALOG_H
