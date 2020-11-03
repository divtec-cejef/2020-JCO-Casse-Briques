#include "cb_jeu.h"
#include "ui_cb_jeu.h"

cb_jeu::cb_jeu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cb_jeu)
{
    ui->setupUi(this);

    QPixmap background("C:/Users/broqray/OneDrive - DIVTEC/Atelier/atelier/cpp.png");
    ui->label_bg->setPixmap(background);
}

cb_jeu::~cb_jeu()
{
    delete ui;
}

void cb_jeu::on_pushButton_clicked()
{
    this->close();
}

