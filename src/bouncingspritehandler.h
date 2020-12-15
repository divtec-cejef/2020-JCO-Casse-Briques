/**
  \file
  \brief    Déclaration de la classe BouncingSpriteHandler.
  \author   RBR
  \date     décembre 2020
*/
#ifndef BOUNCINGSPRITEHANDLER_H
#define BOUNCINGSPRITEHANDLER_H

//! \brief Classe qui gère le déplacement par rebond d'un sprite.
//!

#include <QPointF>

#include "spritetickhandler.h"

class BouncingSpriteHandler : public SpriteTickHandler
{
public:
    BouncingSpriteHandler(Sprite* pParentSprite = nullptr);

    void setSpriteVelocity(QPointF spriteVelocity);
    void setSpriteVelocity(double xVelocity, double yVelocity);

    void tick(long long elapsedTimeInMilliseconds);

private:
    QPointF m_spriteVelocity;
    int initialVelocity= 100;

};

#endif // BOUNCINGSPRITEHANDLER_H
