#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QSqlQuery>
#include <QVariant> //for conversion of QString when binding;

class Product
{
    public:
        Product();

        virtual QSqlQuery getQuery();
        bool validateProd();
        bool validatePrice();

        QString getProdCode() const;
        void setProdCode(const QString &value);

        QString getProdName() const;
        void setProdName(const QString &value);

        QString getProdUniPrice() const;
        void setProdUniPrice(const QString &value);

        int getProdQty() const;
        void setProdQty(int value);

private:
        QString prodCode;
        QString prodName;
        QString prodUniPrice; //these will be received as strings from QLineEdit

    protected:
        int prodQty;

};

#endif // PRODUCT_H
