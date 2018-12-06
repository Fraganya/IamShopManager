#ifndef PRODUCTSCREEN_H
#define PRODUCTSCREEN_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class productScreen;
}

class productScreen : public QDialog
{
    Q_OBJECT

public:
    explicit productScreen(QWidget *parent = 0,QString prod="");
    ~productScreen();

private:
    Ui::productScreen *ui;

    void findProduct(QString& prod);
};

#endif // PRODUCTSCREEN_H
