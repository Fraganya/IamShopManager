#ifndef PURCHASE_H
#define PURCHASE_H

#include "product.h"

class Purchase:public Product
{
    public:
      Purchase();
      QSqlQuery getQuery();

      bool isValid();

      double getTotalCost() const;
      void setTotalCost(double value);

      void calcTotalCost();

    private:
          double totalCost;
};

#endif // PURCHASE_H
