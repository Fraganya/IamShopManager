#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include <QtSql>
#include <QDesktopServices>

#include "screens.h"
#include "user.h"
#include "product.h"
#include "purchase.h"
#include "sale.h"
#include "productscreen.h"

namespace Ui {
class MainScreen;
}

class MainScreen : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScreen(QWidget *parent = 0);
    ~MainScreen();

private:
    //member variables
    Ui::MainScreen *ui;

    User * user;

    //models for storing and filtering system data as retrived from the db
    QSqlQueryModel * prodsModel;
    QSqlQueryModel * purchaseModel;
    QSqlQueryModel * salesModel;
    QSqlQueryModel * stockModel;

    QSortFilterProxyModel * purchasesFilterProxy;
    QSortFilterProxyModel * salesFilterProxy;

    //helper methods - used to display message boxes of similar type
    void ShowMessage(QString,QString,QString);//shows a message to the user;

    //private member methods
    void connectDB(); //creates a db connection
    /**
     * @brief isStockAvail -checks if stock is available
     * @return bool
     */
    bool isStockAvail(QString,int);

    /**
     * @brief updateStock -updates stock
     * @return bool
     * @param field - field name to update
     * @param action - action to perfom [increment/decrement]
     * @param qty - quantity
     */
    bool updateStock(QString,QString,QString,int);

    //clean up methods
    void clearUserDetails(); //clears the details entered on the add new user form
    void clearProdDetails(); //clears the details entered on the new product form
    void clearPurchaseDetails();//clears up the purchases form
    void clearSaleDetails(); //clears up the sales form


    //initilization methods
    void prepMyAccPage(); //prepares the account page for the currently logged in user
    void disableSuperFnx(); //diasbles super user fnxs for std users
    void getProdList(); //retrives product list from db into prodsModel;
    void initData(); //triggers the loading of initial data
    void prepTabsScreen(); //activates the tabs page and enables the main menu

    void resetAppState(); //resets every setting to the initial



private slots:

    //button event handlers
    void togglePassMode(bool); //toggles the echo mode of the password field
    void login(); //validates the user credentials and logs the user into the system

    /**
    * main screen action event handlers
    */


    /**
     * navigates to the appropriate tab.
     */

    void on_btnSalesTile_clicked();
    void on_btnPurchasesTile_clicked();
    void on_btnProductsTile_clicked();
    void on_btnStockTile_clicked();
    void on_btnUsersTile_clicked();
    void on_btnMyAccount_clicked();

    /**
     * @brief on_btnMainScreen_clicked
     * activates the main screen page
     */
    void on_btnMainScreen_clicked();
    /**
     * @brief on_btnLogout_clicked
     * logs out the user
     */
    void on_btnLogout_clicked();

    /**
     * @brief on_btnVNewUser_clicked
     * activates the add user widget page
     */
    void on_btnVNewUser_clicked();

    /**
     * @brief on_btnVUsers_clicked
     * activates the view users widget page
     */
    void on_btnVUsers_clicked();

    /**
     * @brief on_btnAddUser_clicked
     * activates the add user widget page
     */
    void on_btnAddUser_clicked();

    /**
     * @brief on_btnVDelUsers_clicked
     * activates the add user widget page
     */
    void on_btnVDelUsers_clicked();

    /**
     * @brief on_btnFindUser_clicked
     * highlights the specified user on the user list when deleting a user
     */
    void on_btnFindUser_clicked();

    /**
     * @brief on_btnDeleteUser_clicked
     * deletes a user from the databse
     */
    void on_btnDeleteUser_clicked();

    /**
     * @brief on_btnChangePassword_clicked
     * saves the changed password or enables the password field for modification
     */
    void on_btnChangePassword_clicked();

    /**
     * @brief on_btnVNewProd_clicked
     * activate the view new product page
     */
    void on_btnVNewProd_clicked();

    /**
     * @brief on_btnVProds_clicked
     * activates the view list of products page
     */
    void on_btnVProds_clicked();

    /**
     * @brief on_btnVDelProds_clicked
     * activate the delete products page
     */
    void on_btnVDelProds_clicked();

    /**
     * @brief on_btnVChangeProds_clicked
     * activates the change product price page
     */
    void on_btnVChangeProds_clicked();

    /**
     * @brief on_btnAddProd_clicked
     * adds a products a product to the DB and updates stock.
     */
    void on_btnAddProd_clicked();

    /**
     * @brief on_btnClearProd_clicked
     * clears up the product form
     */
    void on_btnClearProd_clicked();

    /**
     * @brief on_btnFindProduct_clicked
     * highlights the specified product on the delete product list on the delete page
     */
    void on_btnFindProduct_clicked();

    /**
     * @brief on_btnDelProduct_clicked
     * delete a product from the database
     */
    void on_btnDelProduct_clicked();

    /**
     * @brief on_cmbCProdsList_currentIndexChanged - updates the price to that of the selected product for adjustment
     * @param index -index of the currently selected product
     */
    void on_cmbCProdsList_currentIndexChanged(int index);

    /**
     * @brief on_btnUpdateProdPrice_clicked - updates the price of a product
     */
    void on_btnUpdateProdPrice_clicked();

    /**
     * @brief on_btnAddPurchase_clicked - adds a purchase transaction to the DB
     */
    void on_btnAddPurchase_clicked();

    /**
     * @brief on_btnVNewPurchase_clicked - activates the add new purchase page
     */
    void on_btnVNewPurchase_clicked();

    /**
     * @brief on_btnVPurchases_clicked - activates the view purchases page
     */
    void on_btnVPurchases_clicked();

    /**
     * @brief on_btnVNSale_clicked -activates the add new sale page
     */
    void on_btnVNSale_clicked();

    /**
     * @brief on_cmbSProdList_currentIndexChanged - updates the selling price field
     * @param index -index of current product as reflected in the prodsModel
     */
    void on_cmbSProdList_currentIndexChanged(int index);

    /**
     * @brief on_sbSProdQty_valueChanged - updates the total sales if qty changes
     * @param (int) -value
     */
    void on_sbSProdQty_valueChanged(int);

    /**
     * @brief on_txtSUnitPrice_textChanged - updates the total sales if price changes
     */
    void on_txtSUnitPrice_textChanged(const QString&);

    /**
     * @brief on_btnAddSale_clicked - adds a sale transaction to the DB
     */
    void on_btnAddSale_clicked();

    /**
     * @brief on_btnVSales_clicked - activates the view sales list page
     */
    void on_btnVSales_clicked();

    /**
     * @brief on_btnVStock_clicked - activates the view stock and refreshes stock
     */
    void on_btnVStock_clicked();

    /**
     * @brief on_sbPQty_valueChanged -adjusts the total cost when qty changes
     * @param (int)
     */
    void on_sbPQty_valueChanged(int);

    /**
     * @brief on_txtPProdCost_textChanged -updates the total cost when cost changes
     * @param (string)
     */
    void on_txtPProdCost_textChanged(const QString&);

    /**
     * @brief on_btnClearUser_clicked - triggers the clear user method
     */
    void on_btnClearUser_clicked();

    /**
     * @brief on_btnClearPurchase_clicked - triggers the clear purchases method
     */
    void on_btnClearPurchase_clicked();

    /**
     * @brief on_btnCLearSale_clicked -triggers the clear sale method
     */
    void on_btnCLearSale_clicked();

    /**
     * @brief on_btnFindProd_clicked - opens up the search window to display product details
     */
    void on_btnFindProd_clicked();

    /**
     * @brief loadStock -loads stock
     */
    void loadStock();

    /**
     * @brief on_txtPurchaseFilter_textChanged -filters the list of purchases
     * @param (string) -product code
     */
    void on_txtPurchaseFilter_textChanged(const QString&);

    /**
     * @brief on_txtSaleFilter_textChanged -filters the list of sales
     * @param (string) - product code
     */
    void on_txtSaleFilter_textChanged(const QString&);

    void on_btnHelp_clicked();
};

#endif // MAINSCREEN_H
