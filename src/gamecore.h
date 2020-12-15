/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   JCO
  \date     septembre 2018
*/
#ifndef GAMECORE_H
#define GAMECORE_H


#include <QObject>
#include <QPointF>


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
    Sprite* m_pPlayer = nullptr;
    Sprite* m_pBloc = nullptr;
    int m_PlayerDirection = 1;
    void setupBlueBall();
    void setupBouncingArea();
    Sprite* m_pTennisBall = nullptr;
    bool m_keySpacePressed = false;;
    bool isWaiting = false;
    int playerLife = 5;
    int playerLifeCompare = 5;



private slots:

};


#endif // GAMECORE_H
