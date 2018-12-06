#ifndef USER_H
#define USER_H

#include <QSqlQuery>
#include <QString>
#include <QVariant> //for conversion of QString when binding to query;

class User
{
    public:
     User();

     bool validateFields();
     bool validatePassword();

     QSqlQuery getQuery();
     QString getFname();
     QString getSname() const;
     QString getUsername() const;
     QString getPassword() const;
     QString getUserType() const;

     void setFname(const QString &value);
     void setSname(const QString &value);
     void setUsername(const QString &value);
     void setPassword(const QString &value);
     void setUserType(const QString &value);
     void setConfirmPassword(const QString &value);



private:
     QString fname;
     QString sname;
     QString username;
     QString password;
     QString userType;
     QString confirmPassword;

};

#endif // USER_H
