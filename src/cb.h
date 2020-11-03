#ifndef CB_H
#define CB_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class cb; }
QT_END_NAMESPACE

class cb : public QMainWindow
{
    Q_OBJECT

public:
    cb(QWidget *parent = nullptr);
    ~cb();

private slots:
    void on_pushButton_clicked();

    void on_actionQuitter_triggered();

    void on_pushButton_2_clicked();

private:
    Ui::cb *ui;
};
#endif // CB_H
