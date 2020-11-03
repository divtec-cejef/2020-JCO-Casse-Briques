#ifndef CB_JEU_H
#define CB_JEU_H

#include <QDialog>

namespace Ui {
class cb_jeu;
}

class cb_jeu : public QDialog
{
    Q_OBJECT

public:
    explicit cb_jeu(QWidget *parent = nullptr);
    ~cb_jeu();

private slots:
    void on_pushButton_clicked();

private:
    Ui::cb_jeu *ui;
};

#endif // CB_JEU_H
