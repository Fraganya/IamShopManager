#include "user.h"

User::User()
{

}

bool User::validateFields()
{
  if(fname.isEmpty() || sname.isEmpty() || username.isEmpty() || password.isEmpty())
      return false;
  return true;
}

bool User::validatePassword()
{
    if(password==confirmPassword){
         return true;
    }
    return false;
}

QSqlQuery User::getQuery()
{
    QSqlQuery qry;
    qry.prepare("insert into users values(?,?,?,?,?)");
    qry.addBindValue(username);
    qry.addBindValue(password);
    qry.addBindValue(userType);
    qry.addBindValue(fname);
    qry.addBindValue(sname);
    return qry;
}

QString User::getFname()
{
    return fname;
}


void User::setFname(const QString &value)
{
    fname = value;
}


void User::setSname(const QString &value)
{
    sname = value;
}


void User::setUsername(const QString &value)
{
    username = value;
}


void User::setPassword(const QString &value)
{
    password = value;
}


void User::setUserType(const QString &value)
{
    userType = value;
}
void User::setConfirmPassword(const QString &value)
{
    confirmPassword = value;
}
QString User::getSname() const
{
    return sname;
}
QString User::getUsername() const
{
    return username;
}
QString User::getPassword() const
{
    return password;
}
QString User::getUserType() const
{
    return userType;
}











