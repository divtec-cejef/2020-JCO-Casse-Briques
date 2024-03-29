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
#include <QImage>

class GameCanvas;
class GameScene;
class Sprite;

//! \brief Classe qui gère la logique du jeu.
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
    GameScene* m_pSceneGame = nullptr;
    GameScene* m_pSceneMenu = nullptr;
    GameScene* m_pSceneWin = nullptr;
    GameScene* m_pSceneLoss = nullptr;

    Sprite* m_pPlayer = nullptr;
    Sprite* m_pBloc = nullptr;
    Sprite* m_pBall = nullptr;
    Sprite* m_pButtonLeave = nullptr;
    Sprite* m_pButtonRestart = nullptr;
    Sprite* m_pButtonResume = nullptr;
    Sprite* m_pWinGame = nullptr;
    Sprite* m_pLossGame = nullptr;
    Sprite* m_pTrophy = nullptr;
    Sprite* m_pLifePlayer1 = nullptr;
    Sprite* m_pLifePlayer2 = nullptr;
    Sprite* m_pLifePlayer3 = nullptr;
    Sprite* m_pLifePlayerEmpty = nullptr;

    bool m_keySpacePressed = false;
    bool m_keyEscPressed = false;
    bool m_isDead = false;
    bool m_onClick = false;
    bool m_isRestart = false;
    bool m_isWaiting = true;

    int m_PlayerDirection = 1;
    int m_playerLife = 3;
    int m_playerLifeDisplayX = 10;
    int m_playerLifeDisplayY = -80;
    int m_spaceLines = 0;
    int m_spaceColumns = 0;
    int m_counterBlock = 54;

    QColor m_colorReturnMenu = QColor(107,245,138);
    QColor m_colorBackGround = QColor(200,191,231);

    QGraphicsSimpleTextItem* m_textLifePlayer = nullptr;
    QGraphicsSimpleTextItem* m_textMenuResume = nullptr;
    QGraphicsSimpleTextItem* m_textMenuRestart = nullptr;
    QGraphicsSimpleTextItem* m_textMenuLeave = nullptr;

    void setupBlueBall();
    void setupBouncingArea();
    void setupBackGroundMenu();
    void createBlock();
    void createPlayer();
    void createButton();
    void createLifePlayer();
    void createSceneWin();
    void createSceneLoss();
    void createSceneGame();
    void createSceneMenu();


private slots:
    void onSpriteDestroyed();
};


#endif // GAMECORE_H
