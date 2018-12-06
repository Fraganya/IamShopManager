#include "sale.h"

Sale::Sale()
{

}

bool Sale::isValid()
{
    if(getProdCode().isEmpty() || getProdQty()<=0 || !validatePrice())
         return false;
    return true;
}

QSqlQuery Sale::getQuery()
{
    QSqlQuery qry;
    qry.prepare("insert into sales(product,qty,unit_price,total) values(?,?,?,?)");
    qry.addBindValue(getProdCode());
    qry.addBindValue(prodQty);
    qry.addBindValue(getProdUniPrice().toDouble());
    qry.addBindValue(totalSale);
    return qry;
}

void Sale::calcTotalSale()
{
    bool * status=new bool;
    double unitPrice=this->getProdUniPrice().toDouble(status);
    totalSale=unitPrice*prodQty;
}

