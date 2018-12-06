#ifndef SALE_H
#define SALE_H

#include "product.h"

class Sale:public Product
{
    public:
        Sale();

        bool isValid();
        QSqlQuery getQuery();
        void calcTotalSale();

    private:
      double totalSale;

};

#endif // SALE_H
