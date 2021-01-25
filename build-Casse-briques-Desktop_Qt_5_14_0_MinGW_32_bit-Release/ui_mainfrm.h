/********************************************************************************
** Form generated from reading UI file 'mainfrm.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINFRM_H
#define UI_MAINFRM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gameview.h"

QT_BEGIN_NAMESPACE

class Ui_MainFrm
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    GameView *grvGame;

    void setupUi(QWidget *MainFrm)
    {
        if (MainFrm->objectName().isEmpty())
            MainFrm->setObjectName(QString::fromUtf8("MainFrm"));
        MainFrm->setEnabled(true);
        MainFrm->resize(1400, 1100);
        QFont font;
        font.setFamily(QString::fromUtf8("8514oem"));
        font.setPointSize(15);
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        MainFrm->setFont(font);
        MainFrm->setCursor(QCursor(Qt::PointingHandCursor));
        MainFrm->setWindowTitle(QString::fromUtf8("Casse-briques"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../res/images/icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainFrm->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(MainFrm);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(MainFrm);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("OCR A Extended"));
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        label->setFont(font1);
        label->setScaledContents(false);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        grvGame = new GameView(MainFrm);
        grvGame->setObjectName(QString::fromUtf8("grvGame"));
        grvGame->setMaximumSize(QSize(16777215, 16777215));
        QFont font2;
        font2.setFamily(QString::fromUtf8("8514oem"));
        font2.setPointSize(15);
        font2.setBold(true);
        font2.setUnderline(true);
        font2.setWeight(75);
        font2.setKerning(true);
        grvGame->setFont(font2);
        grvGame->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(grvGame);


        retranslateUi(MainFrm);

        QMetaObject::connectSlotsByName(MainFrm);
    } // setupUi

    void retranslateUi(QWidget *MainFrm)
    {
        label->setText(QCoreApplication::translate("MainFrm", " Jouable clavier/souris | Amusez-vous bien et bonne chance !", nullptr));
        (void)MainFrm;
    } // retranslateUi

};

namespace Ui {
    class MainFrm: public Ui_MainFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFRM_H
