#include "purchase.h"
#include <QDebug>
Purchase::Purchase()
{

}

bool Purchase::isValid()
{
    if(getProdCode().isEmpty() || getProdQty()<=0 || !validatePrice())
         return false;
    return true;
}

QSqlQuery Purchase::getQuery()
{
    QSqlQuery qry;
    qry.prepare("insert into purchases(product,qty,unit_cost,total) values(?,?,?,?)");
    qry.addBindValue(getProdCode());
    qry.addBindValue(prodQty);
    qry.addBindValue(getProdUniPrice().toDouble());
    qry.addBindValue(totalCost);
    return qry;
}
double Purchase::getTotalCost() const
{
    return totalCost;
}

void Purchase::setTotalCost(double value)
{
    totalCost = value;
}

void Purchase::calcTotalCost()
{
    bool * status=new bool;
    double unitPrice=this->getProdUniPrice().toDouble(status);
    totalCost=unitPrice*prodQty;
}




