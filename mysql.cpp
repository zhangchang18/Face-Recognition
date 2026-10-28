#include "mysql.h"
MySql MySql::instance;
MySql::MySql()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kaochang.db");
    query=(QSqlQuery)db;
    db.open();
     QString sql="create table if not exists stu_info(stuLabel integer primary key autoincrement,stuName varchar(20),startTime datetime,stopTime datetime);";
     bool ok=query.exec(sql);
     if(ok){
         qDebug()<<"创建成功";
     }else{
         qDebug()<<"创建失败";
     }
}

MySql::MySql(const MySql *other)
{

}

MySql &MySql::getMySql()
{
    return instance;
}

bool MySql::insertStudent(QString name)
{
    query.prepare("INSERT INTO stu_info (stuName,startTime) VALUES(?,datetime('now','localtime'));");
    query.bindValue(0,name);
    return query.exec();


}

int MySql::getStudentLabel()
{
    query.exec("SELECT last_insert_rowid();");
    int label=-1;
    if(query.next()){
        label=query.value(0).toInt();

    }
    return label;

}

bool MySql::updateStopTime(int label)
{
    query.prepare("UPDATE stu_info set stopTime = datetime('now','localtime' ) WHERE stuLabel=?;");
    query.bindValue(0,label);
    return query.exec();
}

void MySql::selectStuAllInfo(vector<StuData> &v)
{
    query.prepare("SELECT * FROM stu_info;");
    query.exec();
    while(query.next()){
        int label=query.value(0).toInt();
        QString name=query.value(1).toString();
        QDateTime start=query.value(2).toDateTime();
        QDateTime stop=query.value(3).toDateTime();
        StuData data(label,name,start,stop);
        v.push_back(data);
    }
}

