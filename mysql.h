#ifndef MYSQL_H
#define MYSQL_H
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<vector>
#include<QDateTime>
using namespace std;
struct StuData{
    int label;
    QString name;
    QDateTime start;
    QDateTime stop;
    StuData(int label,QString name,QDateTime start,QDateTime stop):label(label),name(name),start(start),stop(stop){

    }
};
class MySql
{

private:
    static MySql instance;
    MySql();
    MySql(const MySql *other);
    QSqlDatabase db;
    QSqlQuery query;
public:
    static MySql& getMySql();
    bool insertStudent(QString name);
    int getStudentLabel();
    bool updateStopTime(int label);
    void selectStuAllInfo(vector<StuData>& v);
};

#endif // MYSQL_H
