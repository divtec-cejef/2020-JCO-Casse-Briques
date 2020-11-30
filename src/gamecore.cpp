/**
  Fichier qui contient toute la logique du jeu.
  
  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"
#include "sprite.h"

#include <cmath>

#include <QDebug>
#include <QSettings>
#include <QPainter>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "blueball.h"
#include "bouncingspritehandler.h"

const int SCENE_WIDTH = 1280;
const int PLAYER_SPEED = 150;
const QPointF BOUNCING_AREA_POS(700,300);
const float BOUNCING_AREA_SIZE = 86.5;

Sprite* m_pPlayer;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {

    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;
    
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);
    
    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));
    
    // Instancier et initialiser les sprite ici :

    // Création de la zone de rebond
    setupBouncingArea();

    // Ajout d'un sprite du joueur (rectangle)
    Sprite* pSprite = new Sprite(GameFramework::imagesPath() + "rectangle2.png");
    m_pScene->addSpriteToScene(pSprite);
    pSprite->setPos(m_pScene->width()/2.00, 700);
    m_pPlayer = pSprite;

    // Animations du personnage (rectangle)
    pSprite->addAnimationFrame(GameFramework::imagesPath() + "rectangle2_vert.png");
    pSprite->startAnimation(1000);


    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();

}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {

    emit notifyKeyPressed(key);

    switch(key) {
    case Qt::Key_Left:
        if(m_pPlayer->left())
         m_pPlayer->setX(m_pPlayer->x() - 20); break;

    case Qt::Key_Right:
        if(m_pPlayer->right() < m_pScene->width() - 10)
        m_pPlayer->setX(m_pPlayer->x() + 20); break;
    }
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
    float distance = PLAYER_SPEED * elapsedTimeInMilliseconds / 1000.0F * m_PlayerDirection;

    // Test si la balle touche le mur, la téléporte en haut
    if (pTennisBall->y() >= 684) {
        pTennisBall->setPos(pTennisBall->x(),0);
        //qDebug() << pTennisBall->y();
    }

    m_pPlayer->setX(m_pPlayer->x());
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}


//! Construit la zone de rebond, ainsi que la balle de tennis qui va s'y déplacer.
void GameCore::setupBouncingArea() {
    const int BRICK_SIZE = 15;

    // Création des briques de délimitation de la zone et placement
    QPixmap smallBrick(GameFramework::imagesPath() + "wall.png");
    smallBrick = smallBrick.scaled(BRICK_SIZE,BRICK_SIZE);

    // Création d'une image faite d'une suite horizontale de briques
    QPixmap horizontalWall(BRICK_SIZE * BOUNCING_AREA_SIZE, BRICK_SIZE);
    QPainter painterHW(&horizontalWall);
    for (int col = 0; col < BOUNCING_AREA_SIZE; col++)
         painterHW.drawPixmap(col * BRICK_SIZE,0, smallBrick);

    // Création d'une image faite d'une suite verticale de briques
    QPixmap verticalWall(BRICK_SIZE, BRICK_SIZE * BOUNCING_AREA_SIZE - 38);
    QPainter painterVW(&verticalWall);
    for (int col = 0; col < BOUNCING_AREA_SIZE - 38; col++)
         painterVW.drawPixmap(0, col * BRICK_SIZE, smallBrick);

    // Ajout de 4 sprites (utilisant les murs horizontaux et verticaux) pour délimiter
    // une zone de rebond.
    m_pScene->addSpriteToScene(new Sprite(horizontalWall), 0, 0);
    m_pScene->addSpriteToScene(new Sprite(horizontalWall), 0, 720);

    m_pScene->addSpriteToScene(new Sprite(verticalWall), -15, 0);
    m_pScene->addSpriteToScene(new Sprite(verticalWall), 1281, 0);

    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::red));

    // Création de la balle de tennis qui rebondi
    pTennisBall = new Sprite(GameFramework::imagesPath() + "basket.png");
    pTennisBall->setTickHandler(new BouncingSpriteHandler);
    pTennisBall->setPos(BOUNCING_AREA_POS + QPointF(10,100));
    m_pScene->addSpriteToScene(pTennisBall);

    pTennisBall->registerForTick();
}
