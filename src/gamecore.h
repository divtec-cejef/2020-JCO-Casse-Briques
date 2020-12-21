/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   RBR
  \date     décembre 2020
*/
#ifndef GAMECORE_H
#define GAMECORE_H


#include <QObject>
#include <QPointF>
#include <QString>
#include <QColor>
#include <QGraphicsSimpleTextItem>


class GameCanvas;
class GameScene;
class Sprite;


//! \brief Classe qui gère la logique du jeu.
//!
//! Dans son état actuel, cette classe crée une scène vide, délimite
//! sa surface avec une ligne blanche puis démarre la cadence du jeu.
class GameCore : public QObject
{
    Q_OBJECT
public:
    explicit GameCore(GameCanvas* pGameCanvas, QObject *parent = nullptr);
    ~GameCore();

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(QPointF newMousePosition);
    void mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);

    void tick(long long elapsedTimeInMilliseconds);

signals:
    void notifyMouseMoved(QPointF newMousePosition);
    void notifyMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

private:
    GameCanvas* m_pGameCanvas = nullptr;
    GameScene* m_pScene = nullptr;
    GameScene* m_pSceneMenu = nullptr;
    GameScene* m_pSceneWin = nullptr;
    GameScene* m_pSceneLoss = nullptr;
    Sprite* m_pPlayer = nullptr;
    Sprite* m_pBloc = nullptr;
    int m_PlayerDirection = 1;
    void setupBlueBall();
    void setupBouncingArea();
    Sprite* m_pTennisBall = nullptr;
    bool m_keySpacePressed = false;
    bool m_keyEscPressed = false;
    bool isWaiting = false;
    bool isDead = false;
    int playerLife = 3;
    int spaceLines = 0;
    int spaceColumns = 0;
    int counterBlock = 54;
    QGraphicsSimpleTextItem* textLifePlayer = nullptr;



private slots:
    void onSpriteDestroyed(QObject* pSprite);

};


#endif // GAMECORE_H
