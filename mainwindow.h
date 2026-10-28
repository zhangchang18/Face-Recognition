#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVector>
#include <QRandomGenerator>
#include<opencv2/opencv.hpp>
#include<opencv2/face.hpp>
#include<vector>
#include<QTimerEvent>
#include<map>
using namespace std;
using namespace cv::face;
using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct PosAndName{
    int column;
    int row;
    QString studentName;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    // 带参构造函数：传入考试科目名称、参考人数
    MainWindow(const QString &subject, int num, QWidget *parent = nullptr);
    void timerEvent(QTimerEvent *event);
    ~MainWindow();

public slots:
    void recvInfo(QString name,int num);
private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_message_clicked();

    void on_pushButton_end_clicked();

private:
    bool changeLabelImg(Mat face,PosAndName value,int flag);
private:


    Ui::MainWindow *ui;

    vector<Mat> studyFaces;
    Mat src;
    vector<int> studyLabels;
    Ptr<FaceRecognizer> recognizer;
    VideoCapture vc;
    CascadeClassifier classsifier;
    vector<Rect> findFaceRect;
    int stuCount;
    map<int,PosAndName> map_stus;
};
#endif // MAINWINDOW_H
