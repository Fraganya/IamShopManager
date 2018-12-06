#include "productscreen.h"
#include "ui_productscreen.h"

productScreen::productScreen(QWidget *parent,QString prod) :
    QDialog(parent),
    ui(new Ui::productScreen)
{
    ui->setupUi(this);
    this->findProduct(prod);
    this->setWindowTitle("Product -"+prod);

}

productScreen::~productScreen()
{
    delete ui;
}

void productScreen::findProduct(QString &prod)
{
    QSqlQuery qry;
    qry.prepare("select count(*),* from products where prod_code=?");
    qry.addBindValue(prod);
    if(!qry.exec()){
        QMessageBox::critical(this,"Critical","Could not retrieve products.<br>error:"+qry.lastError().text());
        return;
    }
    qry.next();
    if(qry.record().value(0).toInt()==0){
        ui->searchResult->setPlainText("No product found");
        return;
    }
    ui->searchResult->setPlainText("Product Code\t\t:"+qry.record().value("prod_code").toString());
    ui->searchResult->appendPlainText("Product Name\t\t:"+qry.record().value("prod_name").toString());
    ui->searchResult->appendPlainText("Unit Price\t\t:"+qry.record().value("unit_price").toString());
    //get other data total sales and total cost
    qry.prepare("select total(total) as 'TSALES' from sales where product=?");
    qry.addBindValue(prod);
    qry.exec();
    qry.next(); //seek to the next record
    double totalSales=qry.record().value("TSALES").toDouble();
    qry.prepare("select total(total) as 'TCOST' from purchases where product=?");
    qry.addBindValue(prod);
    qry.exec();
    qry.next(); //seek to the next record
    double totalCost=qry.record().value("TCOST").toDouble();
    double grossProfit=totalSales-totalCost;

    ui->searchResult->appendPlainText("Total Cost to Date\t:"+QString::number(totalCost));
    ui->searchResult->appendPlainText("Total Sales to Date\t:"+QString::number(totalSales));
    ui->searchResult->appendPlainText("Gross Profit\t\t:"+QString::number(grossProfit));
}
