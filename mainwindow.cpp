#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFile>
#include<QMessageBox>
#include"mysql.h"
#include<cstdlib>
#include<ctime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::MainWindow(const QString &subject, int num, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(0));


    QFile file("face.xml");
    if(file.exists()){
        recognizer=FaceRecognizer::load<LBPHFaceRecognizer>("face.xml");
    }else{
        recognizer=LBPHFaceRecognizer::create();
    }

    recognizer->setThreshold(150);
    if(vc.open(0))
    {
        startTimer(50);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recvInfo(QString name, int num)
{
    qDebug()<<"槽函数接收："<<name<<num;
    this->stuCount=num;
    ui->label_title->setText(name+"考试");
    for(int i=0;i<num;i++){
        QLabel *label=new QLabel(this);
        label->setPixmap(QPixmap(":/person.png"));
        label->setScaledContents(true);
        label->setFrameShape(QFrame::Box);
        label->setObjectName("0");
        ui->gridLayout->addWidget(label,i/6,i%6);


    }



}
void MainWindow::timerEvent(QTimerEvent *event){
    classsifier=CascadeClassifier("D:/opencv/opencv3.4-install/install/etc/haarcascades/haarcascade_frontalface_alt2.xml");
    vc>>src;
    if (src.empty()) {
        return;
    }
    flip(src,src,1);
    Mat gray;
    cvtColor(src,gray,CV_BGR2GRAY);

    classsifier.detectMultiScale(gray,findFaceRect);
    if(findFaceRect.size()>0){
        rectangle(src,findFaceRect[findFaceRect.size()-1],Scalar(0,0,255),3);
    }
    QImage img(src.data,src.cols,src.rows,src.cols*src.channels(),QImage::Format_BGR888);
    QPixmap p=QPixmap::fromImage(img);

    ui->label->setPixmap(p);
    ui->label->setScaledContents(true);
}

void MainWindow::on_pushButton_start_clicked()
{
    if(findFaceRect.size()<=0){
        QMessageBox::information(this,"提示","未找到学生");
        return;
    }
        QString name = ui->lineEdit->text().trimmed();
        if(name.isEmpty()){
            QMessageBox::information(this,"提示","请输入考生姓名");
            return;
        }
        if(map_stus.size()==stuCount){
            QMessageBox::information(this,"提示","座位已满");
            return;
        }
        Mat face=src(findFaceRect[findFaceRect.size()-1]).clone();
        Mat gray;
        cvtColor(face,gray,CV_BGRA2GRAY);
        cv::resize(gray,gray,Size(100,100));
        studyFaces.push_back(gray);
        bool ok=MySql::getMySql().insertStudent(name);
        if(!ok){
            QMessageBox::information(this,"提示","数据库插入失败");
            return;
        }
        int label=MySql::getMySql().getStudentLabel();
        if(label==-1){
            QMessageBox::information(this,"提示","数据库标签异常");
            return;
        }
        qDebug()<<"录入学生的标签为："<<label;
        studyLabels.push_back(label);

        recognizer->update(studyFaces,studyLabels);
        recognizer->save("face.xml");

        PosAndName pos;
        do{
            int num=rand()%stuCount;
            pos.row=num/6;
            pos.column=num%6;
            pos.studentName=ui->lineEdit->text();
        }while(changeLabelImg(face,pos,0));

        map_stus.insert(pair<int,PosAndName>(label,pos));
        QMessageBox::information(this,"提示","录入成功，请开始考试！！！");

}

void MainWindow::on_pushButton_message_clicked()
{
    vector<StuData> infos;
    MySql::getMySql().selectStuAllInfo(infos);
    QFile file("data.csv");
    QTextStream stream(&file);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Truncate)){

        return;
    }
    QList<QString> titles={"标签","姓名","开始考试时间","结束考试时间"};
    for(auto t: titles){
        stream<<t<<",";
    }
    stream<<'\n';
    for(auto temp: infos){
        int label=temp.label;
        QString name=temp.name;
        QString start=temp.start.toString("yyyy-MM-dd hh:mm:ss");
        QString stop=temp.stop.toString("yyyy-MM-dd hh:mm:ss");
        stream<<label<<","<<name<<","<<start<<","<<stop;
        stream<<'\n';
    }
    file.close();
    QMessageBox::information(this,"提示","导出完成");
}

void MainWindow::on_pushButton_end_clicked()
{
    if(findFaceRect.size()<=0){
        QMessageBox::information(this,"提示","未找到学生");
        return;
    }
    Mat face=src(findFaceRect[findFaceRect.size()-1]).clone();
    Mat gray;
    cvtColor(face,gray,CV_BGRA2GRAY);
    cv::resize(gray,gray,Size(100,100));
    int label=-1;
    double confidence=0;
    recognizer->predict(gray,label,confidence);
    if(label==-1){
        QMessageBox::information(this,"提示","未匹配人员信息");
        return;
    }
    if(map_stus.count(label)){
        PosAndName pos=map_stus[label];

    }
    if(!map_stus.count(label)){
        QMessageBox::information(this,"提示","没找到学生");
        return;
    }
    PosAndName pos =map_stus[label];
    qDebug()<<"学生姓名："<<pos.studentName;
    int choose=QMessageBox::information(this,"提示",pos.studentName+"确认结束考试吗？",QMessageBox::Ok|QMessageBox::Cancel);
    if(choose=QMessageBox::Ok){
        changeLabelImg(Mat(),pos,1);
        MySql::getMySql().updateStopTime(label);
    }
}

bool MainWindow::changeLabelImg(Mat face, PosAndName value, int flag)
{
    QWidget* widget= ui->gridLayout->itemAtPosition(value.row,value.column)->widget();
    QLabel *label=dynamic_cast<QLabel*>(widget);
    if(label==nullptr){
        return false;
    }
    if(flag==1){
        label->setText(value.studentName+"结束考试");
        return false;
    }
    QString objectName=label->objectName();
    if(objectName=="1"){
        return true;
    }
    label->setObjectName("1");
    if(flag==0){
        QImage img(face.data,face.cols,face.rows,face.cols*face.channels(),QImage::Format_BGR888);
        label->setPixmap(QPixmap::fromImage(img));
    }

    return false;
}
