#include "cb.h"
#include "ui_cb.h"
#include "cb_jeu.h"

cb::cb(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cb)
{
    ui->setupUi(this);

    QPixmap pix("C:/Users/broqray/OneDrive - DIVTEC/Atelier/atelier/cbrique.jpg");
    ui->label_img->setPixmap(pix);
}

cb::~cb()
{
    delete ui;
}


void cb::on_pushButton_clicked()
{
    window()->close();
}

void cb::on_actionQuitter_triggered()
{
    this->close();
}

void cb::on_pushButton_2_clicked()
{
    this->close();
    cb_jeu cbjeu;
    cbjeu.setModal(true);
    cbjeu.exec();

}
