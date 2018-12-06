#include "product.h"
#include <QDebug>
Product::Product()
{

}

QSqlQuery Product::getQuery()
{
    QSqlQuery qry;
    qry.prepare("insert into products values(?,?,?)");
    qry.addBindValue(prodCode);
    qry.addBindValue(prodName);
    qry.addBindValue(prodUniPrice.toDouble());
    return qry;
}

bool Product::validateProd()
{
  //on a minimum a product should have a code,name and unit price -check for nullness
  if(prodCode.isEmpty() || prodName.isEmpty() || prodUniPrice.isEmpty())
      return false;
  return true;
}

bool Product::validatePrice()
{
    //check if the price is a valid number by casting it to double;
    bool * isValid=new bool;
    prodUniPrice.toDouble(isValid);
    return (*isValid) ? true :false;
}
QString Product::getProdCode() const
{
    return prodCode;
}

void Product::setProdCode(const QString &value)
{
    prodCode = value;
}

QString Product::getProdName() const
{
    return prodName;
}

void Product::setProdName(const QString &value)
{
    prodName = value;
}
QString Product::getProdUniPrice() const
{
    return prodUniPrice;
}

void Product::setProdUniPrice(const QString &value)
{
    prodUniPrice = value;
}
int Product::getProdQty() const
{
    return prodQty;
}

void Product::setProdQty(int value)
{
    prodQty = value;
}








