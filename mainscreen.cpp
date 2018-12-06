#include "mainscreen.h"
#include "ui_mainscreen.h"
#include "productscreen.h"
#include <QDebug>

MainScreen::MainScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScreen)
{
    ui->setupUi(this);
    //initalize window
    this->setFixedSize(700,400);
    ui->lbError->setVisible(false);
    ui->btnMainScreen->setVisible(false);
    ui->topBar->setVisible(false);

    //connect the show password checkbox
    connect(ui->cbPassMode,SIGNAL(toggled(bool)),this,SLOT(togglePassMode(bool)));
    connect(ui->btnLogin,SIGNAL(clicked()),this,SLOT(login()));

    //add event handlers for the quick buttons on the topBar
    connect(ui->btnQuickProduct,SIGNAL(clicked()),this,SLOT(on_btnProductsTile_clicked()));
    connect(ui->btnQuickPurchase,SIGNAL(clicked()),this,SLOT(on_btnPurchasesTile_clicked()));
    connect(ui->btnQuickSale,SIGNAL(clicked()),this,SLOT(on_btnSalesTile_clicked()));

    //set validators for firstname and surname
    ui->txtUFname->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z]{3,15}[][a-zA]{3,15}"),ui->txtUFname));
    ui->txtUSName->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z]{3,15}[][a-zA]{3,15}"),ui->txtUSName));

    //connect to database
    this->connectDB();

}

MainScreen::~MainScreen()
{
    delete ui;
}

void MainScreen::ShowMessage(QString type,QString heading, QString message)
{
    if(type=="warning")
        QMessageBox::warning(this,heading,message);
    else if(type=="critical")
        QMessageBox::critical(this,heading,message);
    else if(type=="information")
        QMessageBox::information(this,heading,message);
}

void MainScreen::connectDB()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    //load database in the current application folder
    db.setDatabaseName("./iam_shop_manager.db");
    if(!db.isOpen()){
        if(!db.open()){
            QMessageBox::critical(this,"DB Error",db.lastError().text());
        }
    }
}

bool MainScreen::isStockAvail(QString prod,int qty)
{
    QSqlQuery qry;
    qry.prepare("select qty from stock where product=?");
    qry.addBindValue(prod);
    if(!qry.exec()){
        qDebug()<<"Error with DB"+qry.lastError().text();
        return false;
    }
    qry.next();
    qDebug()<<qry.record().value("qty").toInt();
    if(qry.record().value("qty").toInt()>=qty)
        return true;
    return false;
}

bool MainScreen::updateStock(QString field,QString cmd,QString prod, int qty)
{
    QSqlQuery qry;
    if(cmd=="increment"){
          qry.prepare("update stock set "+field+"=(select "+field+" from stock where product=?)+? where product=?");
    }
    else{
          qry.prepare("update stock set "+field+"=(select "+field+" from stock where product=?)-? where product=?");
    }
    qry.addBindValue(prod);
    qry.addBindValue(qty);
    qry.addBindValue(prod);
    if(!qry.exec()){
        QMessageBox::critical(this,"Stock Error","Could not update stock.<br>Error"+qry.lastError().text());
        return false;
    }
    return true;
}
void MainScreen::prepMyAccPage()
{
    ui->txtCUfname->setText(user->getFname());
    ui->txtCUsname->setText(user->getSname());
    ui->txtCUname->setText(user->getUsername());
    ui->txtCUpassword->setText(user->getPassword());
}

void MainScreen::disableSuperFnx()
{
    QWidget * widgets[]={
        ui->changePriceFrame,ui->delProdFrame,ui->newProdFrame,ui->usersTab,ui->btnUsersTile,ui->btnQuickProduct
    };

    for(int index=0;index<6;index++){
        widgets[index]->setEnabled(false);
    }
}

void MainScreen::getProdList()
{
    prodsModel=new QSqlQueryModel(this);
    prodsModel->setQuery(QSqlQuery("select prod_code,unit_price from products"));
}

void MainScreen::initData()
{
    ui->btnVNSale->clicked(true);
    ui->btnVNewPurchase->clicked(true);
    ui->btnVStock->clicked(true);
}

void MainScreen::prepTabsScreen()
{
    ui->btnMainScreen->setVisible(true);
    ui->screenHolder->setCurrentIndex(SCREENS::TABS_PAGE);
}

void MainScreen::resetAppState()
{
    //re-enable possible disabled functionalities
    QWidget * widgets[]={
        ui->changePriceFrame,ui->delProdFrame,ui->newProdFrame,ui->usersTab,ui->btnUsersTile,ui->btnQuickProduct
    };

    for(int index=0;index<6;index++){
        widgets[index]->setEnabled(true);
    }
    //hide the main menu button and topbar
    ui->btnMainScreen->setVisible(false);
    ui->topBar->setVisible(false);

    this->setWindowTitle("IAM Shop Manager"); //reset window title
    this->setFixedSize(700,400);
    ui->screenHolder->setCurrentIndex(SCREENS::LOGIN_PAGE);

    //reset login page
    ui->txtUsername->clear();
    ui->txtPassKey->clear();
    ui->cbPassMode->setChecked(false);

    //reset myaccount tab state
    ui->btnChangePassword->setText("Change");
    ui->txtCUpassword->setEnabled(false);

    //reset stacked widget screens to the first window
    QStackedWidget * screens[]={
            ui->salesScreens,ui->purchasesScreens,ui->productScreens,ui->userScreens
        };
    for(int index=0;index<4;index++)
        screens[index]->setCurrentIndex(0);

    //clean up left data on forms
    ui->txtProdSearch->clear();
    this->clearProdDetails();
    this->clearSaleDetails();
    this->clearUserDetails();
    this->clearPurchaseDetails();
}

void MainScreen::clearUserDetails()
{
    QLineEdit * fields[]={
                            ui->txtUFname,ui->txtUSName,ui->txtNUsername,
                            ui->txtNPassword,ui->txtNCPassword};
    for(int index=0;index<5;index++)
       fields[index]->clear();
    QRadioButton * userType=(ui->rbStd->isChecked())? ui->rbStd : ui->rbSuper;
    userType->setAutoExclusive(false);
    userType->setChecked(false);
    userType->setAutoExclusive(true);
}

void MainScreen::clearProdDetails()
{
    QLineEdit * fields[]={ui->txtProdName,ui->txtProdCode,ui->txtProdPrice};
    for(int index=0;index<3;index++){
        (fields[index])->clear();
    }
}

void MainScreen::clearPurchaseDetails()
{
    ui->cmbPProdList->setCurrentIndex(0);
    ui->sbPQty->setValue(0);
    ui->txtPProdCost->clear();
}

void MainScreen::clearSaleDetails()
{
    ui->sbSProdQty->setValue(0);
}


void MainScreen::togglePassMode(bool checked)
{
    if(checked){
        ui->txtPassKey->setEchoMode(QLineEdit::Normal);
    }
    else{
        ui->txtPassKey->setEchoMode(QLineEdit::Password);
    }
}

void MainScreen::login()
{
    QString username=ui->txtUsername->text();
    QString passKey=ui->txtPassKey->text();

    if(username.isEmpty()){
        ui->txtUsername->setFocus();
        ui->lbError->setText("Username cannot be empty!");
        ui->lbError->setVisible(true);
        return;
    }

    if(passKey.isEmpty()){
        ui->txtPassKey->setFocus();
        ui->lbError->setText("Password cannot be empty!");
        ui->lbError->setVisible(true);
        return;
    }
    ui->lbError->setVisible(false);
    QSqlQuery qry;
    qry.prepare("select count(*),username,firstname,surname,password,user_type from users where username=? and password=?");
    qry.addBindValue(username);
    qry.addBindValue(passKey);
    //execute and check if the query was executed successfully
    if(!qry.exec()){
        QMessageBox::critical(this,"Authentication Error",qry.lastError().text());
        return;
    }

    //check if credentials were valid
    qry.next();
    if(!(qry.record().value(0).toInt()==1)){
        ui->lbError->setText("Invalid Credentials!");
        ui->lbError->setVisible(true);
        return;
    }
    if(ui->lbError->isVisible()) ui->lbError->setVisible(false);

    //record details of the currently logged in user
    this->user=new User;
    user->setUsername(qry.record().value(1).toString());
    user->setFname(qry.record().value(2).toString());
    user->setSname(qry.record().value(3).toString());
    user->setPassword(qry.record().value(4).toString());
    user->setUserType(qry.record().value(5).toString());

    this->prepMyAccPage();
    this->initData(); //triggers the loading of data

    if(!(user->getUserType()=="SUPER"))
        this->disableSuperFnx();
    //adjust window size and prep main page;
    this->setMinimumSize(700,550);
    this->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    ui->screenHolder->setCurrentIndex(SCREENS::MAIN_PAGE);
    ui->topBar->setVisible(true);
    this->setWindowTitle(QString("IAM Shop Manager [%2][%1]").arg(username,user->getUserType()));
}



void MainScreen::on_btnMainScreen_clicked()
{
    ui->btnMainScreen->setVisible(false);
    ui->screenHolder->setCurrentIndex(SCREENS::MAIN_PAGE);
}

void MainScreen::on_btnLogout_clicked()
{
    this->resetAppState();
}

void MainScreen::on_btnSalesTile_clicked()
{
    this->prepTabsScreen();
    ui->btnVNSale->clicked(true); //trigger the new sale to load data
    ui->fnxTabs->setCurrentWidget(ui->salesTab);
    ui->salesScreens->setCurrentIndex(SALE_SCREENS::NEW_SALE_PAGE);
}

void MainScreen::on_btnVNewUser_clicked()
{
    ui->userScreens->setCurrentIndex(USER_SCREENS::NEW_USER_PAGE);
}

void MainScreen::on_btnVUsers_clicked()
{
      QSqlQueryModel * usersModel=new QSqlQueryModel(this);
      usersModel->setQuery(QSqlQuery("select firstname,surname,username,user_type from users"));
      ui->usersTView->setModel(usersModel);
      ui->usersTView->resizeRowsToContents();
      //set proper header names
      usersModel->setHeaderData(0,Qt::Horizontal,tr("Firstname"));
      usersModel->setHeaderData(1,Qt::Horizontal,tr("Surname"));
      usersModel->setHeaderData(2,Qt::Horizontal,tr("Username"));
      usersModel->setHeaderData(3,Qt::Horizontal,tr("Type"));
      //adjust horizontal header to take full space
      QHeaderView * header=ui->usersTView->horizontalHeader();
      for(int col=0;col<header->count();col++)
      {
          header->setSectionResizeMode(col,QHeaderView::Stretch);
      }
      ui->userScreens->setCurrentIndex(USER_SCREENS::VIEW_USER_PAGE);
}




void MainScreen::on_btnAddUser_clicked()
{
    //create new user object and retrive entered values
    User * newUser=new User;
    newUser->setFname(ui->txtUFname->text());
    newUser->setSname(ui->txtUSName->text());
    newUser->setUsername(ui->txtNUsername->text());
    newUser->setPassword(ui->txtNPassword->text());
    newUser->setConfirmPassword(ui->txtNCPassword->text());

    //get the type of user
    if(ui->rbStd->isChecked()){
        newUser->setUserType("STD");
    }
    else if(ui->rbSuper->isChecked()){
        newUser->setUserType("SUPER");
    }
    else{
        QMessageBox::warning(this,"Notice!","Please select user Type!");
        return;
    }
    //validate user details for nullness
    if(!newUser->validateFields()){
        QMessageBox::warning(this,"Notice!","Some fields are empty!");
        return;
    }
    //comfirm that user has the correct password by matching
    if(!(newUser->validatePassword())){
        QMessageBox::warning(this,"Notice!","Passwords do not match!");
        return;
    }

    QSqlQuery userAddQry=newUser->getQuery();
    if(!userAddQry.exec()){
        QMessageBox::critical(this,"DB Error","Coud not add user <br>Error: "+userAddQry.lastError().text());
    }
    else{
        QMessageBox::information(this,"Success","The user "+newUser->getFname()+" was added successfuly");
        this->clearUserDetails();
    }
}

void MainScreen::on_btnVDelUsers_clicked()
{
    QSqlTableModel delUsersModel(this);
    delUsersModel.setTable("users");
    //filter out the currently logged in username
    delUsersModel.setFilter(QString("username!=\"%1\"").arg(user->getUsername()));
    delUsersModel.select();
    ui->userList->clear(); //prep for incoming data;
    //retrieve the username from the model data and add to users list box for user choice
    for(int index=0;index<delUsersModel.rowCount();index++)
        ui->userList->addItem(delUsersModel.record(index).value("username").toString());
    ui->userScreens->setCurrentIndex(USER_SCREENS::DELETE_USER_PAGE);
}

void MainScreen::on_btnFindUser_clicked()
{
   QString username=QInputDialog::getText(this,"Find User","Enter username");
   if(username.size()==0)
       return;
   for(int index=0;index<(ui->userList->count());index++){
       if(username==ui->userList->item(index)->text()){
           ui->userList->setCurrentItem(ui->userList->item(index));
           return;
       }
   }
   QMessageBox::information(this,"Message","No user by that username was found");
}

void MainScreen::on_btnDeleteUser_clicked()
{
    QModelIndex curIndex=ui->userList->currentIndex();
    if(!(curIndex.isValid())){
      QMessageBox::warning(this,"Warning","No user was selected");
      return;
    }
    QString username=ui->userList->currentItem()->text();
    QSqlQuery delQry;
    delQry.prepare("delete from users where username=?");
    delQry.addBindValue(username);
    if(!delQry.exec()){
        QMessageBox::critical(this,"Failure","Failed to delete user <br>Error:"+delQry.lastError().text());
        return;
    }
    delete ui->userList->currentItem();
    QMessageBox::information(this,"Success","Successfully deleted the user "+username);
}

void MainScreen::on_btnChangePassword_clicked()
{
    //enable the password field for editing if the user clicks the btn with Change caption
    if(ui->btnChangePassword->text()=="Change"){
        ui->txtCUpassword->setDisabled(false);
        ui->txtCUpassword->setFocus();
        ui->btnChangePassword->setText("Save");
        return;
    }

    //the user wants to save the updated password
    if(ui->txtCUpassword->text().isEmpty()){
        QMessageBox::warning(this,"Warning","Password cannot be empty");
        ui->txtCUpassword->setFocus();
        return;
    }
    //update password if it has been changed else skip if is the same
    if(ui->txtCUpassword->text()!=user->getPassword()){
        //the password has changed
        QSqlQuery passQry;
        passQry.prepare("update users set password=? where username=?");
        passQry.addBindValue(ui->txtCUpassword->text());
        passQry.addBindValue(user->getUsername());
        if(!passQry.exec()){
            QMessageBox::critical(this,"Failure","Could not change password <br>Error:"+passQry.lastError().text());
            return;
        }
        //updated the user password to the current one
        user->setPassword(ui->txtCUpassword->text());
        QMessageBox::information(this,"Success!","Password successfully changed");
    }
    ui->btnChangePassword->setText("Change");
    ui->txtCUpassword->setDisabled(true); //disable the field if password changed or is the same
}

void MainScreen::on_btnVNewProd_clicked()
{
    ui->productScreens->setCurrentIndex(PROD_SCREENS::NEW_PROD_PAGE);
}

void MainScreen::on_btnVProds_clicked()
{
    //get the products data
    QSqlQueryModel * prodsModel=new QSqlQueryModel(this);
    prodsModel->setQuery(QSqlQuery("select * from products"));
    ui->prodsView->setModel(prodsModel);
    ui->prodsView->resizeRowsToContents();
    //set proper header names
    prodsModel->setHeaderData(0,Qt::Horizontal,tr("Product Code"));
    prodsModel->setHeaderData(1,Qt::Horizontal,tr("Product Name"));
    prodsModel->setHeaderData(2,Qt::Horizontal,tr("Unit Price"));
    //adjust horizontal header to take full space
    QHeaderView * header=ui->prodsView->horizontalHeader();
    for(int col=0;col<header->count();col++)
    {
        header->setSectionResizeMode(col,QHeaderView::Stretch);
    }
     ui->productScreens->setCurrentIndex(PROD_SCREENS::VIEW_PROD_PAGE);
}

void MainScreen::on_btnVDelProds_clicked()
{
    QSqlQueryModel delProdModel(this);
    delProdModel.setQuery(QSqlQuery("select prod_code from products"));
    ui->productList->clear(); //prep for incoming data;
    //retrieve the product code from the model data and add to product list box for user choice
    for(int index=0;index<delProdModel.rowCount();index++)
        ui->productList->addItem(delProdModel.record(index).value("prod_code").toString());
     ui->productScreens->setCurrentIndex(PROD_SCREENS::DELETE_PROD_PAGE);
}

void MainScreen::on_btnVChangeProds_clicked()
{
    prodsModel=new QSqlQueryModel(this);
    prodsModel->setQuery(QSqlQuery("select prod_code,unit_price from products"));
    ui->cmbCProdsList->clear(); //prep for incoming data;
    //retrieve the product code from the model data and add to product list box for user choice
    for(int index=0;index<prodsModel->rowCount();index++)
        ui->cmbCProdsList->addItem(prodsModel->record(index).value("prod_code").toString());
    //init the current product price field with the first product's price
    double firstPrice=prodsModel->record(0).value("unit_price").toDouble();
    ui->txtCProdPrice->setText(QString::number(firstPrice));
    ui->productScreens->setCurrentIndex(PROD_SCREENS::CHANGE_PRICE_PAGE);
}

void MainScreen::on_btnAddProd_clicked()
{
    //create product object and validate info
    Product * newProd=new Product;
    newProd->setProdCode(ui->txtProdCode->text());
    newProd->setProdName(ui->txtProdName->text());
    newProd->setProdUniPrice(ui->txtProdPrice->text());

    //validate product
    if(!newProd->validateProd()){
        ShowMessage("warning","Warning","Check<br>Some product properties are empty!");
        return;
    }

    if(!newProd->validatePrice()){
        ShowMessage("warning","Warning","Check<br>You have an invalid unit price!");
        return;
    }

    QSqlQuery prodQry=newProd->getQuery();
    if(!prodQry.exec()){
        ShowMessage("warning","Failure","Failed to add the product<br>Error:"+prodQry.lastError().text());
        return;
    }
    //add entry in stock
    prodQry.prepare("insert into stock(product) values(?)");
    prodQry.addBindValue(newProd->getProdCode());
    prodQry.exec();
    this->clearProdDetails();
    ShowMessage("information","Success","The product "+newProd->getProdName()+"was added successfully");
    this->initData();
}

void MainScreen::on_btnClearProd_clicked()
{
    this->clearProdDetails();
}

void MainScreen::on_btnFindProduct_clicked()
{
    QString prodCode=QInputDialog::getText(this,"Find Product","Enter Product Code");
    if(prodCode.size()==0)
        return;
    for(int index=0;index<(ui->productList->count());index++){
        if(prodCode==ui->productList->item(index)->text()){
            ui->productList->setCurrentItem(ui->productList->item(index));
            return;
        }
    }
    QMessageBox::information(this,"Message","No Product by that product code was found");
}

void MainScreen::on_btnDelProduct_clicked()
{
    QModelIndex curIndex=ui->productList->currentIndex();
    if(!(curIndex.isValid())){
      QMessageBox::warning(this,"Warning","No product was selected");
      return;
    }
    QString prodCode=ui->productList->currentItem()->text();
    QSqlQuery delQry;
    delQry.prepare("delete from products where prod_code=?");
    delQry.addBindValue(prodCode);
    if(!delQry.exec()){
        QMessageBox::critical(this,"Failure","Failed to delete Product <br>Error:"+delQry.lastError().text());
        return;
    }
    QMessageBox::information(this,"Success","Successfully deleted the product");
    this->initData(); //refresh data
}

void MainScreen::on_cmbCProdsList_currentIndexChanged(int index)
{
    double price=prodsModel->record(index).value(1).toDouble();
    ui->txtCProdPrice->setText(QString::number(price));
}

void MainScreen::on_btnUpdateProdPrice_clicked()
{
    bool * status=new bool;
    double price=ui->txtCProdPrice->text().toDouble(status);
    if(!(*status) || ui->txtCProdPrice->text().isEmpty()){
        ShowMessage("warning","Warning","Check<br>Product Price is invalid");
        return;
    }

    QSqlQuery updateQry;
    updateQry.prepare("update products set unit_price=? where prod_code=?");
    updateQry.addBindValue(price);
    updateQry.addBindValue(ui->cmbCProdsList->currentText());
    if(!updateQry.exec()){
        ShowMessage("critical","Failure","Failed to updated price<br>Error:"+updateQry.lastError().text());
        return;
    }
    ShowMessage("information","Success","Successfully updated product price!");
}

void MainScreen::on_btnAddPurchase_clicked()
{
    Purchase * newPurchase=new Purchase;
    newPurchase->setProdCode(ui->cmbPProdList->currentText());
    newPurchase->setProdQty(ui->sbPQty->value());
    newPurchase->setProdUniPrice(ui->txtPProdCost->text()); //this is the purchase price

    if(!(newPurchase->isValid())){
        ShowMessage("warning","Warning","Check<br>Purchase entry is invalid.");
        return;
    }
    //update stock - dont add the purchase if it fails
    if(!updateStock("qty","increment",newPurchase->getProdCode(),newPurchase->getProdQty()))
        return;
    //update purchases in stock
    if(!updateStock("purchases","increment",newPurchase->getProdCode(),newPurchase->getProdQty()))
        return;
    newPurchase->calcTotalCost();
    QSqlQuery purchaseQry=newPurchase->getQuery();
    if(!purchaseQry.exec()){
         ShowMessage("critical","Failure","Failed to add pruchase<br>Error:"+purchaseQry.lastError().text());
         return;
    }
    this->clearPurchaseDetails();
    ShowMessage("information","Success","The purchase was added successfully!.");
}

void MainScreen::on_btnVNewPurchase_clicked()
{
    this->getProdList();
    ui->cmbPProdList->clear(); //prep for refreshed data
    //retrieve the product code from the model data and add to product list box for user choice
    for(int index=0;index<prodsModel->rowCount();index++)
        ui->cmbPProdList->addItem(prodsModel->record(index).value("prod_code").toString());
    ui->purchasesScreens->setCurrentIndex(PURCHASE_SCREENS::NEW_PURCHASE_PAGE);
}

void MainScreen::on_btnVPurchases_clicked()
{
    //get the purchases data
    purchaseModel=new QSqlQueryModel(this);
    purchaseModel->setQuery(QSqlQuery("select * from purchases"));
    ui->purchasesView->setModel(purchaseModel);

    //set proper header names
    purchaseModel->setHeaderData(0,Qt::Horizontal,tr("Purchase # "));
    purchaseModel->setHeaderData(1,Qt::Horizontal,tr("Product"));
    //  purchaseModel->setHeaderData(2,Qt::Horizontal,tr("Product Code"));
    purchaseModel->setHeaderData(2,Qt::Horizontal,tr("QTY"));
    purchaseModel->setHeaderData(3,Qt::Horizontal,tr("Unit Price"));
    purchaseModel->setHeaderData(4,Qt::Horizontal,tr("Total"));

    //initilize filter
    purchasesFilterProxy=new QSortFilterProxyModel(this);
    purchasesFilterProxy->setSourceModel(purchaseModel);
    purchasesFilterProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    purchasesFilterProxy->setFilterKeyColumn(1);

    ui->purchasesView->setModel(purchasesFilterProxy);
    //adjust horizontal header to take full space
    QHeaderView * header=ui->purchasesView->horizontalHeader();
    for(int col=0;col<header->count();col++)
    {
        header->setSectionResizeMode(col,QHeaderView::Stretch);
    }
    ui->purchasesScreens->setCurrentIndex(PURCHASE_SCREENS::VIEW_PURCHASES_PAGE);
 }


void MainScreen::on_btnVNSale_clicked()
{
    this->getProdList(); //in case of updates to prices
    ui->cmbSProdList->clear(); //prep for refreshed data
    //retrieve the product code from the model data and add to product list box for user choice
    for(int index=0;index<prodsModel->rowCount();index++)
        ui->cmbSProdList->addItem(prodsModel->record(index).value("prod_code").toString());
    //init the current product price field with the first product's price
    double firstPrice=prodsModel->record(0).value("unit_price").toDouble();
    ui->txtSUnitPrice->setText(QString::number(firstPrice));
    ui->salesScreens->setCurrentIndex(SALE_SCREENS::NEW_SALE_PAGE);

}

void MainScreen::on_cmbSProdList_currentIndexChanged(int index)
{
    double price=prodsModel->record(index).value("unit_price").toDouble();
    ui->txtSUnitPrice->setText(QString::number(price));
}

void MainScreen::on_sbSProdQty_valueChanged(int qty)
{
    double unitPrice=ui->txtSUnitPrice->text().toDouble();
    ui->totalSale->display(QString::number(unitPrice*qty)); //multiply unit price and qty
}

void MainScreen::on_txtSUnitPrice_textChanged(const QString &price)
{
    double qty=ui->sbSProdQty->value();
    double unitPrice=price.toDouble();
    ui->totalSale->display(QString::number(unitPrice*qty)); //multiply unit price and qty
}

void MainScreen::on_btnAddSale_clicked()
{

    Sale * newSale=new Sale;
    newSale->setProdCode(ui->cmbSProdList->currentText());
    newSale->setProdQty(ui->sbSProdQty->value());
    newSale->setProdUniPrice(ui->txtSUnitPrice->text()); //this is the purchase price

    if(!(newSale->isValid())){
        ShowMessage("warning","Warning","Check<br>Sales entry is invalid.");
        return;
    }
    //check if sufficient stock is available to make this sale
    if(!isStockAvail(newSale->getProdCode(),newSale->getProdQty())){
        ShowMessage("warning","Insufficient Stock","You cannot make this sale because you have insufficient stock.");
        return;
    }
    //update stock first -dont add the sale if it fails
    if(!updateStock("qty","decrement",newSale->getProdCode(),newSale->getProdQty()))
        return;
    if(!updateStock("sales","increment",newSale->getProdCode(),newSale->getProdQty()))
        return;
    newSale->calcTotalSale();
    QSqlQuery saleQry=newSale->getQuery();
    if(!saleQry.exec()){
         ShowMessage("critical","Failure","Failed to add sale<br>Error:"+saleQry.lastError().text());
         return;
    }
    this->clearSaleDetails();
    ShowMessage("information","Success","The sale was added successfully!.");
}

void MainScreen::on_btnVSales_clicked()
{
    //get the purchases data
    salesModel=new QSqlQueryModel(this);
    salesModel->setQuery(QSqlQuery("select * from sales"));
    //set proper header names
    salesModel->setHeaderData(0,Qt::Horizontal,tr("Sale # "));
    salesModel->setHeaderData(1,Qt::Horizontal,tr("Product"));
    salesModel->setHeaderData(2,Qt::Horizontal,tr("Unit Price"));
    salesModel->setHeaderData(3,Qt::Horizontal,tr("Qty"));
    salesModel->setHeaderData(4,Qt::Horizontal,tr("Total"));

    //initilize filter
    salesFilterProxy=new QSortFilterProxyModel(this);
    salesFilterProxy->setSourceModel(salesModel);
    salesFilterProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    salesFilterProxy->setFilterKeyColumn(1);

    ui->salesView->setModel(salesFilterProxy);
    //adjust horizontal header to take full space
    QHeaderView * header=ui->salesView->horizontalHeader();
    for(int col=0;col<header->count();col++)
    {
        header->setSectionResizeMode(col,QHeaderView::Stretch);
    }
    ui->salesScreens->setCurrentIndex(SALE_SCREENS::VIEW_SALES_PAGE);
}



void MainScreen::on_btnVStock_clicked()
{
   this->loadStock();

}

void MainScreen::on_btnPurchasesTile_clicked()
{
    this->prepTabsScreen();
    ui->btnVNewPurchase->clicked(true);
    ui->purchasesScreens->setCurrentIndex(PURCHASE_SCREENS::NEW_PURCHASE_PAGE);
    ui->fnxTabs->setCurrentWidget(ui->purchasesTab);
}

void MainScreen::on_btnProductsTile_clicked()
{
    this->prepTabsScreen();
    ui->productScreens->setCurrentIndex(PROD_SCREENS::NEW_PROD_PAGE);
    ui->fnxTabs->setCurrentWidget(ui->productsTab);
}

void MainScreen::on_btnStockTile_clicked()
{
    this->prepTabsScreen();
    this->loadStock();
    ui->btnVStock->clicked(true); //trigger the VStock button to load stock
    ui->stockScreens->setCurrentIndex(STOCK_SCREENS::VIEW_PAGE);
    ui->fnxTabs->setCurrentWidget(ui->stockTab);
}

void MainScreen::on_btnUsersTile_clicked()
{
    this->prepTabsScreen();
    ui->userScreens->setCurrentIndex(USER_SCREENS::NEW_USER_PAGE);
    ui->fnxTabs->setCurrentWidget(ui->usersTab);
}

void MainScreen::on_btnMyAccount_clicked()
{
    this->prepTabsScreen();
    ui->fnxTabs->setCurrentWidget(ui->accountTab);
}

void MainScreen::on_sbPQty_valueChanged(int qty)
{
    double unitCost=ui->txtPProdCost->text().toDouble();
    ui->totalCost->display(QString::number(unitCost*qty)); //multiply unit cost and qty and display to lcd
}

void MainScreen::on_txtPProdCost_textChanged(const QString &cost)
{
    bool * valid=new bool;
    double unitCost=cost.toDouble(valid);
    if(!valid){
        QMessageBox::warning(this,"Warninig","Invalid ");
        return;
    }
    ui->totalCost->display(QString::number(unitCost*ui->sbPQty->value()));
}

void MainScreen::on_btnClearUser_clicked()
{
    this->clearUserDetails();
}

void MainScreen::on_btnClearPurchase_clicked()
{
    this->clearPurchaseDetails();
}

void MainScreen::on_btnCLearSale_clicked()
{
    this->clearSaleDetails();
}

void MainScreen::on_btnFindProd_clicked()
{
    QString prod=ui->txtProdSearch->text();
    if(prod.isEmpty()){
        QMessageBox::warning(this,"Warning","You can't search for a product with an empty string");
        return;
    }
    productScreen * searchSreen=new productScreen(this,prod);
    searchSreen->show();
    ui->txtProdSearch->clear();
}

void MainScreen::loadStock()
{
    //get the stock table data
    stockModel=new QSqlQueryModel(this);
    stockModel->setQuery(QSqlQuery("select product,prod_name,purchases,sales,qty from "
                                   "stock inner join products on stock.product=products.prod_code"));
    ui->stockView->setModel(stockModel);
    ui->stockView->resizeRowsToContents();
    //set proper header names
    stockModel->setHeaderData(0,Qt::Horizontal,tr("Product Code "));
    stockModel->setHeaderData(1,Qt::Horizontal,tr("Name"));
    stockModel->setHeaderData(2,Qt::Horizontal,tr("Purchases(QTY)"));
    stockModel->setHeaderData(3,Qt::Horizontal,tr("SALES(QTY)"));
    stockModel->setHeaderData(4,Qt::Horizontal,tr("Available(QTY)"));
    //adjust horizontal header to take full space
    QHeaderView * header=ui->stockView->horizontalHeader();
    for(int col=0;col<header->count();col++)
    {
        header->setSectionResizeMode(col,QHeaderView::Stretch);
    }
}

void MainScreen::on_txtPurchaseFilter_textChanged(const QString &prod)
{
    this->purchasesFilterProxy->setFilterFixedString(prod);
}

void MainScreen::on_txtSaleFilter_textChanged(const QString &prod)
{
    this->salesFilterProxy->setFilterFixedString(prod);
}


void MainScreen::on_btnHelp_clicked()
{
    QDesktopServices::openUrl(QUrl("file:.////user_manual.pdf",QUrl::TolerantMode));
}
